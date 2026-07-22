# Architecture notes (Analog Enhanced)

## Sensor stack

```
HTTP / App API
    → sensor_define() / sensor_load()
        → sensor_validate_config()
        → SensorBase::fromConfigJson()
        → driver init()
    → poll loop (timing_elapsed_ge)
        → SensorBase::read()
        → toStatusJson() for live views
        → toConfigJson() for persistence
```

## Config vs runtime

| Layer | Methods | Persisted? |
|-------|---------|------------|
| Configuration | `fromConfigJson` / `toConfigJson` | Yes (`sensors.json`) |
| Status / live | `toStatusJson` / legacy `toJson` | No |

Runtime-only keys (`data_ok`, `last`, `data`, `nativedata`, `trend`, …) are ignored on config apply.

## Modbus (OSPI)

`rs485` file lines are classified with `modbus_classify_endpoint()`:

- TCP: host or `host:port` → `modbus_new_tcp`
- RTU: device path → `modbus_new_rtu` + RTU-only serial/RTS setup

Null contexts are never passed to libmodbus APIs; failures skip that adapter only.

## Timing

Scheduling uses wrap-safe `(uint32_t)(now - previous) >= interval`. Failure retries adjust `last_read` so the normal comparison waits `max(60, ri)` once (no interval doubling).
