# Patch proposal 0002 — Modbus hardening

**Target:** `sensors.cpp` (`sensor_api_init` OSPI RS485 path)  
**Upstream baseline:** `3a342e8`

## Problems

```cpp
ctx = modbus_new_tcp(...); // or modbus_new_rtu(...)
modbus_rtu_set_serial_mode(ctx, ...); // unconditional
modbus_rtu_set_rts(ctx, ...);
modbus_set_response_timeout(ctx, ...);
modbus_connect(ctx);
```

1. No NULL check after `modbus_new_*` → crash or UB on allocation / argument failure.
2. RTU-only helpers invoked for TCP contexts.
3. Failure of one adapter should not abort unrelated sensor init.

## Proposed / implemented approach

- After create: if `!ctx`, log error and continue to next line.
- Track whether context is RTU; call `modbus_rtu_set_*` only for RTU.
- On connect failure: `modbus_free(ctx)` and continue.
- Optional thin wrapper used by unit tests to assert create/classify behaviour.

## Compatibility

- Successful RS485 / TCP adapter bring-up behaviour unchanged for valid devices.
