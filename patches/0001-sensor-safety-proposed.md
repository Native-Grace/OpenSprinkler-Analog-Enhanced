# Patch proposal 0001 — Sensor safety

**Target:** `SensorBase.hpp`, `sensors.cpp`, new `sensor_validation.*`  
**Upstream baseline:** `3a342e8`

## Problems

1. No central validation for divider, read interval, port, Modbus address, ADS channel, type, duplicates.
2. `sensor_define` deletes the existing sensor before the replacement is constructed and initialized.
3. `fromJson` mutates runtime state from user/config payloads.
4. Duplicate `nr` during load overwrites map pointers without freeing the previous object.

## Proposed / implemented approach

- Add `sensor_validate_config()` returning explicit error codes / messages.
- Reject `divider == 0`, `ri` below safe minimum, port outside 1..65535, invalid Modbus slave ids, out-of-range channels, unknown types.
- Split JSON: `fromConfigJson` / `toConfigJson` / `toStatusJson`; ignore runtime-only keys on config apply.
- Transactional replace: create → configure → validate → `init()` → swap; keep old sensor on failure.
- On load, skip/log duplicates without leaking the previous object.

## Compatibility

- Existing HTTP APIs continue to accept the same config keys.
- Status responses may still include runtime fields via `toStatusJson` / legacy `toJson` compatibility path.
