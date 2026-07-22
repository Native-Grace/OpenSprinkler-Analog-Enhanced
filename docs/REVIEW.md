# Code review — Analog Sensor / Modbus reliability

Review baseline: upstream `3a342e8` (FW 2.4.0 build 221).

Status legend: **CONFIRMED** (reproduced in source), **FIXED** (in this fork), **UNCONFIRMED** (needs hardware), **PARTIAL**.

## Critical / high

| ID | Finding | Status | Notes |
|----|---------|--------|-------|
| M1 | `modbus_new_tcp` / `modbus_new_rtu` return value used without NULL check | CONFIRMED → FIXED | `sensors.cpp` `sensor_api_init` OSPI path |
| M2 | `modbus_rtu_set_serial_mode` / `modbus_rtu_set_rts` called on TCP contexts | CONFIRMED → FIXED | Same block; RTU-only APIs after TCP create |
| S1 | `divider == 0` accepted → division by zero risk | CONFIRMED → FIXED | `fromJson` / define path |
| S2 | No shared validation for port, Modbus id, channel, interval | CONFIRMED → FIXED | New `sensor_validation.*` |
| S3 | Duplicate `nr` on load overwrites map entry (leak) | CONFIRMED → FIXED | `sensor_parse_file` |
| S4 | Type-change replace deletes old sensor before new init succeeds | CONFIRMED → FIXED | `sensor_define` transactional |
| S5 | Config JSON accepts runtime fields (`data_ok`, `last`, `data`, …) | CONFIRMED → FIXED | `fromConfigJson` / `toStatusJson` |
| T1 | Elapsed checks use `now >= previous + interval` (overflow-unsafe) | CONFIRMED → FIXED | Sensor poll / retry paths |
| T2 | Failure path sets `last_read = time + interval` then compares with `+ interval` again (doubling) | CONFIRMED → FIXED | Retry deadline helper |
| T3 | Trend requires `t_newest > t_oldest + 3600` so exactly 1h fails “at least 1h” | CONFIRMED → FIXED | `<=` → span check with `>=` |
| A1 | ESP8266 ADS1115 fallback accepts bare I²C ACK | CONFIRMED → FIXED | Require register behaviour |
| A2 | ADS1115 scratch test leaves Lo_thresh altered | CONFIRMED → FIXED | Save/restore register |

## Medium

| ID | Finding | Status | Notes |
|----|---------|--------|-------|
| STR1 | Fixed buffers use `strncpy` without forced terminator | CONFIRMED → FIXED | Safe copy helpers |
| STR2 | Hostname / path buffers need audit | PARTIAL | Primary sensor fields fixed; MQTT/topics remain review targets |
| P1 | Pulse-counter rollover / restart classification | PARTIAL | Helpers added; HIL still required |

## Unconfirmed / hardware-dependent

| ID | Finding | Status |
|----|---------|--------|
| H1 | RTU HAT auto-RTS behaviour under failed TCP create | UNCONFIRMED |
| H2 | Dual ADS1115 ASB false-positive rate after hardened detect | UNCONFIRMED |
| H3 | Long-run NTP step-back during sensor poll | UNCONFIRMED |

See also `patches/`, `project/issues.csv`, and `docs/HARDWARE_TEST_PLAN.md`.
