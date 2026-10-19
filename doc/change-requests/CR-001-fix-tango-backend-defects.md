# CR-001: Fix Tango backend defects found in code review

Synopsis: A read-only code review found several defects in the Tango backend:
incorrect enum reads, a possible overrun on partial reads, unsafe handling of a
malformed offline cache and of empty Tango error lists, a dead declaration, and
test-scaffolding inconsistencies.

Status: DONE

## Requirements

R1. Enum attributes read correctly.

- Currently `doPostRead` in `include/TangoRegisterAccessor.h` uses two
independent `if constexpr` blocks, so `DEV_ENUM` data is overwritten by the
generic conversion.
- Replace the second `if constexpr` with `else if constexpr`, matching
`doPreWrite`.

R2. Partial reads never overrun the source vector.

- Currently the read length is bounded only by `value.size()`, not by
`value.size() - elementOffset`.
- A server returning fewer elements than the requested span must not cause
undefined behaviour.

R3. A malformed offline cache is rejected cleanly instead of crashing or giving
a misleading message.

- Currently `OfflineCache::read` in `src/OfflineCache.cc` copies and uses
`data["catalogue"]` before validating it, and does not check that the JSON
root is an object.
- A malformed cache must produce a `ChimeraTK::logic_error` naming the file.

R4. Connection errors always produce a usable message.

- Currently `src/TangoBackend.cc` indexes `ex.errors[0]` without checking that
the sequence is non-empty; Tango allows an empty `errors`.
- A connection failure with no error detail must still produce a readable
message.
- The connection-error messages are built with `std::format`, replacing plain
string concatenation.

R6. Header clean-up in `include/TangoBackend.h` and removal of the proxy mutex.

- Remove the dead `TangoBackend::mapTangoType()` declaration; it is declared but
never defined or used.
- Hold the device proxy as a plain
`std::shared_ptr<Tango::DeviceProxy> _deviceProxy;`.
- `getDeviceProxy()` returns the copy by value without a mutex; the returned
shared pointer keeps the proxy alive for the duration of a call.
- Remove the `_deviceProxyMutex` member and the lock statements in `open()` and
`close()` accordingly.

R7. Test scaffolding is de-duplicated and consistent.

- `ScalarDefaults` in `tests/executables_src/testUnifiedBackend.cpp` duplicates
`ScalarDefaultsBase`.
- `RegSomeRoBool::setValue` writes via `readPath()`; all siblings use
`writePath()`.
- `RegSomeStringArray` carries an unused `int32_t increment` for a string
register.

R8. Validity propagation is tested for the valid case too.

- `tests/executables_src/testValidityPropagation.cpp` only asserts `faulty`, so
a regression marking everything faulty would pass.
- Add a `DataValidity::ok` case.

## Specifications

Affected components: `include/TangoBackend.h` (R6),
`include/TangoRegisterAccessor.h`, `src/TangoBackend.cc`, `src/OfflineCache.cc`,
and the two test files named above. No public interface change except R6
(removal of an already-dead declaration).

### R1 and R2: `doPostRead`

- Change the `DEV_ENUM`/`DEV_STATE`/generic chain to a single
`else if constexpr` chain, as in `doPreWrite`.
- Guard with `elementOffset <= value.size()` before computing the source range.
- Clamp the read length to `value.size() - elementOffset` instead of
`value.size()`.
- Where the server returns fewer elements than requested, the trailing buffer
elements keep their previous contents; the data validity reflects the
attribute quality as before.

### R3: `OfflineCache::read`

- Validate `data.is_object()` and `data.contains("catalogue")` before touching
the value.
- Then bind a `const auto&` to `data["catalogue"]` and validate it is an array.
- Keep the existing scalar/spectrum/image classification.
- Keep the deliberate `max_dim_x == 1` -> `SCALAR` convention; add a brief
comment explaining it.

### R4: connection error message

- Build the message from `ex.errors` only when the sequence is non-empty.
- Otherwise use a generic fallback that still names the address.
- Compose the `WrongNameSyntax` and `ConnectionFailed` messages and the
`CORBA::Exception` fallback with `std::format`, replacing string concatenation.

### R6: header clean-up and device proxy ownership

- Remove the dead `TangoBackend::mapTangoType()` declaration from
`include/TangoBackend.h`.
- Hold the device proxy as a plain `std::shared_ptr<Tango::DeviceProxy>
_deviceProxy;` in `include/TangoBackend.h`.
- `getDeviceProxy()` returns the copy by value without a mutex; the returned
shared pointer keeps the proxy alive for the duration of a call, so no mutex
is needed.
- No `_deviceProxyMutex` and no locking in `open()` or `close()` are needed;
remove the mutex member and the lock statements accordingly.

### Alternatives considered

- Removing `getDeviceProxy()` in favour of pimpl/accessor-only access: larger
refactor, deferred.

### Out of scope and known gaps

- Enum/`DevState` test coverage: would need a new attribute in the
POGO-generated `tests/generated/TangoTestServer*` files, which are
do-not-modify. Known coverage gap; the generated files are not touched.
- No new test coverage for the enum path, the partial-read overrun, the
malformed offline cache or the empty error list; the fixes only need to keep
behaviour well-defined.
- `IMAGE` format handling: `getNumberOfElements()` returning only `max_dim_x`
is unreachable because `IMAGE` is rejected at accessor creation.
- `getDeviceProxy()`: reviewed and kept as the existing accessor (R6 updates its
ownership to a plain `std::shared_ptr` without a mutex); removing it in favour of
pimpl/accessor-only access is a larger refactor, deferred (see Alternatives
considered).
- Offline-cache data format: the cache file stores only length (`max_dim_x`) and
channels (`max_dim_y`), so `OfflineCache::read` re-derives scalar/spectrum/image
from dimensions. A scalar and a one-element spectrum (dimensions `max_dim_x == 1`
and `max_dim_y == 0`) are inherently indistinguishable. Persisting the
attribute's real `data_format` in the cache file, as the live `open()` path gets
from `attribute_list_query_ex`, would remove the ambiguity; deferred as a known
gap.

## Test plan

- Extend `tests/executables_src/testValidityPropagation.cpp` with a valid
attribute case asserting `DataValidity::ok`.
- Use the existing `ChangingValue` attribute for the valid case: its quality is
`ATTR_CHANGING`, which the backend maps to `DataValidity::ok`. A dedicated
valid attribute would need a change to the do-not-modify generated server
files (see known gaps).
- The existing `testUnifiedBackend` exercises the R7 refactored scaffolding and
must keep passing.
