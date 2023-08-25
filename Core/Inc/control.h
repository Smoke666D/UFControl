/*
 * control.h
 *
 *  Created on: Aug 25, 2023
 *      Author: i.dymov
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "main.h"

typedef enum
{
	CONTROLLER_INIT,
	CONTROLLER_WORK,
	CONTROLLER_REINIT,
	CONTROLLER_SHOTDOWN,
} CONTROLLER_STATE_t;

void StartControlTask(void *argument);

#endif /* INC_CONTROL_H_ */
