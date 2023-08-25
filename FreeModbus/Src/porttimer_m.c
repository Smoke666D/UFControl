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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "../Inc/mb.h"
#include "../Inc/mb_m.h"
#include "../Inc/mbport.h"
#include "../Inc/port.h"
#include "mainFSM.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
static USHORT usT35TimeOut50us;
//static struct rt_timer timer;


/* ----------------------- static functions ---------------------------------*/


/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
    /* backup T35 ticks */

    vTimerInit( usTimeOut50us);
 /*   rt_timer_init(&timer, "master timer",
 //                  timer_timeout_ind, /* bind timeout callback function */
 //                  RT_NULL,
 //                  (50 * usT35TimeOut50us) / (1000 * 1000 / RT_TICK_PER_SECOND) + 1,
 //                  RT_TIMER_FLAG_ONE_SHOT); /* one shot */

    return TRUE;
}

void vMBMasterPortTimersT35Enable()
{
   // rt_tick_t timer_tick = (50 * usT35TimeOut50us)
   //         / (1000 * 1000 / RT_TICK_PER_SECOND);
//
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_T35);
 //   vTimerInit( usT35TimeOut50us);
   // rt_timer_control(&timer, RT_TIMER_CTRL_SET_TIME, &timer_tick);

    vStartTimer();
  //  rt_timer_start(&timer);
}

void vMBMasterPortTimersConvertDelayEnable()
{
 //  rt_tick_t timer_tick = MB_MASTER_DELAY_MS_CONVERT * RT_TICK_PER_SECOND / 1000;

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);
    vRespondInit(MB_MASTER_DELAY_MS_CONVERT);
 //   rt_timer_control(&timer, RT_TIMER_CTRL_SET_TIME, &timer_tick);

    vStartRespond();
 //   rt_timer_start(&timer);
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
  //  rt_tick_t timer_tick = MB_MASTER_TIMEOUT_MS_RESPOND * RT_TICK_PER_SECOND / 1000;

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);
    vRespondInit(MB_MASTER_TIMEOUT_MS_RESPOND);

    vStartRespond();
  //  rt_timer_control(&timer, RT_TIMER_CTRL_SET_TIME, &timer_tick);

  //  rt_timer_start(&timer);
}

void vMBMasterPortTimersDisable()
{
	vStopTimer();
	vStopRespond();
  //  rt_timer_stop(&timer);
}
#ifdef MASTER_MODE
void prvvTIMERExpiredISR(void)
{
    (void) pxMBMasterPortCBTimerExpired();
}

#endif

#endif
