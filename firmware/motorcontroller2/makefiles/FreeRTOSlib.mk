
FREE_RTOS_DIR=$(REPO_ROOT)/libs/FreeRTOS-Kernel/FreeRTOS

LIB_OBJS += \
$(OUTPUT_DIR)/obj/FreeRTOS/croutine.o \
$(OUTPUT_DIR)/obj/FreeRTOS/event_groups.o \
$(OUTPUT_DIR)/obj/FreeRTOS/list.o \
$(OUTPUT_DIR)/obj/FreeRTOS/queue.o \
$(OUTPUT_DIR)/obj/FreeRTOS/tasks.o \
$(OUTPUT_DIR)/obj/FreeRTOS/timers.o \
$(OUTPUT_DIR)/obj/FreeRTOS/port.o \
$(OUTPUT_DIR)/obj/FreeRTOS/heap_2.o

LIB_C_DEPS += \
$(OUTPUT_DIR)/obj/FreeRTOS/croutine.d \
$(OUTPUT_DIR)/obj/FreeRTOS/event_groups.d \
$(OUTPUT_DIR)/obj/FreeRTOS/list.d \
$(OUTPUT_DIR)/obj/FreeRTOS/queue.d \
$(OUTPUT_DIR)/obj/FreeRTOS/tasks.d \
$(OUTPUT_DIR)/obj/FreeRTOS/timers.d \
$(OUTPUT_DIR)/obj/FreeRTOS/port.d \
$(OUTPUT_DIR)/obj/FreeRTOS/heap_2.d

INCLUDE_PATH+= \
-I$(FREE_RTOS_DIR)/Source/include \
-I$(FREE_RTOS_DIR)/Source/portable/GCC/ARM_CM4F \
-I$(CODE_DIR)/RTOS

$(shell mkdir -p $(OUTPUT_DIR)/obj/FreeRTOS)

$(OUTPUT_DIR)/obj/FreeRTOS/%.o: $(FREE_RTOS_DIR)/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEVICE_DEFINES) $(OTHER_DEFINES) $(INCLUDE_PATH) -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OUTPUT_DIR)/obj/FreeRTOS/%.o: $(FREE_RTOS_DIR)/Source/portable/GCC/ARM_CM4F/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEVICE_DEFINES) $(OTHER_DEFINES) $(INCLUDE_PATH) -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
$(OUTPUT_DIR)/obj/FreeRTOS/%.o: $(FREE_RTOS_DIR)/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEVICE_DEFINES) $(OTHER_DEFINES) $(INCLUDE_PATH) -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	