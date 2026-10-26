# CR-002: Tango command support in the Tango backend

Synopsis: The backend exposes Tango attributes only. Expose the device's commands
as registers under a `/Commands/` namespace, executed by writing a void trigger
register, with arguments in companion registers and results read on demand or
with `wait_for_new_data`.

Status: PLANNED

## Requirements

Synopsis: commands become visible in the catalogue and executable through the
device API; the offline catalogue can describe them as well.

R1. Commands appear in the catalogue.

- `open()` adds the device's commands, next to the attributes it adds today.
- Every command is exposed except `Init`.
- The built-in `State` and `Status` commands are exposed like any other command.
- All command registers live under `/Commands/`, so a command and an attribute of
  the same name never clash. Attribute registers keep their current names.
- Per command `<Cmd>` the backend exposes:
  - `/Commands/<Cmd>`: the write-only void trigger, always present.
  - `/Commands/<Cmd>/arg`: the write-only argument, present when the input
    type is not `DevVoid`.
  - `/Commands/<Cmd>/result`: the read-only result, present when the output
    type is not `DevVoid`.

R2. Execution semantics.

- Writing the trigger executes the command exactly once, synchronously, with the
  current value of `/Commands/<Cmd>/arg`.
- The argument is sticky: it keeps the last written value.
- A trigger for a command with an argument never written since `open()` throws
  `logic_error`.
- The result of the execution becomes the content of `/Commands/<Cmd>/result`.
- Reading `/Commands/<Cmd>/result` with a plain accessor before the command ran
  once throws `logic_error`; an accessor with `wait_for_new_data` waits instead
  (R5). A read has no side effects.

R3. Type coverage.

- Only fixed-size scalar types are supported, for arguments and results alike:
  `DevVoid`, `DevBoolean`, `DevShort`, `DevLong`, `DevFloat`, `DevDouble`,
  `DevUShort`, `DevULong`, `DevString`, `ConstDevString`, `DevState`, `DevEnum`,
  `DevUChar`, `DevLong64` and `DevULong64`.
- Arrays are unsupported: neither the homogeneous `DevVar*` types nor the mixed
  pairs `DevVarLongStringArray` and `DevVarDoubleStringArray`.
- `DevEncoded`, `DevVarEncodedArray` and `DevPipeBlob` are unsupported as well.
- A command whose argument or result type is unsupported is skipped entirely and
  logged as a warning while `open()` builds the catalogue; the backend never lists
  a command it cannot serve.
- The supported types map as the attribute path already maps them: `DevState` to
  an unsigned 8 bit numeric, `DevString` and `ConstDevString` to string, `DevEnum`
  to a 64 bit signed integer.

R4. Failures.

- A Tango `DevFailed` while executing a command or while querying the command
  list is reported as `ChimeraTK::runtime_error` naming the command.
- A rejected command does not mark the backend faulty.
- A trigger write or an argument write while the device is closed throws
  `logic_error`, as attribute access does today.

R5. Asynchronous result reads.

- A result register accepts `AccessMode::wait_for_new_data`; trigger and argument
  keep rejecting it, as every register does today.
- Requesting it for a register that does not advertise it throws `logic_error`.
- Every execution notifies each subscribed result accessor once; a trigger write
  never blocks.
- `read()` waits for the next execution. `readNonBlocking()` returns false and
  leaves the buffer untouched while no execution is pending. `readLatest()`
  returns the cached result.
- An accessor created after an execution starts from the cached result and does
  not wait.
- One new version per execution; plain reads of an unchanged result keep the
  previous version.
- A consumer slower than the command skips intermediate results; every read
  returns the newest cached result of that command.

R6. Offline cache.

- The cache file gains a top-level `commands` array with `name`, `inType` and
  `outType` per command; `tools/generateCacheFile.py` dumps commands as well.
- A cache file without the `commands` key still loads and yields a catalogue
  without commands.
