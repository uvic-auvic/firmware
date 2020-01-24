
REPO_ROOT=../..
COMPONENT_DIR=$(REPO_ROOT)/motorcontroller2
COMPONENT_SHARED_CODE_DIR=$(COMPONENT_DIR)/code/shared
SHARED_DIR=$(REPO_ROOT)/shared
SHARED_CODE_DIR=$(REPO_ROOT)/shared/code
CODE_DIR=$(COMPONENT_DIR)/code/$(TARGET)
ARM_TOOLS=$(REPO_ROOT)/shared/makefiles/armTools.mk

ifeq ($(TARGET),clean)
	OUTPUT_DIR=$(COMPONENT_DIR)/MOTORCONTROLLER
else
	OUTPUT_DIR=$(COMPONENT_DIR)/MOTORCONTROLLER/$(TARGET)
endif

DEVICE_DEFINES+= \
-DSTM32 \
-DSTM32F4 \
-DSTM32F413_423xx

OTHER_DEFINES+= \
-DUSE_STDPERIPH_DRIVER

INCLUDE_PATH+= \
-I$(COMPONENT_SHARED_CODE_DIR)/config \
-I$(CODE_DIR) \
-I$(CODE_DIR)/RTOS

-include STMLib.mk
-include FreeRTOSlib.mk
#-include protobuf.mk
-include app.mk
-include appStartup.mk
-include bootloader.mk
-include bootloaderStartup.mk

ARM_TOOLS_COMPILER_MCU_SETTINGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARM_TOOLS_COMPILER_DEFINES := $(DEVICE_DEFINES) $(OTHER_DEFINES)
ARM_TOOLS_COMPILER_INCLUDE_PATH := $(INCLUDE_PATH)

-include $(ARM_TOOLS)

LINKER_SCRIPT= $(CODE_DIR)/STM32F413ZHTx_FLASH.ld
TARGET_ELF=$(OUTPUT_DIR)/motorcontroller_$(TARGET).elf
TARGET_BIN=$(OUTPUT_DIR)/motorcontroller_$(TARGET).bin
TARGET_HEX=$(OUTPUT_DIR)/motorcontroller_$(TARGET).hex
TARGET_MAP=$(OUTPUT_DIR)/motorcontroller_$(TARGET).map

APP_LINKER_OPTIONS:=\
-Xlinker -nmagic

BOOTLOADER_LINKER_OPTIONS:=

ARM_TOOL_LINKER_LIBRARIES := \
-lm

ARM_TOOLS_LINKER_SCRIPT := $(LINKER_SCRIPT)
ARM_TOOLS_LINKER_TARGET_MAP := $(TARGET_MAP)
ARM_TOOLS_LINKER_TARGET_ELF := $(TARGET_ELF)

app: $(LIB_OBJS) $(PROTO_OBJS) $(APP_OBJS) $(LINKER_SCRIPT)
	@$(MAKE) arm-linker ARM_TOOLS_LINKER_OBJECT_FILES="$(LIB_OBJS) $(PROTO_OBJS) $(APP_OBJS)" ARM_TOOL_LINKER_OPTIONS="$(APP_LINKER_OPTIONS)" ARM_TOOL_LINKER_LIBRARIES="$(ARM_TOOL_LINKER_LIBRARIES)"
	@$(MAKE) --no-print-directory post-build
	
bootloader: $(LIB_OBJS) $(PROTO_OBJS) $(BOOTLOADER_OBJS) $(LINKER_SCRIPT)
	@$(MAKE) arm-linker ARM_TOOLS_LINKER_OBJECT_FILES="$(LIB_OBJS) $(PROTO_OBJS) $(BOOTLOADER_OBJS)" ARM_TOOL_LINKER_OPTIONS="$(BOOTLOADER_LINKER_OPTIONS)" ARM_TOOL_LINKER_LIBRARIES="$(ARM_TOOL_LINKER_LIBRARIES)"
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
-include $(PROTO_C_DEPS)
-include $(APP_C_DEPS)
-include $(BOOTLOADER_C_DEPS)
