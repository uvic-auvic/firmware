APP_OBJS += \
$(OUTPUT_DIR)/obj/app/RTOS/RTOS.o \
$(OUTPUT_DIR)/obj/app/RTOS/RTOS_componentSpecific.o \
$(OUTPUT_DIR)/obj/app/LED.o \
$(OUTPUT_DIR)/obj/app/LED_componentSpecific.o \
$(OUTPUT_DIR)/obj/app/circBuffer1D.o \
$(OUTPUT_DIR)/obj/app/circBuffer1D_componentSpecific.o \
$(OUTPUT_DIR)/obj/app/debug.o \
$(OUTPUT_DIR)/obj/app/debug_componentSpecific.o \
$(OUTPUT_DIR)/obj/app/CAN.o \
$(OUTPUT_DIR)/obj/app/CAN_componentSpecific.o \
$(OUTPUT_DIR)/obj/app/messageHandler.o \
$(OUTPUT_DIR)/obj/app/messageHandler_componentSpecific.o \
$(OUTPUT_DIR)/obj/app/RCCHelper.o \
$(OUTPUT_DIR)/obj/app/interruptHelper.o \
$(OUTPUT_DIR)/obj/app/ADC.o \
$(OUTPUT_DIR)/obj/app/powerManagement.o \
$(OUTPUT_DIR)/obj/app/housingMonitor.o \

APP_C_DEPS += $(APP_OBJS:%.o=%.d)

INCLUDE_PATH+= \
-I$(SHARED_CODE_DIR) \
-I$(SHARED_CODE_DIR)/RTOS \
-I$(SHARED_DIR)/data/protocol/protocol \
-I$(SHARED_CODE_DIR)/utils \

$(shell mkdir -p $(OUTPUT_DIR)/obj/app)
$(shell mkdir -p $(OUTPUT_DIR)/obj/app/RTOS)

APP_COMPILER_FLAGS = \
$(COMPILER_OPTIMIZATION) \
-g3 \
-Wall \
-Werror \
-Wswitch-enum \
-Wswitch-default \
-Wextra

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/app/%.o: $(CODE_DIR)/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(APP_COMPILER_FLAGS)" ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)

# Each subdirectory must supply rules for building sources it contributes
$(OUTPUT_DIR)/obj/app/%.o: $(SHARED_CODE_DIR)/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(APP_COMPILER_FLAGS)" ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)
