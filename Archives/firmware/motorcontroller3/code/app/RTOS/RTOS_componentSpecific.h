/*
 * RTOS_componentSpecific.h
 *
 *  Created on: Aug 5, 2020
 *      Author: Poornachander
 */

#ifndef RTOS_COMPONENTSPECIFIC_H_
#define RTOS_COMPONENTSPECIFIC_H_

#define RTOS_TASK_STACK_SIZE_1000MS	(400U)
#define RTOS_TASK_STACK_SIZE_100MS	(400U)
#define RTOS_TASK_STACK_SIZE_10MS	(400U)
#define RTOS_TASK_STACK_SIZE_1MS	(400U)

void RTOS_init(void);
void RTOS_run1ms(void);
void RTOS_run10ms(void);
void RTOS_run100ms(void);
void RTOS_run1000ms(void);

#endif /* RTOS_COMPONENTSPECIFIC_H_ */
