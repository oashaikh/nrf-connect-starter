cmake_minimum_required(VERSION 3.20.0)

# Allow per-board overlays in app/boards/<board>.overlay
list(APPEND DTS_EXTRA_OVERLAYS ${CMAKE_CURRENT_SOURCE_DIR}/boards)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(nrf_connect_starter LANGUAGES C)

target_sources(app PRIVATE
    src/main.c
    src/blinker.c
)

target_include_directories(app PRIVATE include)
