/*
 * RCCHelper.c
 *
 *  Created on: Aug 14, 2020
 *      Author: Poornachander
 * 
 *  Helper function for enabling/disabling clock to peripherals
 *
 *  Updated on: Mar 17, 2021
 *  ST has crappy I2C so we need a function for resetting the clock of peripherals
 *  Hopefully that's the only thing who needs the reset function
 *  -- Shuhang
 */

#include "stm32f4xx.h"
#include "assert.h"

typedef enum
{
    RCC_HELPER_CLOCK_AHB1,
    RCC_HELPER_CLOCK_AHB2,
    RCC_HELPER_CLOCK_AHB3,
    RCC_HELPER_CLOCK_APB1,
    RCC_HELPER_CLOCK_APB2,
} RCCHelper_clock_E;

void RCCHelper_clockCmd(const void * const periph, const FunctionalState newState)
{
    RCCHelper_clock_E clock;
    uint32_t bit;

    switch((uint32_t)periph)
    {
        // AHB1
        case (uint32_t)GPIOA:
        {
            bit =  RCC_AHB1ENR_GPIOAEN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)GPIOB:
        {
            bit =  RCC_AHB1ENR_GPIOBEN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)GPIOC:
        {
            bit =  RCC_AHB1ENR_GPIOCEN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)GPIOD:
        {
            bit =  RCC_AHB1ENR_GPIODEN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)GPIOE:
        {
            bit =  RCC_AHB1ENR_GPIOEEN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)GPIOF:
        {
            bit =  RCC_AHB1ENR_GPIOFEN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)GPIOG:
        {
            bit =  RCC_AHB1ENR_GPIOGEN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)GPIOH:
        {
            bit =  RCC_AHB1ENR_GPIOHEN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)CRC:
        {
            bit =  RCC_AHB1ENR_CRCEN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)DMA1:
        {
            bit =  RCC_AHB1ENR_DMA1EN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }
        case (uint32_t)DMA2:
        {
            bit =  RCC_AHB1ENR_DMA2EN;
            clock = RCC_HELPER_CLOCK_AHB1;
            break;
        }

        // AHB2
        case (uint32_t)RNG:
        {
            bit =  RCC_AHB2ENR_RNGEN;
            clock = RCC_HELPER_CLOCK_AHB2;
            break;
        }

        // AHB3
        case (uint32_t)QUADSPI:
        {
            bit =  RCC_AHB3ENR_QSPIEN;
            clock = RCC_HELPER_CLOCK_AHB3;
            break;
        }
        case (uint32_t)FSMC_Bank1:
        case (uint32_t)FSMC_Bank1E:
        case (uint32_t)FSMC_Bank2:
        case (uint32_t)FSMC_Bank3:
        case (uint32_t)FSMC_Bank4:
        {
            bit =  RCC_AHB3ENR_FSMCEN;
            clock = RCC_HELPER_CLOCK_AHB3;
            break;
        }

        // APB1
        case (uint32_t)UART8:
        {
            bit = RCC_APB1ENR_UART8EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)UART7:
        {
            bit = RCC_APB1ENR_UART7EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)DAC:
        {
            bit = RCC_APB1ENR_DACEN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)PWR:
        {
            bit = RCC_APB1ENR_PWREN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)CAN3:
        {
            bit = (1U << 27); // There is no definition for CAN3EN. Thanks ST -_-
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)CAN2:
        {
            bit = RCC_APB1ENR_CAN2EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)CAN1:
        {
            bit = RCC_APB1ENR_CAN1EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)FMPI2C1:
        {
            bit = RCC_APB1ENR_FMPI2C1EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)I2C3:
        {
            bit = RCC_APB1ENR_I2C3EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)I2C2:
        {
            bit = RCC_APB1ENR_I2C2EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)I2C1:
        {
            bit = RCC_APB1ENR_I2C1EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)UART5:
        {
            bit = RCC_APB1ENR_UART5EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)UART4:
        {
            bit = RCC_APB1ENR_UART4EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)USART3:
        {
            bit = RCC_APB1ENR_USART3EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)USART2:
        {
            bit = RCC_APB1ENR_USART2EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)SPI3:
        {
            bit = RCC_APB1ENR_SPI3EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)SPI2:
        {
            bit = RCC_APB1ENR_SPI2EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)WWDG:
        {
            bit = RCC_APB1ENR_WWDGEN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)LPTIM1:
        {
            bit = RCC_APB1ENR_LPTIM1EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM14:
        {
            bit = RCC_APB1ENR_TIM14EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM13:
        {
            bit = RCC_APB1ENR_TIM13EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM12:
        {
            bit = RCC_APB1ENR_TIM12EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM7:
        {
            bit = RCC_APB1ENR_TIM7EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM6:
        {
            bit = RCC_APB1ENR_TIM6EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM5:
        {
            bit = RCC_APB1ENR_TIM5EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM4:
        {
            bit = RCC_APB1ENR_TIM4EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM3:
        {
            bit = RCC_APB1ENR_TIM3EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM2:
        {
            bit = RCC_APB1ENR_TIM2EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }

        // APB2
        case (uint32_t)SAI1:
        {
            bit = RCC_APB2ENR_SAI1EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SPI5:
        {
            bit = RCC_APB2ENR_SPI5EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM11:
        {
            bit = RCC_APB2ENR_TIM11EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM10:
        {
            bit = RCC_APB2ENR_TIM10EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM9:
        {
            bit = RCC_APB2ENR_TIM9EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SYSCFG:
        {
            bit = RCC_APB2ENR_SYSCFGEN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SPI4:
        {
            bit = RCC_APB2ENR_SPI4EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SPI1:
        {
            bit = RCC_APB2ENR_SPI1EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SDIO:
        {
            bit = RCC_APB2ENR_SDIOEN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)ADC1:
        {
            bit = RCC_APB2ENR_ADC1EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)UART10:
        {
            bit = RCC_APB2ENR_UART10EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)UART9:
        {
            bit = RCC_APB2ENR_UART9EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)USART6:
        {
            bit = RCC_APB2ENR_USART6EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)USART1:
        {
            bit = RCC_APB2ENR_USART1EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM8:
        {
            bit = RCC_APB2ENR_TIM8EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM1:
        {
            bit = RCC_APB2ENR_TIM1EN;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }

        default:
        {
            assert(0U);
        }
    }

    switch(clock)
    {
        case RCC_HELPER_CLOCK_AHB1:
        {
            if(newState == ENABLE)
            {
                RCC->AHB1ENR |= bit;
            }
            else
            {
                RCC->AHB1ENR &= ~bit;
            }
            break;
        }
        case RCC_HELPER_CLOCK_AHB2:
        {
            if(newState == ENABLE)
            {
                RCC->AHB2ENR |= bit;
            }
            else
            {
                RCC->AHB2ENR &= ~bit;
            }
            break;
        }
        case RCC_HELPER_CLOCK_AHB3:
        {
            if(newState == ENABLE)
            {
                RCC->AHB3ENR |= bit;
            }
            else
            {
                RCC->AHB3ENR &= ~bit;
            }
            break;
        }
        case RCC_HELPER_CLOCK_APB1:
        {
            if(newState == ENABLE)
            {
                RCC->APB1ENR |= bit;
            }
            else
            {
                RCC->APB1ENR &= ~bit;
            }
            break;
        }
        case RCC_HELPER_CLOCK_APB2:
        {
            if(newState == ENABLE)
            {
                RCC->APB2ENR |= bit;
            }
            else
            {
                RCC->APB2ENR &= ~bit;
            }
            break;
        }
        default:
        {
            assert(0U);
            break;
        }
    }
}


void RCCHelper_resetCmd(const void * const periph, const FunctionalState newState){
	RCCHelper_clock_E clock;
	uint32_t bit;

	switch((uint32_t)periph)
	{
	    // AHB1
	    case (uint32_t)GPIOA:
	    {
	        bit =  RCC_AHB1RSTR_GPIOARST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	        }
	    case (uint32_t)GPIOB:
	    {
	        bit =  RCC_AHB1RSTR_GPIOBRST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }
	    case (uint32_t)GPIOC:
	    {
	        bit =  RCC_AHB1RSTR_GPIOCRST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }
	    case (uint32_t)GPIOD:
	    {
	        bit =  RCC_AHB1RSTR_GPIODRST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }
	    case (uint32_t)GPIOE:
	    {
	        bit =  RCC_AHB1RSTR_GPIOERST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }
	    case (uint32_t)GPIOF:
	    {
	        bit =  RCC_AHB1RSTR_GPIOFRST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }
	    case (uint32_t)GPIOG:
	    {
	        bit =  RCC_AHB1RSTR_GPIOGRST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }
	    case (uint32_t)GPIOH:
	    {
	        bit =  RCC_AHB1RSTR_GPIOHRST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }
	    case (uint32_t)CRC:
	    {
	        bit =  RCC_AHB1RSTR_CRCRST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }
	    case (uint32_t)DMA1:
	    {
	        bit =  RCC_AHB1RSTR_DMA1RST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }
	    case (uint32_t)DMA2:
	    {
	        bit =  RCC_AHB1RSTR_DMA2RST;
	        clock = RCC_HELPER_CLOCK_AHB1;
	        break;
	    }

	    // AHB2
	    case (uint32_t)RNG:
	    {
	        bit =  RCC_AHB2RSTR_RNGRST;
	        clock = RCC_HELPER_CLOCK_AHB2;
	        break;
	    }

	    // AHB3
	    case (uint32_t)QUADSPI:
	    {
	        bit =  RCC_AHB3RSTR_QSPIRST;
	        clock = RCC_HELPER_CLOCK_AHB3;
	        break;
	    }
	    case (uint32_t)FSMC_Bank1:
	    case (uint32_t)FSMC_Bank1E:
	    case (uint32_t)FSMC_Bank2:
	    case (uint32_t)FSMC_Bank3:
	    case (uint32_t)FSMC_Bank4:
	    {
	        bit =  RCC_AHB3RSTR_FSMCRST;
	        clock = RCC_HELPER_CLOCK_AHB3;
	        break;
	    }

	    // APB1
	    case (uint32_t)UART8:
	    {
	        bit = RCC_APB1RSTR_UART8RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)UART7:
	    {
	        bit = RCC_APB1RSTR_UART7RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)DAC:
	    {
	        bit = RCC_APB1RSTR_DACRST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)PWR:
	    {
	        bit = RCC_APB1RSTR_PWRRST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)CAN3:
	    {
	        bit = (1U << 27); // There is also no definition for CAN3RST. Thanks ST T_T
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)CAN2:
	    {
	        bit = RCC_APB1RSTR_CAN2RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)CAN1:
	    {
	        bit = RCC_APB1RSTR_CAN1RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)FMPI2C1:
	    {
	        bit = RCC_APB1RSTR_FMPI2C1RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)I2C3:
	    {
	        bit = RCC_APB1RSTR_I2C3RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)I2C2:
	    {
	        bit = RCC_APB1RSTR_I2C2RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)I2C1:
	    {
	        bit = RCC_APB1RSTR_I2C1RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)UART5:
	    {
	        bit = RCC_APB1RSTR_UART5RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)UART4:
	    {
	        bit = RCC_APB1RSTR_UART4RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)USART3:
	    {
	        bit = RCC_APB1RSTR_USART3RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)USART2:
	    {
	        bit = RCC_APB1RSTR_USART2RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)SPI3:
	    {
	        bit = RCC_APB1RSTR_SPI3RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)SPI2:
	    {
	        bit = RCC_APB1RSTR_SPI2RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)WWDG:
	    {
	        bit = RCC_APB1RSTR_WWDGRST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)LPTIM1:
	    {
	        bit = RCC_APB1RSTR_LPTIM1RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
	    case (uint32_t)TIM14:
	    {
	        bit = RCC_APB1RSTR_TIM14RST;
	        clock = RCC_HELPER_CLOCK_APB1;
	        break;
	    }
        case (uint32_t)TIM13:
        {
            bit = RCC_APB1RSTR_TIM13RST;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM12:
        {
            bit = RCC_APB1RSTR_TIM12RST;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM7:
        {
            bit = RCC_APB1RSTR_TIM7RST;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM6:
        {
            bit = RCC_APB1RSTR_TIM6RST;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM5:
        {
            bit = RCC_APB1RSTR_TIM5RST;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM4:
        {
            bit = RCC_APB1RSTR_TIM4RST;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM3:
        {
            bit = RCC_APB1RSTR_TIM3RST;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)TIM2:
        {
            bit = RCC_APB1RSTR_TIM2RST;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        // APB2
        case (uint32_t)SAI1:
        {
            bit = RCC_APB2RSTR_SAI1RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SPI5:
        {
            bit = RCC_APB2RSTR_SPI5RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM11:
        {
            bit = RCC_APB2RSTR_TIM11RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM10:
        {
            bit = RCC_APB2RSTR_TIM10RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM9:
        {
            bit = RCC_APB2RSTR_TIM9RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SYSCFG:
        {
            bit = RCC_APB2RSTR_SYSCFGRST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SPI4:
        {
            bit = RCC_APB2RSTR_SPI4RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SPI1:
        {
            bit = RCC_APB2RSTR_SPI1RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)SDIO:
        {
            bit = RCC_APB2RSTR_SDIORST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)ADC1:
        {
            bit = RCC_APB2RSTR_ADCRST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)UART10:
        {
            bit = RCC_APB2RSTR_UART10RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)UART9:
        {
            bit = RCC_APB2RSTR_UART9RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)USART6:
        {
            bit = RCC_APB2RSTR_USART6RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)USART1:
        {
            bit = RCC_APB2RSTR_USART1RST ;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM8:
        {
            bit = RCC_APB2RSTR_TIM8RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
        case (uint32_t)TIM1:
        {
            bit = RCC_APB2RSTR_TIM1RST;
            clock = RCC_HELPER_CLOCK_APB2;
            break;
        }
	        default:
        {
            assert(0U);
        }
	}
    switch(clock)
    {
        case RCC_HELPER_CLOCK_AHB1:
        {
            if(newState == ENABLE)
            {
            	RCC->AHB1RSTR |= bit;
            }
            else
            {
            	RCC->AHB1RSTR &= ~bit;
            }
            break;
        }
        case RCC_HELPER_CLOCK_AHB2:
        {
            if(newState == ENABLE)
            {
            	RCC->AHB2RSTR |= bit;
            }
            else
            {
            	RCC->AHB2RSTR &= ~bit;
            }
            break;
        }
        case RCC_HELPER_CLOCK_AHB3:
        {
            if(newState == ENABLE)
            {
            	RCC->AHB3RSTR |= bit;
            }
            else
            {
            	RCC->AHB3RSTR &= ~bit;
            }
            break;
        }
        case RCC_HELPER_CLOCK_APB1:
        {
            if(newState == ENABLE)
            {
            	RCC->APB1RSTR |= bit;
            }
            else
            {
            	RCC->APB1RSTR &= ~bit;
            }
            break;
        }
        case RCC_HELPER_CLOCK_APB2:
        {
            if(newState == ENABLE)
            {
            	RCC->APB2RSTR |= bit;
            }
            else
            {
            	RCC->APB2RSTR &= ~bit;
            }
            break;
        }
        default:
        {
            assert(0U);
            break;
        }
    }
}

