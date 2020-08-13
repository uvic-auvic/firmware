COMPONENT_DIR=$(REPO_ROOT)/powerboard2
COMPONENT_SHARED_CODE_DIR=$(COMPONENT_DIR)/code/shared
SHARED_DIR=$(REPO_ROOT)/shared
SHARED_CODE_DIR=$(REPO_ROOT)/shared/code
CODE_DIR=$(COMPONENT_DIR)/code/$(TARGET)
SHARED_MAKEFILES=$(REPO_ROOT)/shared/makefiles

LINKER_SCRIPT= $(CODE_DIR)/startup/STM32F413VGTX_FLASH.ld

DEVICE=STM32F413
MCU_ARCHITECTURE=M4
DEVICE_DEFINES+= \
-DSTM32 \
-DSTM32F4 \
-DSTM32F413_423xx \
-DDEVICE_STM32F413=1 \
-DMCU_ARCHITECTURE_M4=1 \
-DDEVICE_FAMILY_STM32F4=1 \

ifeq ($(TARGET),clean)
        OUTPUT_DIR=$(COMPONENT_DIR)/POWERBOARD2
else
        OUTPUT_DIR=$(COMPONENT_DIR)/POWERBOARD2/$(TARGET)/$(DEVICE)
endif

OTHER_DEFINES+= \
-DUSE_STDPERIPH_DRIVER \
-DUART_DEBUG

INCLUDE_PATH+= \
-I$(COMPONENT_SHARED_CODE_DIR)/config \
-I$(CODE_DIR) \
-I$(CODE_DIR)/RTOS

COMPILER_OPTIMIZATION=-O3

-include $(SHARED_MAKEFILES)/FreeRTOSlib.mk
-include $(SHARED_MAKEFILES)/ISOTPLib.mk
-include STMLib.mk
-include app.mk
-include appStartup.mk
# -include bootloader.mk
# -include bootloaderStartup.mk

ARM_TOOLS_COMPILER_MCU_SETTINGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARM_TOOLS_COMPILER_DEFINES := $(DEVICE_DEFINES) $(OTHER_DEFINES)
ARM_TOOLS_COMPILER_INCLUDE_PATH := $(INCLUDE_PATH)

-include $(SHARED_MAKEFILES)/armTools.mk

TARGET_ELF=$(OUTPUT_DIR)/powerboard2_$(TARGET).elf
TARGET_BIN=$(OUTPUT_DIR)/powerboard2_$(TARGET).bin
TARGET_HEX=$(OUTPUT_DIR)/powerboard2_$(TARGET).hex
TARGET_MAP=$(OUTPUT_DIR)/powerboard2_$(TARGET).map

APP_LINKER_OPTIONS:=\
-Xlinker -nmagic

BOOTLOADER_LINKER_OPTIONS:=

ARM_TOOL_LINKER_LIBRARIES := \
-lm \
-lc \
-lgcc \
--specs=rdimon.specs
# -lgcc and --specs=rdimon.specs is required for `vsnprintf`

ARM_TOOLS_LINKER_SCRIPT := $(LINKER_SCRIPT)
ARM_TOOLS_LINKER_TARGET_MAP := $(TARGET_MAP)
ARM_TOOLS_LINKER_TARGET_ELF := $(TARGET_ELF)

app: $(LIB_OBJS) $(APP_OBJS) $(LINKER_SCRIPT)
	@$(MAKE) arm-linker ARM_TOOLS_LINKER_OBJECT_FILES="$(LIB_OBJS) $(APP_OBJS)" ARM_TOOL_LINKER_OPTIONS="$(APP_LINKER_OPTIONS)" ARM_TOOL_LINKER_LIBRARIES="$(ARM_TOOL_LINKER_LIBRARIES)"
	@$(MAKE) --no-print-directory post-build

bootloader: $(LIB_OBJS) $(BOOTLOADER_OBJS) $(LINKER_SCRIPT)
	@$(MAKE) arm-linker ARM_TOOLS_LINKER_OBJECT_FILES="$(LIB_OBJS) $(BOOTLOADER_OBJS)" ARM_TOOL_LINKER_OPTIONS="$(BOOTLOADER_LINKER_OPTIONS)" ARM_TOOL_LINKER_LIBRARIES="$(ARM_TOOL_LINKER_LIBRARIES)"
	@$(MAKE) --no-print-directory post-build

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

-include $(LIB_C_DEPS)
-include $(APP_C_DEPS)
-include $(BOOTLOADER_C_DEPS)
