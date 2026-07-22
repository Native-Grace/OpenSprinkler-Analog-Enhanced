# Roadmap — OpenSprinkler Analog Enhanced

## Phase 0 — Repository baseline (done)

- [x] Import OpenSprinklerShop firmware
- [x] Record upstream commit / FW build
- [x] Preserve GPL-3.0 notices
- [x] Establish `main` / `develop` + fix branches

## Phase 1 — Safety-critical fixes (current)

1. Modbus context validation
2. Unified sensor configuration validation
3. Config vs runtime JSON separation
4. Transactional sensor replacement
5. String / buffer termination safety
6. Rollover-safe timing and retry scheduling
7. ADS1115 / ASB detection hardening
8. Host unit tests + CI hooks

## Phase 2 — Coverage and hardening

- Expand pulse-counter restart/reset classification
- MQTT topic / credential log scrubbing audit
- Broader static analysis in CI
- Optional ESP8266 / ESP32 PlatformIO builds in CI (experimental if toolchain unavailable)

## Phase 3 — Upstream sync

- Document rebase procedure against OpenSprinklerShop
- Cherry-pick or PR candidate patches upstream where appropriate

## Non-goals

- Unrelated UI / scheduler rewrites
- Changing normal watering behaviour without documented need
- Adding large third-party dependencies
