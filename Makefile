.DEFAULT_GOAL := help

BOARD ?= nrf52840dk/nrf52840
APP   := app

.PHONY: help init update build clean flash menuconfig sim test

help:  ## Show this help
	@grep -E '^[a-zA-Z_-]+:.*?##' $(MAKEFILE_LIST) | awk 'BEGIN{FS=":.*?## "}; {printf "  %-12s %s\n", $$1, $$2}'

init:  ## One-time: initialise the west workspace from this manifest
	west init -l .
	west update

update:  ## Pull latest from the manifest revisions
	west update

build:  ## Build for $(BOARD)
	west build -b $(BOARD) $(APP) -p auto

clean:  ## Wipe build directory
	rm -rf build

flash:  ## Flash the latest build (requires hardware)
	west flash

menuconfig:  ## Tweak Kconfig interactively
	west build -t menuconfig

sim:  ## Build and run on native_sim
	west build -b native_sim $(APP) -p auto && ./build/zephyr/zephyr.exe

test:  ## Run twister test suite
	west twister --inline-logs --integration -p $(BOARD)
