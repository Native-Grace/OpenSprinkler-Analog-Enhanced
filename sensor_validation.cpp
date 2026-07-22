/* OpenSprinkler Analog Enhanced
 * Unified sensor configuration validation.
 * Licensed under GPL-3.0-or-later — see LICENSE.txt.
 */

#include "sensor_validation.h"
#include <string.h>

const char *sensor_validation_strerror(int code) {
  switch (code) {
    case SENSOR_VAL_OK: return "ok";
    case SENSOR_VAL_ERR_NR: return "invalid sensor number";
    case SENSOR_VAL_ERR_TYPE: return "invalid or unsupported sensor type";
    case SENSOR_VAL_ERR_NAME: return "sensor name too long";
    case SENSOR_VAL_ERR_READ_INTERVAL: return "read interval below minimum";
    case SENSOR_VAL_ERR_DIVIDER: return "divider must not be zero";
    case SENSOR_VAL_ERR_PORT: return "network port out of range (1-65535)";
    case SENSOR_VAL_ERR_MODBUS_ADDR: return "invalid Modbus slave address";
    case SENSOR_VAL_ERR_CHANNEL: return "invalid analog channel";
    case SENSOR_VAL_ERR_DUPLICATE: return "duplicate sensor number";
    case SENSOR_VAL_ERR_IMMUTABLE: return "immutable identity field change rejected";
    case SENSOR_VAL_ERR_INIT: return "driver initialization failed";
    case SENSOR_VAL_ERR_UNSUPPORTED: return "unsupported sensor type";
    default: return "unknown validation error";
  }
}

int sensor_json_key_is_runtime_only(const char *key) {
  if (!key) return 0;
  return strcmp(key, "data_ok") == 0 ||
         strcmp(key, "last") == 0 ||
         strcmp(key, "nativedata") == 0 ||
         strcmp(key, "data") == 0 ||
         strcmp(key, "trend") == 0 ||
         strcmp(key, "last_read") == 0 ||
         strcmp(key, "repeat_read") == 0;
}

int sensor_validate_config(const SensorConfigView *cfg) {
  if (!cfg) return SENSOR_VAL_ERR_NR;

  if (cfg->nr == 0) return SENSOR_VAL_ERR_NR;
  if (cfg->type == 0 || !cfg->type_known) return SENSOR_VAL_ERR_TYPE;

  if (cfg->name) {
    size_t n = cfg->name_len ? cfg->name_len : strlen(cfg->name);
    if (n > SENSOR_NAME_MAX) return SENSOR_VAL_ERR_NAME;
  }

  if (cfg->read_interval != 0 && cfg->read_interval < SENSOR_MIN_READ_INTERVAL_SEC) {
    return SENSOR_VAL_ERR_READ_INTERVAL;
  }

  if (cfg->divider == 0) return SENSOR_VAL_ERR_DIVIDER;

  /* IP-backed sensors must have a valid TCP port. */
  if (cfg->ip != 0) {
    if (cfg->port < 1u || cfg->port > 65535u) return SENSOR_VAL_ERR_PORT;
  } else if (cfg->port != 0 && (cfg->port < 1u || cfg->port > 65535u)) {
    /* Non-IP: port may encode address variants; only reject clearly illegal TCP range if set high. */
    if (cfg->port > 65535u) return SENSOR_VAL_ERR_PORT;
  }

  if (cfg->require_modbus) {
    if (cfg->id < MODBUS_SLAVE_MIN || cfg->id > MODBUS_SLAVE_MAX) {
      return SENSOR_VAL_ERR_MODBUS_ADDR;
    }
  }

  if (cfg->require_ads_channel) {
    if (cfg->id > ADS1115_CHANNEL_MAX) return SENSOR_VAL_ERR_CHANNEL;
  }

  (void)cfg->factor;
  (void)cfg->group;
  return SENSOR_VAL_OK;
}
