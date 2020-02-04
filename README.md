# AUVIC Firmware
Repo for all AUVIC firmware

## Getting started

### Ubuntu 16.04
1) Run `sudo provision.sh`. This will install all required dependencies.
2) In the repo root directory, run `make -C firmware all -j4`. This will build firmware for all components. Ensure no failuers occur.
3) Run `make test`. This will run all unit tests. Ensure no failues occur.
4) To build a specific component, run `make -C firmware <component_name> TARGET=<app_or_bootloader>`. Example: `make -C firmware motorcontroller2 TARGET=app -j4`.
5) Run `make -C firmware help` to display a list of components and options.

Note: The `-C firmware` argument is required since the main makefile is in a subfolder from the repo root. You can alternatively cd into firmware and just run `make all -j4`

### Ubuntu 18.04
Not supported yet

### Windows and MacOS (No unit tests)
1) Download and install System Workbench from [here](https://www.openstm32.org/Downloading%2Bthe%2BSystem%2BWorkbench%2Bfor%2BSTM32%2Binstaller).
2) Open System Workbench. Click `File->Import`.
3) Click `Browse` and select the firmware subfolder.
4) Uncheck the `Search for nested projects` checkbox. Select the project available and click `Finish`.
5) Find the Hammer icon(build button) in the toolbar and select `all`. Ensure no failures occur.

## Builds

![auvic-firmware-build](https://github.com/uvic-auvic/firmware/workflows/auvic-firmware-build/badge.svg)

