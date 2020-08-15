/*
 * interruptHelper.c
 *
 *  Created on: Aug 14, 2020
 *      Author: Poornachander
 */

#ifndef SHARED_CODE_INTERRUPTHELPER_C_
#define SHARED_CODE_INTERRUPTHELPER_C_

#include "interruptHelper.h"

/* INCLUDES */
#include <stdlib.h>
#include "assert.h"

/* DEFINES */

/* TYPEDEFS */
typedef struct
{
    IRQCallback USART1Callback;
    IRQCallback USART2Callback;
    IRQCallback USART3Callback;
    IRQCallback USART6Callback;
    IRQCallback UART4Callback;
    IRQCallback UART5Callback;
    IRQCallback UART7Callback;
    IRQCallback UART8Callback;
    IRQCallback UART9Callback;
    IRQCallback UART10Callback;

} interruptHelper_data_S;

/* PRIVATE DATA */
static interruptHelper_data_S interruptHelper_data;

/* PRIVATE FUNCTIONS DECLARATION */

/* PRIVATE FUNCTION DEFINITION */

/* PUBLIC FUNCTIONS */

/* USART/UART */
IRQn_Type interruptHelper_getIRQn_USART(USART_TypeDef * UARTPeriph)
{
    IRQn_Type ret = ~((IRQn_Type)0U); // Maximum number for type IRQn_Type

    if(IS_USART_ALL_PERIPH(UARTPeriph))
    {
        switch((uint32_t)UARTPeriph)
        {
            case (uint32_t)USART1:
            {
                ret = USART1_IRQn;
                break;
            }
            case (uint32_t)USART2:
            {
                ret = USART2_IRQn;
                break;
            }
            case (uint32_t)USART3:
            {
                ret = USART3_IRQn;
                break;
            }
            case (uint32_t)USART6:
            {
                ret = USART6_IRQn;
                break;
            }
            case (uint32_t)UART4:
            {
                ret = UART4_IRQn;
                break;
            }
            case (uint32_t)UART5:
            {
                ret = UART5_IRQn;
                break;
            }
            case (uint32_t)UART7:
            {
                ret = UART7_IRQn;
                break;
            }
            case (uint32_t)UART8:
            {
                ret = UART8_IRQn;
                break;
            }
            case (uint32_t)UART9:
            {
                ret = UART9_IRQn;
                break;
            }
            case (uint32_t)UART10:
            {
                ret = UART10_IRQn;
                break;
            }
            default:
            {
                ret = ~((IRQn_Type)0U);
                break;
            }
        }
    }

    return ret;
}

void interruptHelper_registerCallback_USART(USART_TypeDef * UARTPeriph, IRQCallback callback)
{
    if(IS_USART_ALL_PERIPH(UARTPeriph))
    {
        if(callback != NULL)
        {
            switch((uint32_t)UARTPeriph)
            {
                case (uint32_t)USART1:
                {
                    interruptHelper_data.USART1Callback = callback;
                    break;
                }
                case (uint32_t)USART2:
                {
                    interruptHelper_data.USART2Callback = callback;
                    break;
                }
                case (uint32_t)USART3:
                {
                    interruptHelper_data.USART3Callback = callback;
                    break;
                }
                case (uint32_t)USART6:
                {
                    interruptHelper_data.USART6Callback = callback;
                    break;
                }
                case (uint32_t)UART4:
                {
                    interruptHelper_data.UART4Callback = callback;
                    break;
                }
                case (uint32_t)UART5:
                {
                    interruptHelper_data.UART5Callback = callback;
                    break;
                }
                case (uint32_t)UART7:
                {
                    interruptHelper_data.UART7Callback = callback;
                    break;
                }
                case (uint32_t)UART8:
                {
                    interruptHelper_data.UART8Callback = callback;
                    break;
                }
                case (uint32_t)UART9:
                {
                    interruptHelper_data.UART9Callback = callback;
                    break;
                }
                case (uint32_t)UART10:
                {
                    interruptHelper_data.UART10Callback = callback;
                    break;
                }
                default:
                {
                    assert(0U);
                    break;
                }
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    if(interruptHelper_data.USART1Callback != NULL)
    {
        interruptHelper_data.USART1Callback();
    }
}

void USART2_IRQHandler(void)
{
    if(interruptHelper_data.USART2Callback != NULL)
    {
        interruptHelper_data.USART2Callback();
    }
}

void USART3_IRQHandler(void)
{
    if(interruptHelper_data.USART3Callback != NULL)
    {
        interruptHelper_data.USART3Callback();
    }
}

void USART6_IRQHandler(void)
{
    if(interruptHelper_data.USART6Callback != NULL)
    {
        interruptHelper_data.USART6Callback();
    }
}

void UART4_IRQHandler(void)
{
    if(interruptHelper_data.UART4Callback != NULL)
    {
        interruptHelper_data.UART4Callback();
    }
}

void UART5_IRQHandler(void)
{
    if(interruptHelper_data.UART5Callback != NULL)
    {
        interruptHelper_data.UART5Callback();
    }
}

void UART7_IRQHandler(void)
{
    if(interruptHelper_data.UART7Callback != NULL)
    {
        interruptHelper_data.UART7Callback();
    }
}

void UART8_IRQHandler(void)
{
    if(interruptHelper_data.UART8Callback != NULL)
    {
        interruptHelper_data.UART8Callback();
    }
}

void UART9_IRQHandler(void)
{
    if(interruptHelper_data.UART9Callback != NULL)
    {
        interruptHelper_data.UART9Callback();
    }
}

void UART10_IRQHandler(void)
{
    if(interruptHelper_data.UART10Callback != NULL)
    {
        interruptHelper_data.UART10Callback();
    }
}

#endif /* SHARED_CODE_INTERRUPTHELPER_C_ */
