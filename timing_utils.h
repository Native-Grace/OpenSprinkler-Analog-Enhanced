/* OpenSprinkler Analog Enhanced
 * Copyright (C) OpenSprinkler / OpenSprinklerShop contributors
 *
 * Rollover-safe timing helpers (host-testable).
 * Licensed under GPL-3.0-or-later — see LICENSE.txt.
 */

#ifndef _TIMING_UTILS_H
#define _TIMING_UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** True if elapsed time from `previous` to `now` is at least `interval` (uint32 wrap-safe). */
static inline int timing_elapsed_ge(uint32_t now, uint32_t previous, uint32_t interval) {
  return (uint32_t)(now - previous) >= interval;
}

/**
 * Schedule next attempt after a failure without double-adding the interval.
 * Store the failure time; next due when timing_elapsed_ge(now, fail_time, retry_delay).
 * Returns the deadline timestamp (fail_time) used as `previous` for comparisons —
 * callers should set last_read = now (not now+delay) and compare with retry_delay.
 */
static inline uint32_t timing_retry_delay_seconds(uint32_t configured_interval) {
  /* Match historical firmware floor of 60s on connect/timeout errors. */
  return configured_interval > 60u ? configured_interval : 60u;
}

/** True if time went substantially backwards (e.g. NTP correction). */
static inline int timing_large_backward_step(uint32_t now, uint32_t previous, uint32_t threshold) {
  /* If now is "before" previous by more than threshold using signed delta intuition:
   * (previous - now) as uint32 is small when now jumped back a little; large when
   * now advanced (forward wrap). Treat as backward when previous > now and
   * (previous - now) <= threshold. */
  if (previous <= now) return 0;
  return (uint32_t)(previous - now) <= threshold;
}

/** Pulse-counter delta with natural uint32/uint16 wrap; returns unsigned delta. */
static inline uint32_t timing_counter_delta_u32(uint32_t current, uint32_t previous) {
  return (uint32_t)(current - previous);
}

#ifdef __cplusplus
}
#endif

#endif /* _TIMING_UTILS_H */
