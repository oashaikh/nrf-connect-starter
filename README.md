# nrf-connect-starter

A drop-in **nRF Connect SDK / Zephyr** application template — west
manifest, CMake project, Kconfig fragments, board overlays, and a CI
workflow that builds for multiple boards on every push.

## What this repo does

- Boots a minimal Zephyr app on Nordic NRF52/NRF53 targets (nRF52840 DK,
  nRF52833 DK, nRF5340 DK out of the box).
- Pins NCS to a specific revision via `west.yml` so builds are
  reproducible — no "works on my machine" toolchain drift.
- Splits hardware-portable code (`src/blinker.c`) from board-specific
  Kconfig and devicetree overlays (`boards/`).
- Includes a `native_sim` build so PRs can verify the source compiles and
  runs through Zephyr's emulator without dedicated hardware.
- Twister harness (`sample.yaml`) so the same CI matrix can run smoke
  checks on the simulated build.

## Project layout

- `west.yml` - manifest pinning NCS + Zephyr at a specific revision.
- `app/` - the application
  - `CMakeLists.txt` - zephyr-style CMake.
  - `prj.conf` - Kconfig defaults.
  - `Kconfig` - app-level Kconfig (e.g. blink interval).
  - `src/` - source files.
  - `include/` - private headers.
  - `boards/` - per-board Kconfig fragments and devicetree overlays.
  - `sample.yaml` - Twister test description.

## Quick start

You need the nRF Connect toolchain installed. The shortest path:

1. Install [nRF Connect for Desktop] and the **Toolchain Manager**.
2. From Toolchain Manager, install **NCS v2.7.0** (matching `west.yml`).
3. Open the bundled "command-line shell" so `west`/`cmake`/`arm-none-eabi-gcc`
   are on PATH.

Then:

```bash
mkdir my-workspace && cd my-workspace
git clone <this-repo> nrf-connect-starter
cd nrf-connect-starter
west init -l .
west update             # pulls Zephyr, NCS, HALs, MCUboot ...

# Build for an NRF52840 DK
west build -b nrf52840dk/nrf52840 app

# Flash via the on-board J-Link
west flash
```

Or in the simulator (no hardware required):

```bash
west build -b native_sim app
./build/zephyr/zephyr.exe
```

## Common commands

| Command | Description |
|---|---|
| `make init` | First-time `west init -l . && west update`. |
| `make build BOARD=nrf52840dk/nrf52840` | Build for a board. |
| `make sim` | Build for native_sim and run. |
| `make flash` | Flash the latest build. |
| `make menuconfig` | Tweak Kconfig interactively. |
| `make test` | Run the Twister suite. |

## Adding a board

1. Create `app/boards/<board>.overlay` (devicetree overrides) and
   `app/boards/<board>.conf` (Kconfig overrides) if needed. They get
   picked up automatically by Zephyr's board-specific overlay search.
2. Add the board to the matrix in `.github/workflows/build.yml` and the
   `integration_platforms` list in `app/sample.yaml`.

## Adding a feature

1. Drop new sources in `app/src/` and add them to `app/CMakeLists.txt`.
2. New Kconfig options go in `app/Kconfig` so they show up in
   `menuconfig` and can be tweaked per-board.
3. New devicetree nodes go in `app/dts/bindings/` (custom bindings) and
   per-board overlays.

## Why pin NCS

Nordic ships breaking changes between NCS versions (Zephyr-style API
churn). Pinning the manifest at `v2.7.0` means every developer and CI run
gets the same toolchain, the same DT bindings, the same Kconfig surface.
Bump it deliberately, not by accident.

[nRF Connect for Desktop]: https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-Desktop
