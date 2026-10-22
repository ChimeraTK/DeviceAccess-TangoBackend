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
  (R6). A read has no side effects.

R3. Type coverage.

- Every `CmdArgType` value is mapped, for arguments and results alike, except
  `DevPipeBlob`, which is rejected with `logic_error`.
- Scalars map as the attribute path already maps them: `DevState` maps to an
  unsigned 8 bit numeric, `DevString` and `ConstDevString` to string.
- A homogeneous array maps to one register carrying its element's type.
- The mixed pairs are split into a numeric and a string register:
  - `/Commands/<Cmd>/arg/values` and `/Commands/<Cmd>/arg/strings`,
  - `/Commands/<Cmd>/result/values` and `/Commands/<Cmd>/result/strings`.
- `DevEncoded` and `DevVarEncodedArray` are split into a format and a data part:
  - `/Commands/<Cmd>/arg/format` and `/Commands/<Cmd>/arg/data`,
  - `/Commands/<Cmd>/result/format` and `/Commands/<Cmd>/result/data`.

R4. Array lengths.

- Tango declares no length for array arguments and results; the user picks it with
  `numberOfWords` when the accessor is created.
- The catalogue reports one element for every command register.
- A written argument is sent with exactly the register length.
- A result with more elements than the buffer throws `runtime_error`.
- A result with fewer elements fills the leading elements only.
- `numberOfWords == 0` for a command register throws `logic_error`.

R5. Failures.

- A Tango `DevFailed` while executing a command or while querying the command
  list is reported as `ChimeraTK::runtime_error` naming the command.
- A rejected command does not mark the backend faulty.
- A trigger write or an argument write while the device is closed throws
  `logic_error`, as attribute access does today.

R6. Asynchronous result reads.

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
- One new version per execution, shared by a command's result register and its
  composite parts; plain reads of an unchanged result keep the previous version.
- A consumer slower than the command skips intermediate results; every read
  returns the newest cached result of that command.

R7. Offline cache.

- The cache file gains a top-level `commands` array with `name`, `inType` and
  `outType` per command; `tools/generateCacheFile.py` dumps commands as well.
- A cache file without the `commands` key still loads and yields a catalogue
  without commands.
- A malformed `commands` entry is rejected with `logic_error` naming the file,
  matching the existing catalogue validation.

R8. Test server.

- The POGO-generated test server gains the commands the command tests need.
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
  `trigger`, `argument`, `result` and the composite parts) plus, for command
  registers, the command name, the part's `CmdArgType` and the input/output
  direction.
- One `TangoRegisterInfo` per register, so the existing interface methods answer
  per part: trigger and argument are write-only, result is read-only.
- `isReadable()` and `isWriteable()` drive the existing accessor checks;
  `getSupportedAccessModes()` stays empty for every register except result
  registers (see below).
- `getNumberOfElements()` and `getNumberOfChannels()` return 1 for every command
  register. The array length is a property of the accessor (R4), not of the
  catalogue.
- `util::mapTangoToUserType` is extended by the array types (through their element
  type), `ConstDevString`, `DevEncoded` and `DevVarEncodedArray`; `DevPipeBlob`
  throws `logic_error`.
- `open()` assembles the catalogue from `attribute_list_query_ex()` as today, then
  appends the command registers from `command_list_query()`, skipping `Init`.

### Command state

- The backend holds one state object per command: the last written argument, the
  last `Tango::DeviceData` returned by the server, and flags for "argument
  written" and "executed".
- The state is type agnostic: the argument is packed into a `Tango::DeviceData` at
  trigger time from the part registers, so no per-type storage is needed.
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
- The result accessor unpacks the stored `Tango::DeviceData` into its buffer,
  applying R4's length rules and reporting `DataValidity::ok`. Scalars use the
  `DeviceData` scalar operators, arrays the array operators, `DevState` the array
  pointer operator.
- Version numbers follow the attribute path: one new version per read.

### Composite parts

- A composite argument needs both parts written. Each part has its own "written"
  flag, and a trigger with a missing part throws `logic_error` as for a plain
  argument (R2).
- Packing builds `DevVarLongStringArray{lvalue, svalue}`,
  `DevVarDoubleStringArray{dvalue, svalue}` or
  `DevEncoded{encoded_format, encoded_data}` from the two registers.
- Unpacking reads both parts back from the stored `Tango::DeviceData`.

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
- One 2D string register for the mixed pairs: rejected, the numeric part would
  lose its type.
- Waiting for a command concept in DeviceAccess: 03.31 has none, so the convention
  has to live in this backend.
- Regenerating the test server with POGO: POGO is not installed here, so the
  generated files are edited by hand.
- The DeviceAccess async framework (`SubDomain` with a push distributor) for
  command results: not usable, the non-void push path is not instantiated in
  03.31 and the needed backend hooks live on `NumericAddressedBackend`.
- One queued notification per execution, blocking the trigger write while the
  consumer lags: rejected, a command write must not stall (R6).

### Known gaps

- `DevPipeBlob` is not usable as a command type and is rejected.
- `ConstDevString` and `DevEnum` are mapped defensively; Tango does not allow them
  as command argument or result types, so no test server command exercises them.
- Should the server refuse to register a command of an exotic `CmdArgType`
  (`DevVarStateArray`, `DevVarEncodedArray`), that type stays untested; the
  mapping exists regardless.
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
    `State` and `Status` present with their result registers;
  - void to void command: one trigger write executes it exactly once, visible
    through a companion command reporting the execution count;
  - argument stickiness: two triggers without a new argument send the same value;
    a trigger before any argument write throws `logic_error`;
  - result: a read before execution throws `logic_error`, a read after execution
    returns the value, a second execution replaces it;
  - per supported type: write the argument, trigger, compare the result, using an
    echo command per type including the arrays, the mixed pairs and the encoded
    types;
  - array lengths: fewer elements fill the leading buffer elements, more elements
    throw `runtime_error`, `numberOfWords == 0` throws `logic_error`;
  - async result: a `read()` from a second thread blocks until the trigger write
    and returns the result; `readNonBlocking()` is false before an execution and
    leaves the buffer untouched, true after it; `readLatest()` returns the cached
    result;
  - async result of a command executed before the accessor existed returns the
    cached result without waiting;
  - versions: two plain reads of an unchanged result share a version, and after
    an execution the plain and the async accessor report the same version;
  - composite result parts are both notified by one execution, with the same
    version;
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
