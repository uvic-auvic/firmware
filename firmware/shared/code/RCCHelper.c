/*
 * RCCHelper.c
 *
 *  Created on: Aug 14, 2020
 *      Author: Poornachander
 */

#include "stm32f4xx.h"
#include "assert.h"

typedef enum
{
    RCC_HELPER_CLOCK_AHB1,
    RCC_HELPER_CLOCK_APB1,
    RCC_HELPER_CLOCK_APB2,
} RCCHelper_clock_E;

void RCCHelper_clockCmd(const void * const periph, const FunctionalState newState)
{
    RCCHelper_clock_E clock;
    uint32_t bit;

    switch((uint32_t)periph)
    {
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
        case (uint32_t)CAN1:
        {
            bit = RCC_APB1ENR_CAN1EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)CAN2:
        {
            bit = RCC_APB1ENR_CAN2EN;
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)CAN3:
        {
            bit = (1U << 27); // There is no definition for CAN3EN. Thanks ST -_-
            clock = RCC_HELPER_CLOCK_APB1;
            break;
        }
        case (uint32_t)USART6:
        {
            bit = RCC_APB2ENR_USART6EN;
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

