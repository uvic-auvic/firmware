# AUVIC Firmware
Repo for all AUVIC firmware

## Getting started

This branch contains submodule(s). You will have to download the submodule after you clone.
Run the following commands in your command line
1) `git clone https://github.com/uvic-auvic/firmware.git` to clone the repo. **IMPORTANT**: Clone it to a location without any spaces in the file path.
2) `cd firmware`
3) `git status` to ensure the repo cloned correctly
4) `git submodule init` to download the submodule(s)
5) `git submodule update`. Anytime you switch branches (i.e. by doing `git checkout some_branch`) you have to run `git submodule update`

### Ubuntu 16.04 or 18.04
1) Checkout repo into a path without any spaces
2) Run `sudo ./provision.sh`. This will install all required dependencies.
3) In the repo root directory, run `make -C firmware all -j4`. This will build firmware for all components. Ensure no failuers occur.
4) Run `make -C firmware test`. This will run all unit tests. Ensure no failues occur.
5) To build a specific component, run `make -C firmware <component_name> TARGET=<app_or_bootloader>`. Example: `make -C firmware motorcontroller2 TARGET=app -j4`.
6) Run `make -C firmware help` to display a list of components and options.

Note: The `-C firmware` argument is required since the main makefile is in a subfolder from the repo root. You can alternatively cd into firmware and just run `make all -j4`

### Windows and MacOS (No unit tests)
1) Checkout repo into a path without any spaces
2) Download and install System Workbench from [here](https://www.openstm32.org/Downloading%2Bthe%2BSystem%2BWorkbench%2Bfor%2BSTM32%2Binstaller).
3) Open System Workbench. Click `File->Import`.
4) Click `Browse` and select the firmware subfolder.
5) Uncheck the `Search for nested projects` checkbox. Select the project available and click `Finish`.
6) Find the Hammer icon(build button) in the toolbar and select `all`. Ensure no failures occur.

## Builds

![auvic-firmware-build](https://github.com/uvic-auvic/firmware/workflows/auvic-firmware-build/badge.svg)

## Documentation
Checkout the [wiki](https://github.com/uvic-auvic/firmware/wiki) for detailed information about the firmware.

