/* OpenSprinkler Analog Enhanced
 * Unified sensor configuration validation (host-testable core).
 * Licensed under GPL-3.0-or-later — see LICENSE.txt.
 */

#ifndef _SENSOR_VALIDATION_H
#define _SENSOR_VALIDATION_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Minimum poll interval (seconds). Matches practical firmware floor used on errors. */
#ifndef SENSOR_MIN_READ_INTERVAL_SEC
#define SENSOR_MIN_READ_INTERVAL_SEC 5u
#endif

#define SENSOR_NAME_MAX 29u          /* sizeof(name)-1 */
#define SENSOR_UNIT_NAME_MAX 7u      /* sizeof(userdef_unit)-1 */
#define MODBUS_SLAVE_MIN 1u
#define MODBUS_SLAVE_MAX 247u
#define ADS1115_CHANNEL_MAX 7u       /* ASB uses dual chips × 4 ch → id 0..7 */

enum SensorValidationCode {
  SENSOR_VAL_OK = 0,
  SENSOR_VAL_ERR_NR = 1,
  SENSOR_VAL_ERR_TYPE = 2,
  SENSOR_VAL_ERR_NAME = 3,
  SENSOR_VAL_ERR_READ_INTERVAL = 4,
  SENSOR_VAL_ERR_DIVIDER = 5,
  SENSOR_VAL_ERR_PORT = 6,
  SENSOR_VAL_ERR_MODBUS_ADDR = 7,
  SENSOR_VAL_ERR_CHANNEL = 8,
  SENSOR_VAL_ERR_DUPLICATE = 9,
  SENSOR_VAL_ERR_IMMUTABLE = 10,
  SENSOR_VAL_ERR_INIT = 11,
  SENSOR_VAL_ERR_UNSUPPORTED = 12
};

typedef struct SensorConfigView {
  uint32_t nr;
  uint32_t type;
  uint32_t read_interval;
  int16_t divider;
  int16_t factor;
  uint32_t port;
  uint32_t id;           /* Modbus slave or hardware channel */
  uint32_t ip;           /* 0 = local / non-IP */
  uint32_t group;
  const char *name;      /* may be NULL */
  size_t name_len;       /* if name non-NULL and name_len==0, use strlen */
  int require_modbus;    /* validate id as Modbus slave */
  int require_ads_channel; /* validate id as ADS/ASB channel */
  int type_known;        /* 0 = reject as invalid type */
} SensorConfigView;

const char *sensor_validation_strerror(int code);

/**
 * Validate a configuration view. Does not clamp dangerous values.
 * Returns SENSOR_VAL_OK or an error code.
 */
int sensor_validate_config(const SensorConfigView *cfg);

/** True if key is a runtime-only JSON field that must not apply via config. */
int sensor_json_key_is_runtime_only(const char *key);

#ifdef __cplusplus
}
#endif

#endif /* _SENSOR_VALIDATION_H */
