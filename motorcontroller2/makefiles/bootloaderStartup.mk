
BOOTLOADER_OBJS += \
$(OUTPUT_DIR)/obj/bootloader/startup/system_stm32f4xx.o \
$(OUTPUT_DIR)/obj/bootloader/startup/startup_stm32f411xe.o \
$(OUTPUT_DIR)/obj/bootloader/startup/main.o

BOOTLOADER_C_DEFS += \
$(OUTPUT_DIR)/obj/bootloader/startup/system_stmf4xx.d \
$(OUTPUT_DIR)/obj/bootloader/startup/stm32f4xx_it.d \
$(OUTPUT_DIR)/obj/bootloader/startup/main.d

INCLUDE_PATH+= \
-I$(SHARED_CODE_DIR)/bootloader

$(shell mkdir -p $(OUTPUT_DIR)/obj/bootloader/startup)

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/bootloader/startup/%.o: $(CODE_DIR)/startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEVICE_DEFINES) $(OTHER_DEFINES) $(INCLUDE_PATH) -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

	# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/bootloader/startup/startup_stm32f411xe.o: $(CODE_DIR)/startup/startup_stm32f411xe.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(INCLUDE_PATH) -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	