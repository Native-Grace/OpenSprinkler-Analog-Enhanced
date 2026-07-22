# Building

## Targets

| Target | How | Status in this environment |
|--------|-----|----------------------------|
| Host unit tests | `make -C tests/unit test` | Supported |
| Demo native | `pio run -e demo` | Optional / experimental if PlatformIO present |
| Linux / OSPI | `pio run -e linux` or `make` | Requires Linux deps (`libmodbus`, mosquitto, …) |
| ESP8266 (`os3x_esp8266`) | `pio run -e os3x_esp8266` | Experimental in CI if toolchain available |
| ESP32-C5 Zigbee / Matter | `pio run -e esp32-c5-zigbee` / `esp32-c5-matter` | Needs Espressif packages; often builder-specific |

Do not claim a target is supported unless a build has succeeded on the machine/CI runner in use.

## Submodules

```bash
git submodule update --init --recursive
```

Note: upstream’s `external/influxdb-cpp` pin may fail to fetch; update the submodule to a reachable commit if needed.

## Flags of interest

- `OSPI` — Raspberry Pi / Linux native
- `ADS1115` / `PCF8591` — OSPI analog front-ends
- `ESP8266` / `ESP32` / `ESP32C5` — MCU builds
