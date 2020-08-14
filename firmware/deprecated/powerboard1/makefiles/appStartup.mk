
APP_OBJS += \
$(OUTPUT_DIR)/obj/app/startup/system_stm32f0xx.o \
$(OUTPUT_DIR)/obj/app/startup/startup_stm32.o \
$(OUTPUT_DIR)/obj/app/startup/main.o

APP_C_DEPS += $(APP_OBJS:%.o=%.d)

ARM_TOOLS_COMPILER_FLAGS := \
-O0 \
-g3 \
-Wall

$(shell mkdir -p $(OUTPUT_DIR)/obj/app/startup)

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/app/startup/%.o: $(CODE_DIR)/startup/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)

	# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/app/startup/startup_stm32.o: $(CODE_DIR)/startup/startup_stm32.s
	@$(MAKE) --no-print-directory arm-assembler ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@
