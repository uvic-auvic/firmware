
# STM32F4
STM32F4_LIB_DIR:=$(REPO_ROOT)/libs/STM/STM32F4xx_DSP_StdPeriph_Lib_V1.8.0
STM32F4_STD_PERIPH_DIR:=$(STM32F4_LIB_DIR)/Libraries/STM32F4xx_StdPeriph_Driver

# STM32F0
STM32F0_LIB_DIR:=$(REPO_ROOT)/libs/STM/STM32F0xx_StdPeriph_Lib_V1.5.0
STM32F0_STD_PERIPH_DIR:=$(STM32F0_LIB_DIR)/Libraries/STM32F0xx_StdPeriph_Driver

CMSIS_DIR=$(STM32F0_LIB_DIR)/Libraries/CMSIS
STM32F0_CMISS_DIR:=$(CMSIS_DIR)/Device/ST/STM32F0xx

LIB_OBJS += \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_adc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_can.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_cec.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_crc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_crs.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_dac.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_dbgmcu.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_dma.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_exti.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_flash.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_gpio.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_i2c.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_iwdg.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_misc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_pwr.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_rcc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_rtc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_spi.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_syscfg.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_tim.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_usart.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f0xx_wwdg.o \

LIB_C_DEPS += $(LIB_OBJS:%.o=%.d)

INCLUDE_PATH += \
-I$(STM32F0_STD_PERIPH_DIR)/inc \
-I$(STM32F0_CMISS_DIR)/Include \
-I$(CMSIS_DIR)/Include

$(shell mkdir -p $(OUTPUT_DIR)/obj/STM_Std_Periph)

STM_LIB_COMPILER_FLAGS = \
$(COMPILER_OPTIMIZATION) \
-g3 \
-Wall

$(OUTPUT_DIR)/obj/STM_Std_Periph/%.o: $(STM32F0_STD_PERIPH_DIR)/src/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_FLAGS="$(STM_LIB_COMPILER_FLAGS)" ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)
