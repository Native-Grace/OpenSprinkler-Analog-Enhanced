# Hardware test plan

Baseline firmware: 2.4.0 build 221 (`3a342e8`).

## ASB / ADS1115 detection

| Step | Action | Pass | Fail |
|------|--------|------|------|
| 1 | Power controller without ASB | Logs show no ASB boards; no crash | Spurious ASB_BOARD flags |
| 2 | Attach ASB (0x48/0x49) | Both addresses pass Lo_thresh scratch; board flag set | ACK-only false positive |
| 3 | After detection, read Lo_thresh | Matches pre-probe value (restored) | Threshold left at 0x5A5A |
| 4 | Place non-ADS device on bus if available | Reported absent/unrecognized, not ADS1115 | Mis-identified |

## Modbus OSPI `rs485` file

| Step | Action | Pass | Fail |
|------|--------|------|------|
| 1 | Line `/dev/ttyUSBx` | RTU context; serial mode set; connect or clean fail | Crash on null ctx |
| 2 | Line `192.168.x.x:502` | TCP context; **no** RTU setters | RTU APIs on TCP |
| 3 | Invalid create (bad path) | Error log; other sensors still load | Abort / segfault |
| 4 | Mixed good TCP + bad RTU | Good adapter usable | One failure blocks all |

## Sensor config API

| Step | Action | Pass | Fail |
|------|--------|------|------|
| 1 | Set `div=0` | Rejected; existing sensor unchanged | Accepted / div0 crash |
| 2 | Replace type with failing init | Old sensor remains active | Gap with no sensor |
| 3 | Submit `data_ok`/`data` in config | Live readings unchanged by those keys | Runtime overwritten |
| 4 | Duplicate `nr` in sensors.json | Second skipped; log present; no leak | Overwrite / heap growth |

## Timing / retries

| Step | Action | Pass | Fail |
|------|--------|------|------|
| 1 | Force connect error on remote sensor | Next attempt ≈ max(60, ri) later | ≈ 2× ri delay |
| 2 | Run across uint32 `millis`/`now` wrap if practicable | Polling continues | Stuck / busy loop |
| 3 | Trend window of exactly 3600s | Trend becomes available | Still UNAVAILABLE |

## Pulse counter

| Step | Action | Pass | Fail |
|------|--------|------|------|
| 1 | Natural counter wrap | Volume accounts for wrap delta | Zeroed as “reset” |
| 2 | Hard reset of counter to 0 | Treated as reset; no huge phantom volume | Huge usage spike |
