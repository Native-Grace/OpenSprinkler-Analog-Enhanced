/* OpenSprinkler Analog Enhanced
 * Modbus context create helper (testable classification).
 * Licensed under GPL-3.0-or-later — see LICENSE.txt.
 */

#ifndef _MODBUS_CONTEXT_H
#define _MODBUS_CONTEXT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

enum ModbusTransportKind {
  MODBUS_TRANSPORT_NONE = 0,
  MODBUS_TRANSPORT_TCP = 1,
  MODBUS_TRANSPORT_RTU = 2
};

/**
 * Classify an rs485 line from the OSPI `rs485` file.
 * Lines containing '.' or ':' are treated as TCP (IP or IP:port); else RTU device path.
 * Pure string logic — host-testable without libmodbus.
 */
static inline int modbus_classify_endpoint(const char *line) {
  if (!line || !line[0]) return MODBUS_TRANSPORT_NONE;
  for (const char *p = line; *p; ++p) {
    if (*p == '.' || *p == ':') return MODBUS_TRANSPORT_TCP;
  }
  return MODBUS_TRANSPORT_RTU;
}

#ifdef __cplusplus
}
#endif

#endif /* _MODBUS_CONTEXT_H */
