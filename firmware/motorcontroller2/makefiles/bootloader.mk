
BOOTLOADER_OBJS += \
$(OUTPUT_DIR)/obj/bootloader/UARTDma.o \
$(OUTPUT_DIR)/obj/bootloader/UARTDma_componentSpecific.o \
$(OUTPUT_DIR)/obj/bootloader/bootloader/Bootloader.o \
$(OUTPUT_DIR)/obj/bootloader/bootloader/Flash_Interface.o

BOOTLOADER_C_DEPS += $(BOOTLOADER_OBJS:%.o=%.d)

BOOTLOADER_COMPILER_FLAGS= \
-O0 \
-g3 \
-Wall \
-Werror

INCLUDE_PATH+= \
-I$(SHARED_CODE_DIR) \
-I$(SHARED_CODE_DIR)/RTOS \
-I$(SHARED_CODE_DIR)/bootloader

$(shell mkdir -p $(OUTPUT_DIR)/obj/bootloader)
$(shell mkdir -p $(OUTPUT_DIR)/obj/bootloader/bootloader)
$(shell mkdir -p $(OUTPUT_DIR)/obj/bootloader/RTOS)

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/bootloader/%.o: $(CODE_DIR)/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(ARM_TOOLS_COMPILER_FLAGS)"  ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/bootloader/%.o: $(SHARED_CODE_DIR)/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(ARM_TOOLS_COMPILER_FLAGS)" ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)

	