- A malformed `commands` entry is rejected with `logic_error` naming the file,
  matching the existing catalogue validation.

R7. Test server.

- The POGO-generated test server gains the commands the command tests need; every
  added command has a supported argument and result type (R3).
- `tests/generated/TangoTestServer.xmi` and the generated sources are kept
  consistent by hand; POGO is not available in this environment.

## Specifications

Affected components: `include/RegisterCatalogue.h`, `src/RegisterCatalogue.cc`,
`src/TangoBackend.cc`, a new command accessor header, `include/OfflineCache.h`,
`src/OfflineCache.cc`, `tools/generateCacheFile.py`, `tests/generated/` and new
test sources.

### Catalogue

- `TangoRegisterInfo` wraps a `Tango::AttributeInfoEx` today and is the only
  register type of `TangoRegisterCatalogue`. It gains a role (`attribute`,
  `trigger`, `argument`, `result`) plus, for command registers, the command name,
  the `CmdArgType` and the input/output direction.
- One `TangoRegisterInfo` per register, so the existing interface methods answer
  per register: trigger and argument are write-only, result is read-only.
- `isReadable()` and `isWriteable()` drive the existing accessor checks;
  `getSupportedAccessModes()` stays empty for every register except result
  registers (see below).
- `getNumberOfElements()` and `getNumberOfChannels()` return 1 for every command
  register, each command register holding a single scalar value (R3).
- `util::mapTangoToUserType` is extended by `ConstDevString` only; every other
  supported type is mapped already. An unsupported type never reaches it, because
  the command is skipped while the catalogue is built.
- `open()` assembles the catalogue from `attribute_list_query_ex()` as today, then
  appends the command registers from `command_list_query()`, skipping `Init` and
  the commands of unsupported type (R3).

### Command state

- The backend holds one state object per command: the last written argument, the
  last `Tango::DeviceData` returned by the server, and flags for "argument
  written" and "executed".
- The state is type agnostic: the argument is packed into a `Tango::DeviceData` at
  trigger time from the argument register, so no per-type storage is needed.
- `Tango::DeviceData` is copyable, so the result of an execution can be stored.
- The state is guarded by a mutex. The device proxy stays mutex free, as after
  CR-001.
- The state is created while the catalogue is built, so trigger, argument and
  result accessors share it and it outlives single accessors.

### Accessors

- `getRegisterAccessor_impl` dispatches on the register role instead of
  `attributeInfo.data_format`. Attribute registers keep their current accessor.
- The trigger accessor is an accessor over `ChimeraTK::Void` with one empty
  element. Its write packs the stored argument, calls `command_inout` and stores
  the returned data.
- The argument accessor converts its buffer to the Tango type, reusing the
  existing conversion helpers, and stores it in the command state.
- The result accessor unpacks the stored `Tango::DeviceData` into its single
  element buffer and reports `DataValidity::ok`. Scalars use the `DeviceData`
  scalar operators, `DevState` the array pointer operator.
- Version numbers follow the attribute path: one new version per read.

### Asynchronous result accessors

- `getSupportedAccessModes()` returns `{wait_for_new_data}` for result registers
  only; trigger, argument and attribute registers keep returning an empty set.
- The backend rejects `wait_for_new_data` for registers that do not advertise it
  itself; the framework only checks this in `NumericAddressedBackend`.
- The accessor installs the inherited void read queue
  (`TransferElement::_readQueue`, one slot) instead of relying on
  `doReadTransferSynchronously`, overrides `doPostRead` to apply the value only
  when a token was consumed, and overrides `interrupt()`.
- Each execution pushes one token from the triggering thread, overwriting a
  pending one, so the trigger write never blocks.
- The accessor takes the cached `Tango::DeviceData` and the version number from
  the command state when the token is consumed; data validity is always ok.
- The version number is therefore created at execution time, not at read time,
  and the command state stores it next to the result.
