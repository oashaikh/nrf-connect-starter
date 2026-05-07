#include "blinker.h"

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

int main(void)
{
	int ret;

	LOG_INF("nrf-connect-starter booting on %s", CONFIG_BOARD);

	ret = blinker_init();
	if (ret < 0) {
		LOG_ERR("blinker init failed: %d", ret);
		return ret;
	}

	while (1) {
		k_sleep(K_FOREVER);
	}

	return 0;
}
