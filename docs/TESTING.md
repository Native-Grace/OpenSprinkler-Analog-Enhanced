# Testing

## Host unit tests (no hardware)

```bash
make -C tests/unit test
```

Coverage includes:

- divider zero rejection
- minimum read interval
- invalid port / Modbus address / ADS channel
- duplicate nr bookkeeping
- transactional replacement preserving old sensor on failed init
- timestamp rollover comparisons
- retry delay floor
- exactly-one-hour elapsed
- string NUL termination
- pulse counter wrap delta helper
- runtime JSON key classification

## Static analysis

Optional local checks:

```bash
cppcheck --enable=warning,style --std=c++14 \
  sensor_validation.cpp sensors.cpp SensorBase.hpp 2>&1 | head -100
```

CI runs unit tests on every push/PR; PlatformIO target builds are best-effort / experimental.

## Hardware-in-the-loop

See [HARDWARE_TEST_PLAN.md](HARDWARE_TEST_PLAN.md) for pass/fail steps that cannot be automated without ASB / RS485 / ADS1115 hardware.
