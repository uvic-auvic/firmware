/*
 * RTOS_componentSpecific.h
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#ifndef RTOS_COMPONENTSPECIFIC_H_
#define RTOS_COMPONENTSPECIFIC_H_

#define RTOS_1MS_TASK_STACK_SIZE     (200U) 
#define RTOS_10MS_TASK_STACK_SIZE    (100U) 
#define RTOS_100MS_TASK_STACK_SIZE   (100U) 
#define RTOS_1000MS_TASK_STACK_SIZE  (100U) 

void RTOS_init(void);
void RTOS_run1ms(void);
void RTOS_run10ms(void);
void RTOS_run100ms(void);
void RTOS_run1000ms(void);

#endif /* RTOS_COMPONENTSPECIFIC_H_ */
