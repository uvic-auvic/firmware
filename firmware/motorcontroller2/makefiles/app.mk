
APP_OBJS += \
$(OUTPUT_DIR)/obj/app/RTOS/RTOS.o \
$(OUTPUT_DIR)/obj/app/RTOS/RTOS_componentSpecific.o \
$(OUTPUT_DIR)/obj/app/LED.o

APP_C_DEFS += \
$(OUTPUT_DIR)/obj/app/RTOS/RTOS.d \
$(OUTPUT_DIR)/obj/app/RTOS/RTOS_componentSpecific.d \
$(OUTPUT_DIR)/obj/app/LED.d

COMPILE_FLAGS= \
-O0 \
-g3 \
-Wall \
-Werror

INCLUDE_PATH+= \
-I$(SHARED_CODE_DIR) \
-I$(SHARED_CODE_DIR)/RTOS \


$(shell mkdir -p $(OUTPUT_DIR)/obj/app)
$(shell mkdir -p $(OUTPUT_DIR)/obj/app/RTOS)

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/app/%.o: $(CODE_DIR)/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEVICE_DEFINES) $(OTHER_DEFINES) $(INCLUDE_PATH) $(COMPILE_FLAGS) -fmessage-length=0 -ffunction-sections -c -MMD -MP -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/app/%.o: $(SHARED_CODE_DIR)/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEVICE_DEFINES) $(OTHER_DEFINES) $(INCLUDE_PATH) $(COMPILE_FLAGS) -fmessage-length=0 -ffunction-sections -c -MMD -MP -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	