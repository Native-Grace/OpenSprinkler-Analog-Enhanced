# Sensor configuration

## Validated fields

| Field | Rule |
|-------|------|
| `nr` | Non-zero; duplicates rejected on load/define |
| `type` | Must be a known sensor type id |
| `name` | Max 29 characters (NUL-terminated) |
| `ri` / read_interval | 0 (unset) or ≥ 5 seconds |
| `div` / divider | Must not be 0 |
| `port` | For IP sensors: 1..65535 |
| `id` (Modbus) | 1..247 when Modbus required |
| `id` (ADS/ASB channel) | 0..7 when channel required |
| `ip` | Non-zero implies TCP port validation |

Dangerous values are **rejected**, not silently clamped (except established retry floor of 60s on connect/timeout errors).

## API behaviour

- Partial updates (no `type`) cannot change identity (`nr` / driver type).
- Full definitions with a new `type` create the replacement first; the old sensor remains until the new one validates and `init()` succeeds.
- Same-type updates roll back key fields if validation fails; driver fields (`ip`/`port`/`id`) trigger safe re-init.

## Runtime fields in JSON

Submitting `data_ok`, `last`, `data`, `nativedata`, or `trend` in a configuration payload does not alter live state.
