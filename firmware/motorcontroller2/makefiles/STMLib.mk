
STM32F4_LIB_DIR:=$(REPO_ROOT)/libs/STM/STM32F4xx_DSP_StdPeriph_Lib_V1.8.0
STM32F4_STD_PERIPH_DIR:=$(STM32F4_LIB_DIR)/Libraries/STM32F4xx_StdPeriph_Driver
CMSIS_DIR=$(STM32F4_LIB_DIR)/Libraries/CMSIS
STM32F4_CMISS_DIR:=$(CMSIS_DIR)/Device/ST/STM32F4xx

LIB_OBJS += \
$(OUTPUT_DIR)/obj/STM_Std_Periph/misc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_adc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_can.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_cec.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_crc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_cryp.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_cryp_aes.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_cryp_des.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_cryp_tdes.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_dac.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_dbgmcu.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_dcmi.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_dfsdm.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_dma.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_dma2d.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_dsi.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_exti.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_flash.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_flash_ramfunc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_fmpi2c.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_gpio.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_hash.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_hash_md5.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_hash_sha1.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_i2c.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_iwdg.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_lptim.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_ltdc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_pwr.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_qspi.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_rcc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_rng.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_rtc.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_sai.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_sdio.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_spdifrx.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_spi.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_syscfg.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_tim.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_usart.o \
$(OUTPUT_DIR)/obj/STM_Std_Periph/stm32f4xx_wwdg.o \

LIB_C_DEPS += $(LIB_OBJS:%.o=%.d)

INCLUDE_PATH += \
-I$(STM32F4_STD_PERIPH_DIR)/inc \
-I$(STM32F4_CMISS_DIR)/Include \
-I$(CMSIS_DIR)/Include

$(shell mkdir -p $(OUTPUT_DIR)/obj/STM_Std_Periph)

ARM_TOOLS_COMPILER_FLAGS := \
-O0 \
-g3 \
-Wall

$(OUTPUT_DIR)/obj/STM_Std_Periph/%.o: $(STM32F4_STD_PERIPH_DIR)/src/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)
