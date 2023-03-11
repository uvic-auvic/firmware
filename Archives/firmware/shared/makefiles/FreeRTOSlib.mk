
FREE_RTOS_DIR=$(REPO_ROOT)/libs/FreeRTOS/FreeRTOS

LIB_OBJS += \
$(OUTPUT_DIR)/obj/FreeRTOS/croutine.o \
$(OUTPUT_DIR)/obj/FreeRTOS/event_groups.o \
$(OUTPUT_DIR)/obj/FreeRTOS/list.o \
$(OUTPUT_DIR)/obj/FreeRTOS/queue.o \
$(OUTPUT_DIR)/obj/FreeRTOS/tasks.o \
$(OUTPUT_DIR)/obj/FreeRTOS/timers.o \
$(OUTPUT_DIR)/obj/FreeRTOS/port.o \
$(OUTPUT_DIR)/obj/FreeRTOS/heap_2.o

LIB_C_DEPS += $(LIB_OBJS:%.o=%.d)

ifeq ($(MCU_ARCHITECTURE), M0)
	PORT_TO_USE=ARM_CM0
else # M4
	PORT_TO_USE=ARM_CM4F
endif

INCLUDE_PATH+= \
-I$(FREE_RTOS_DIR)/Source/include \
-I$(FREE_RTOS_DIR)/Source/portable/GCC/$(PORT_TO_USE) \
-I$(CODE_DIR)/RTOS

$(shell mkdir -p $(OUTPUT_DIR)/obj/FreeRTOS)

FREE_RTOS_COMPILER_FLAGS := \
$(COMPILER_OPTIMIZATION) \
-g3 \
-Wall

$(OUTPUT_DIR)/obj/FreeRTOS/%.o: $(FREE_RTOS_DIR)/Source/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(FREE_RTOS_COMPILER_FLAGS)" ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)

$(OUTPUT_DIR)/obj/FreeRTOS/%.o: $(FREE_RTOS_DIR)/Source/portable/GCC/$(PORT_TO_USE)/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(FREE_RTOS_COMPILER_FLAGS)" ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)
	
$(OUTPUT_DIR)/obj/FreeRTOS/%.o: $(FREE_RTOS_DIR)/Source/portable/MemMang/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(FREE_RTOS_COMPILER_FLAGS)" ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)
