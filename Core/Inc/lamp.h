/*
 * lamp.h
 *
 *  Created on: Aug 1, 2023
 *      Author: i.dymov
 */

#ifndef INC_LAMP_H_
#define INC_LAMP_H_

#define MAX_LAMP_COUNT  44
#define LAMP_HOUR_STEP  1000

#include "main.h"

typedef enum
{
  NOT_CONNECT = 0,
  ACTIVE	  = 1,
  LAMP_ERROR  = 2,
} LAMP_STATUS;

typedef struct __packed
{
 LAMP_STATUS state;
 uint32_t timer;
 uint8_t work_time;

} LAMP_t;


typedef enum
{
  WAIT_FOR_INIT,
  INIT,
  WORK,
} LAMP_FSM_SATE;

void LAMPstart(void *argument);
#endif /* INC_LAMP_H_ */
