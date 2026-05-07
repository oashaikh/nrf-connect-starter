#include "blinker.h"

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(blinker, CONFIG_LOG_DEFAULT_LEVEL);

#define LED0_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Board missing led0 alias in devicetree — add a boards/<board>.overlay"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static struct k_work_delayable work;
static atomic_t enabled = ATOMIC_INIT(1);
static bool led_state;

static void blink_handler(struct k_work *unused)
{
	ARG_UNUSED(unused);

	if (atomic_get(&enabled)) {
		led_state = !led_state;
		gpio_pin_set_dt(&led, led_state);
	}

	k_work_schedule(&work, K_MSEC(CONFIG_STARTER_BLINK_INTERVAL_MS));
}

int blinker_init(void)
{
	int ret;

	if (!gpio_is_ready_dt(&led)) {
		LOG_ERR("led0 gpio not ready");
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		LOG_ERR("failed to configure led0 (%d)", ret);
		return ret;
	}

	k_work_init_delayable(&work, blink_handler);
	k_work_schedule(&work, K_MSEC(CONFIG_STARTER_BLINK_INTERVAL_MS));

	LOG_INF("blinker started @ %d ms", CONFIG_STARTER_BLINK_INTERVAL_MS);
	return 0;
}

void blinker_set_enabled(bool enable)
{
	atomic_set(&enabled, enable ? 1 : 0);
	if (!enable) {
		gpio_pin_set_dt(&led, 0);
		led_state = false;
	}
	LOG_INF("blinker %s", enable ? "resumed" : "paused");
}

bool blinker_is_enabled(void)
{
	return atomic_get(&enabled) != 0;
}
