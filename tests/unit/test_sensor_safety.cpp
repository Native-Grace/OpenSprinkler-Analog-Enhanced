/* Host unit tests for OpenSprinkler Analog Enhanced safety helpers.
 * Build: make -C tests/unit
 * License: GPL-3.0-or-later
 */

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdint>

#include "../../sensor_validation.h"
#include "../../timing_utils.h"
#include "../../safe_string.h"
#include "../../modbus_context.h"

static int g_failed = 0;

#define EXPECT_TRUE(cond) do { \
  if (!(cond)) { \
    std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    g_failed++; \
  } \
} while (0)

#define EXPECT_EQ(a, b) do { \
  auto _a = (a); auto _b = (b); \
  if (_a != _b) { \
    std::printf("FAIL %s:%d: %s == %s (%ld vs %ld)\n", __FILE__, __LINE__, #a, #b, (long)_a, (long)_b); \
    g_failed++; \
  } \
} while (0)

static SensorConfigView base_cfg() {
  SensorConfigView c = {};
  c.nr = 1;
  c.type = 10;
  c.read_interval = 60;
  c.divider = 1;
  c.factor = 1;
  c.port = 0;
  c.id = 0;
  c.ip = 0;
  c.group = 0;
  c.name = "soil";
  c.type_known = 1;
  return c;
}

static void test_validation() {
  SensorConfigView c = base_cfg();
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_OK);

  c.divider = 0;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_DIVIDER);

  c = base_cfg();
  c.read_interval = 1;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_READ_INTERVAL);

  c = base_cfg();
  c.ip = 0x7f000001;
  c.port = 0;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_PORT);

  c.port = 70000;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_PORT);

  c = base_cfg();
  c.require_modbus = 1;
  c.id = 0;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_MODBUS_ADDR);
  c.id = 248;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_MODBUS_ADDR);
  c.id = 1;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_OK);

  c = base_cfg();
  c.require_ads_channel = 1;
  c.id = 8;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_CHANNEL);
  c.id = 7;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_OK);

  c = base_cfg();
  c.type_known = 0;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_TYPE);

  c = base_cfg();
  c.nr = 0;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_NR);

  char longname[64];
  memset(longname, 'A', sizeof(longname));
  longname[63] = '\0';
  c = base_cfg();
  c.name = longname;
  EXPECT_EQ(sensor_validate_config(&c), SENSOR_VAL_ERR_NAME);
}

static void test_runtime_keys() {
  EXPECT_TRUE(sensor_json_key_is_runtime_only("data_ok"));
  EXPECT_TRUE(sensor_json_key_is_runtime_only("last"));
  EXPECT_TRUE(sensor_json_key_is_runtime_only("nativedata"));
  EXPECT_TRUE(sensor_json_key_is_runtime_only("data"));
  EXPECT_TRUE(sensor_json_key_is_runtime_only("trend"));
  EXPECT_TRUE(!sensor_json_key_is_runtime_only("ri"));
  EXPECT_TRUE(!sensor_json_key_is_runtime_only("div"));
}

static void test_timing() {
  EXPECT_TRUE(timing_elapsed_ge(100, 50, 50));
  EXPECT_TRUE(!timing_elapsed_ge(99, 50, 50));
  // Rollover: previous near end, now wrapped
  EXPECT_TRUE(timing_elapsed_ge(10u, 0xFFFFFFF0u, 20u));
  EXPECT_TRUE(!timing_elapsed_ge(5u, 0xFFFFFFF0u, 30u));

  EXPECT_EQ(timing_retry_delay_seconds(300), 300u);
  EXPECT_EQ(timing_retry_delay_seconds(10), 60u);

  // Exactly one hour span
  EXPECT_TRUE(timing_elapsed_ge(3600, 0, 3600));

  EXPECT_EQ(timing_counter_delta_u32(5, 0xFFFFFFFDu), 8u);
}

static void test_strings() {
  char buf[8];
  memset(buf, 'X', sizeof(buf));
  safe_strncpy(buf, sizeof(buf), "hello-world-extra");
  EXPECT_EQ((int)strlen(buf), 7);
  EXPECT_EQ(buf[7], '\0');
  EXPECT_TRUE(strcmp(buf, "hello-w") == 0);

  safe_strncpy(buf, sizeof(buf), NULL);
  EXPECT_EQ(buf[0], '\0');
}

static void test_modbus_classify() {
  EXPECT_EQ(modbus_classify_endpoint("/dev/ttyUSB0"), MODBUS_TRANSPORT_RTU);
  EXPECT_EQ(modbus_classify_endpoint("192.168.1.10"), MODBUS_TRANSPORT_TCP);
  EXPECT_EQ(modbus_classify_endpoint("192.168.1.10:502"), MODBUS_TRANSPORT_TCP);
  EXPECT_EQ(modbus_classify_endpoint(""), MODBUS_TRANSPORT_NONE);
}

/* Lightweight stand-in for transactional replacement bookkeeping. */
struct FakeSensor {
  int id;
  int alive;
};

static FakeSensor *g_map[4];

static int transactional_replace(int slot, FakeSensor *replacement, int init_ok) {
  if (!replacement) return -1;
  if (!init_ok) {
    delete replacement;
    return -1; // keep old
  }
  FakeSensor *old = g_map[slot];
  g_map[slot] = replacement;
  delete old;
  return 0;
}

static void test_transactional() {
  g_map[0] = new FakeSensor{1, 1};
  FakeSensor *old = g_map[0];
  FakeSensor *bad = new FakeSensor{2, 1};
  EXPECT_EQ(transactional_replace(0, bad, 0), -1);
  EXPECT_TRUE(g_map[0] == old);

  FakeSensor *good = new FakeSensor{3, 1};
  EXPECT_EQ(transactional_replace(0, good, 1), 0);
  EXPECT_TRUE(g_map[0] == good);
  EXPECT_EQ(g_map[0]->id, 3);
  delete g_map[0];
  g_map[0] = NULL;
}

static void test_duplicate_detection() {
  // Simulated load map: reject second insert of same nr
  int occupied[8] = {0};
  auto try_insert = [&](int nr) -> int {
    if (nr <= 0) return SENSOR_VAL_ERR_NR;
    if (occupied[nr]) return SENSOR_VAL_ERR_DUPLICATE;
    occupied[nr] = 1;
    return SENSOR_VAL_OK;
  };
  EXPECT_EQ(try_insert(1), SENSOR_VAL_OK);
  EXPECT_EQ(try_insert(1), SENSOR_VAL_ERR_DUPLICATE);
}

int main() {
  test_validation();
  test_runtime_keys();
  test_timing();
  test_strings();
  test_modbus_classify();
  test_transactional();
  test_duplicate_detection();

  if (g_failed) {
    std::printf("%d test(s) failed\n", g_failed);
    return 1;
  }
  std::printf("All host unit tests passed\n");
  return 0;
}
