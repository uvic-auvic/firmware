
REPO_ROOT=/root/firmware
COMPONENT_DIR=$(REPO_ROOT)/motorcontroller2
COMPONENT_SHARED_CODE_DIR=$(COMPONENT_DIR)/code/shared
SHARED_CODE_DIR=$(REPO_ROOT)/shared/code
CODE_DIR=$(COMPONENT_DIR)/code/$(ARG1)

ifeq ($(ARG1),clean)
	OUTPUT_DIR=$(COMPONENT_DIR)/MOTORCONTROLLER
else
	OUTPUT_DIR=$(COMPONENT_DIR)/MOTORCONTROLLER/$(ARG1)
endif

DEVICE_DEFINES+= \
-DSTM32 \
-DSTM32F4 \
-DSTM32F411VETx \
-DSTM32F411E_DISCO \
-DSTM32F411xE

OTHER_DEFINES+= \
-DUSE_STDPERIPH_DRIVER

INCLUDE_PATH+= \
-I$(COMPONENT_SHARED_CODE_DIR)/config \
-I$(CODE_DIR) \
-I$(CODE_DIR)/RTOS

-include STMLib.mk
-include FreeRTOSlib.mk
-include app.mk
-include appStartup.mk
-include bootloader.mk
-include bootloaderStartup.mk

LINKER_SCRIPT= $(CODE_DIR)/STM32F411RETx_FLASH.ld
TARGET_ELF=$(OUTPUT_DIR)/motorcontroller_$(ARG1).elf
TARGET_BIN=$(OUTPUT_DIR)/motorcontroller_$(ARG1).bin
TARGET_HEX=$(OUTPUT_DIR)/motorcontroller_$(ARG1).hex
TARGET_MAP=$(OUTPUT_DIR)/motorcontroller_$(ARG1).map

APP_LINKER_FLAGS+= \
--nmagic

app: $(LIB_OBJS) $(APP_OBJS) $(LINKER_SCRIPT)
	@echo Building Motor Controller App
	@echo 'Building $(TARGET_ELF): $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T"$(LINKER_SCRIPT)" -Wl,-Map=$(TARGET_MAP) -Wl,--gc-sections -o "$(TARGET_ELF)" $(LIB_OBJS) $(APP_OBJS) -lm -Xlinker $(APP_LINKER_FLAGS)
	@echo 'Finished building $(TARGET_ELF): $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

BOOTLOADER_LINKER_FLAGS+= \


bootloader: $(LIB_OBJS) $(BOOTLOADER_OBJS) $(LINKER_SCRIPT)
	@echo Building Motor Controller Bootloader
	@echo 'Building $(TARGET_ELF): $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T"$(LINKER_SCRIPT)" -Wl,-Map=$(TARGET_MAP) -Wl,--gc-sections -o "$(TARGET_ELF)" $(LIB_OBJS) $(BOOTLOADER_OBJS) -lm $(BOOTLOADER_LINKER_FLAGS)
	@echo 'Finished building $(TARGET_ELF): $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

all: app bootloader

post-build: $(TARGET_ELF)
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "$(TARGET_ELF)" "$(TARGET_BIN)"
	arm-none-eabi-objcopy -O ihex "$(TARGET_ELF)" "$(TARGET_HEX)"
	arm-none-eabi-size "$(TARGET_ELF)"
	-@echo ' '

clean:
	rm -rf $(OUTPUT_DIR)

.PHONY: all clean dependents
.SECONDARY: post-build