# AUVIC Firmware
![auvic-firmware-build](https://github.com/uvic-auvic/firmware/workflows/auvic-firmware-build/badge.svg)

Repo for all AUVIC firmware

## Getting started
Simply download your favorite IDE that runs GCC to get this running. Import a project into your workspace and exclude the following from the build (right click on the file/folder):
-   Drivers->FreeRTOS->Source->Portable->MemMang->heap_"1,2,3,5"
-   Drivers->FreeRTOS->Source->Portable->GCC->(All but "ARM_CM4F")

This step is required because the compiler will try to build all architecture types - we dont want that. Last step is to include folders into your build path. It should look something like this...
![IncludePathImg](./STM32F413/Include%20Paths%20Instructions.png)

## Documentation
Checkout the [wiki](https://github.com/uvic-auvic/firmware/wiki) for detailed information about the firmware.


