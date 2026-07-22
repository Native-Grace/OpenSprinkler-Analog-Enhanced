# OpenSprinkler Analog Enhanced

Reliability and safety hardening fork of the [OpenSprinklerShop Analog Sensor firmware](https://github.com/opensprinklershop/OpenSprinkler-Firmware).

## Upstream baseline

| Field | Value |
|-------|-------|
| Upstream | https://github.com/opensprinklershop/OpenSprinkler-Firmware |
| Commit | `3a342e8f614027c20586bf2fba239ee6f170db8f` |
| Firmware | **2.4.0** build **221** |
| License | GPL-3.0-or-later |

See [docs/UPSTREAM.md](docs/UPSTREAM.md).

## What this fork adds

- Modbus context null-checks and RTU/TCP separation
- Unified sensor configuration validation
- Config vs runtime JSON separation
- Transactional sensor replacement
- Rollover-safe timing / retry scheduling
- Hardened ADS1115 detection (no ACK-only identification; register restore)
- Host unit tests for validation and timing helpers

Normal irrigation behaviour is intentionally unchanged except where unsafe defaults are rejected.

## Documentation

| Doc | Purpose |
|-----|---------|
| [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) | Sensor / Modbus architecture notes |
| [docs/REVIEW.md](docs/REVIEW.md) | Confirmed findings and fix status |
| [docs/ROADMAP.md](docs/ROADMAP.md) | Phased work plan |
| [docs/SENSOR_CONFIGURATION.md](docs/SENSOR_CONFIGURATION.md) | Config validation rules |
| [docs/BUILDING.md](docs/BUILDING.md) | Build targets |
| [docs/TESTING.md](docs/TESTING.md) | Unit + HIL tests |
| [docs/HARDWARE_TEST_PLAN.md](docs/HARDWARE_TEST_PLAN.md) | Hardware pass/fail steps |

Upstream MkDocs live under `docs/` as shipped by OpenSprinklerShop.

## Quick start

```bash
git submodule update --init --recursive
make -C tests/unit test
# Demo / sanity native compile (PlatformIO):
#   pio run -e demo
# OSPI / Linux (requires libmodbus and friends):
#   pio run -e linux
```

## License

Copyright notices from OpenSprinkler / OpenSprinklerShop are preserved. This project remains under the GNU GPL v3 (or later). See `LICENSE.txt`.
