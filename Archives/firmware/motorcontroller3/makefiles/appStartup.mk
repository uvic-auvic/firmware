
APP_OBJS += \
$(OUTPUT_DIR)/obj/app/startup/system_stm32f4xx.o \
$(OUTPUT_DIR)/obj/app/startup/startup_stm32f413_423xx.o \
$(OUTPUT_DIR)/obj/app/startup/main.o

APP_C_DEPS += $(APP_OBJS:%.o=%.d)

$(shell mkdir -p $(OUTPUT_DIR)/obj/app/startup)

APP_STARTUP_COMPILER_FLAGS = \
$(COMPILER_OPTIMIZATION) \
-g3 \
-Wall \
-Wswitch-enum \
-Wswitch-default \
-Wextra

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/app/startup/%.o: $(CODE_DIR)/startup/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(APP_STARTUP_COMPILER_FLAGS)" ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)

	# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/app/startup/startup_stm32f413_423xx.o: $(CODE_DIR)/startup/startup_stm32f413_423xx.s
	@$(MAKE) --no-print-directory arm-assembler ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@
