/*
 * RTOS_componentSpecific.h
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#ifndef RTOS_COMPONENTSPECIFIC_H_
#define RTOS_COMPONENTSPECIFIC_H_

#define RTOS_TASK_STACK_SIZE_1000MS	(100U)
#define RTOS_TASK_STACK_SIZE_100MS	(100U)
#define RTOS_TASK_STACK_SIZE_10MS	(100U)
#define RTOS_TASK_STACK_SIZE_1MS	(100U)

void RTOS_init(void);
void RTOS_run1ms(void);
void RTOS_run10ms(void);
void RTOS_run100ms(void);
void RTOS_run1000ms(void);

#endif /* RTOS_COMPONENTSPECIFIC_H_ */
