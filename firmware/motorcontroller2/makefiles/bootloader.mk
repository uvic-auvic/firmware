
BOOTLOADER_OBJS += \
$(OUTPUT_DIR)/obj/bootloader/UART.o \
$(OUTPUT_DIR)/obj/bootloader/UART_componentSpecific.o \
$(OUTPUT_DIR)/obj/bootloader/bootloader/Bootloader.o \
$(OUTPUT_DIR)/obj/bootloader/bootloader/Flash_Interface.o

BOOTLOADER_C_DEFS += \
$(OUTPUT_DIR)/obj/bootloader/UART.d \
$(OUTPUT_DIR)/obj/bootloader/UART_componentSpecific.d \
$(OUTPUT_DIR)/obj/bootloader/bootloader/Bootloader.d \
$(OUTPUT_DIR)/obj/bootloader/bootloader/Flash_Interface.d

COMPILE_FLAGS= \
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
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEVICE_DEFINES) $(OTHER_DEFINES) $(INCLUDE_PATH) $(COMPILE_FLAGS) -fmessage-length=0 -ffunction-sections -c -MMD -MP -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/bootloader/%.o: $(SHARED_CODE_DIR)/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEVICE_DEFINES) $(OTHER_DEFINES) $(INCLUDE_PATH) $(COMPILE_FLAGS) -fmessage-length=0 -ffunction-sections -c -MMD -MP -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	