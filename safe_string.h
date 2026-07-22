/* OpenSprinkler Analog Enhanced
 * Safe fixed-buffer string helpers (host-testable).
 * Licensed under GPL-3.0-or-later — see LICENSE.txt.
 */

#ifndef _SAFE_STRING_H
#define _SAFE_STRING_H

#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Copy at most dest_size-1 bytes and always NUL-terminate.
 * No-op if dest is NULL or dest_size == 0.
 */
static inline void safe_strncpy(char *dest, size_t dest_size, const char *src) {
  if (!dest || dest_size == 0) return;
  if (!src) {
    dest[0] = '\0';
    return;
  }
  size_t i = 0;
  for (; i + 1 < dest_size && src[i] != '\0'; i++) {
    dest[i] = src[i];
  }
  dest[i] = '\0';
}

#ifdef __cplusplus
}
#endif

#endif /* _SAFE_STRING_H */
