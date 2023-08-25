/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "../Inc/mb.h"
#include "../Inc/mbport.h"
#include "../Inc/port.h"
#include "mainFSM.h"

/* ----------------------- static functions ---------------------------------*/
//static struct rt_timer timer;



/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{

   vTimerInit(usTim1Timerout50us);

  //  rt_timer_init(&timer, "slave timer",
   //                timer_timeout_ind, /* bind timeout callback function */
    //               RT_NULL,
     //              (50 * usTim1Timerout50us) / (1000 * 1000 / RT_TICK_PER_SECOND) + 1,
      //             RT_TIMER_FLAG_ONE_SHOT); /* one shot */
    return TRUE;
}

void vMBPortTimersEnable()
{
	vStartTimer();
}


void vMBPortTimersDisable()
{
	vStopTimer();
}


#ifdef SLAVE_MODE
void rvvTIMERExpiredISR()
{
    (void) pxMBPortCBTimerExpired();
}
#endif

