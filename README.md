# AUVIC Firmware
Repo for all AUVIC firmware

### Getting started:

Ubuntu 16.04
1) Run `sudo provision.sh`. This will install all required dependencies.
2) In the repo root directory, run `make all -j4`. This will build firmware for all components. Ensure no failuers occur.
3) Run `make test`. This will run all unit tests. Ensure no failues occur.
4) To build a specific component, run `make <component_name> TARGET=<app_or_bootloader>`. Example: `make motorcontroller2 TARGET=app -j4`.
5) Run `make help` to display a list of components and options.

