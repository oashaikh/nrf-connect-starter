#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialise the blinker module.
 *
 * Looks up the board's `led0` alias from devicetree, configures it as an
 * output, and starts a kernel work item that toggles it on the configured
 * interval.
 *
 * @return 0 on success, negative errno on failure.
 */
int blinker_init(void);

/**
 * @brief Pause/resume the blinker.
 */
void blinker_set_enabled(bool enabled);

/**
 * @brief Whether the blinker is currently active.
 */
bool blinker_is_enabled(void);
