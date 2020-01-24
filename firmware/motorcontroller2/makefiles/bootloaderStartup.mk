
BOOTLOADER_OBJS += \
$(OUTPUT_DIR)/obj/bootloader/startup/system_stm32f4xx.o \
$(OUTPUT_DIR)/obj/bootloader/startup/startup_stm32f411xe.o \
$(OUTPUT_DIR)/obj/bootloader/startup/main.o

BOOTLOADER_C_DEPS += $(BOOTLOADER_OBJS:%.o=%.d)

INCLUDE_PATH+= \
-I$(SHARED_CODE_DIR)/bootloader

$(shell mkdir -p $(OUTPUT_DIR)/obj/bootloader/startup)

ARM_TOOLS_COMPILER_FLAGS := \
-O0 \
-g3 \
-Wall

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/bootloader/startup/%.o: $(CODE_DIR)/startup/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/bootloader/startup/startup_stm32f411xe.o: $(CODE_DIR)/startup/startup_stm32f411xe.s
	@$(MAKE) --no-print-directory arm-assembler ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@