- Hand-rolling is needed because the framework's push distributor
  (`VariableDistributor` with a non-void payload) is not instantiable in 03.31
  and the async backend hooks (`activateSubscription`,
  `getAsyncDomainInitialValue`) exist only on `NumericAddressedBackend`. The
  base class supports a hand-rolled queue accessor fully.
- No activation handshake: results are delivered from accessor creation on,
  independent of `activateAsyncRead()`.

### Offline cache

- `OfflineCache::read()` fills the command part of the catalogue from the optional
  `commands` array, mirroring the existing attribute validation.

### Alternatives considered

- Reading the result executes the command: rejected, reads stay free of side
  effects and the trigger remains the only invocation path.
- Trigger `/Cmd/execute` without a namespace: rejected in favour of `/Commands/`,
  which also removes attribute clashes and keeps `State` and `Status` usable.
- Exposing array arguments and results with a user-chosen `numberOfWords`:
  rejected, Tango declares no length for them and `CommandInfo` carries none, so
  the backend cannot report a size that matches the data the server sends or
  expects. Commands of an unsupported type are skipped instead (R3).
- Waiting for a command concept in DeviceAccess: 03.31 has none, so the convention
  has to live in this backend.
- Regenerating the test server with POGO: POGO is not installed here, so the
  generated files are edited by hand.
- The DeviceAccess async framework (`SubDomain` with a push distributor) for
  command results: not usable, the non-void push path is not instantiated in
  03.31 and the needed backend hooks live on `NumericAddressedBackend`.
- One queued notification per execution, blocking the trigger write while the
  consumer lags: rejected, a command write must not stall (R5).

### Known gaps

- Commands with an array, encoded or pipe argument or result are not supported
  and are skipped by `open()`; a user cannot reach them through the backend.
- `ConstDevString` and `DevEnum` are mapped defensively; Tango does not allow them
  as command argument or result types, so no test server command exercises them.
- `UnifiedBackendTest` has no command concept, so command behaviour is covered by
  a dedicated test executable.
- Attributes still reject `wait_for_new_data`; supporting them needs Tango
  attribute subscriptions, which this change does not add.
- Command results have no activation or deactivation handshake; data is available
  as soon as the accessor exists.

## Test plan

- New executable `tests/executables_src/testCommands.cpp`, run against the test
  server through `TangoServerLauncher`:
  - catalogue: `/Commands/` registers for the server's commands, `Init` absent,
    `State` and `Status` present with their result registers, and a command with
    an array or encoded argument or result absent;
  - void to void command: one trigger write executes it exactly once, visible
    through a companion command reporting the execution count;
  - argument stickiness: two triggers without a new argument send the same value;
    a trigger before any argument write throws `logic_error`;
  - result: a read before execution throws `logic_error`, a read after execution
    returns the value, a second execution replaces it;
  - per supported type: write the argument, trigger, compare the result, using one
    echo command per supported type; `ConstDevString` and `DevEnum` stay untested
    (see gaps);
  - async result: a `read()` from a second thread blocks until the trigger write
    and returns the result; `readNonBlocking()` is false before an execution and
    leaves the buffer untouched, true after it; `readLatest()` returns the cached
    result;
  - async result of a command executed before the accessor existed returns the
    cached result without waiting;
  - versions: two plain reads of an unchanged result share a version, and after
    an execution the plain and the async accessor report the same version;
  - two executions before a read yield the newest result only;
  - `wait_for_new_data` for the trigger, the argument and an attribute throws
    `logic_error`;
  - failure: a command that throws on the server side yields `runtime_error`
    and a following attribute read still works;
  - closed device: a trigger write and a result read throw `logic_error`.
- Offline: a cache file with a `commands` array yields the command registers
  without a server; `unifiedBackendTestCache.json`, which has no `commands` key,
  keeps loading.
- The existing `testUnifiedBackend` and `testValidityPropagation` keep passing.
