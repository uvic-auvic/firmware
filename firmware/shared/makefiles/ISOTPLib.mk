
ISOTP_LIB_DIR=$(REPO_ROOT)/libs/isotp-c-master/isotp-c-master

LIB_OBJS += \
$(OUTPUT_DIR)/obj/isotp/isotp.o \

LIB_C_DEPS += $(LIB_OBJS:%.o=%.d)

INCLUDE_PATH+= \
-I$(ISOTP_LIB_DIR) \

$(shell mkdir -p $(OUTPUT_DIR)/obj/isotp)

ISOTP_LIB_COMPILER_FLAGS := \
-O0 \
-g3 \
-Wall

$(OUTPUT_DIR)/obj/isotp/%.o: $(ISOTP_LIB_DIR)/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(ISOTP_LIB_COMPILER_FLAGS)" ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)

