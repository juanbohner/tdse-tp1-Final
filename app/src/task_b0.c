/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_b0.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
//#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "task_b0_fsm.h"
#include "task_b0.h"

/********************** macros and definitions *******************************/
#define G_TASK_B0_CNT_INI	0u

#define TASK_B0_DEL_INI		0u
#define TASK_B0_DEL_MAX		50u

/********************** internal data declaration ****************************/
s_task_b0_t state_task_b0;
e_task_b0_t event_task_b0;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_b0 		= "Task B (Sensor Modeling)";

/********************** external data declaration *****************************/
uint32_t g_task_b0_cnt;

e_task_b0_t g_event_task_b0;
bool g_b_event_task_b0;

/********************** external functions definition ************************/
void task_b0_init(void *parameters)
{
//	/* Print out: Task Initialized */
//	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_b0_init), p_task_b0);

	g_task_b0_cnt = G_TASK_B0_CNT_INI;

//	/* Print out: Task execution counter */
//	LOGGER_LOG("   %s = %d\r\n", GET_NAME(g_task_B0_cnt), (int)g_task_b0_cnt);

	state_task_b0 = STATE_TASK_B0_0;
	event_task_b0 = EVENT_TASK_B0_0;
	g_event_task_b0 = event_task_b0;
	g_b_event_task_b0 = false;

//	/* Print out: Task execution FSM */
//	LOGGER_LOG("   %s = %d\r\n", GET_NAME(state_task_b0), (int)state_task_b0);
//	LOGGER_LOG("   %s = %d\r\n", GET_NAME(event_task_b0), (int)event_task_b0);
}

void task_b0_update(void *parameters)
{
	/* Update Task B Counter */
	g_task_b0_cnt++;

	static uint32_t then_task_b0 = TASK_B0_DEL_INI;
	static uint32_t now_task_b0  = TASK_B0_DEL_INI;

	now_task_b0 = HAL_GetTick();
	if ((now_task_b0 - then_task_b0) >= TASK_B0_DEL_MAX)
	{
		/* Reset then = now */
		then_task_b0 = now_task_b0;
	}

	if (true == g_b_event_task_b0)
	{
		g_b_event_task_b0 = false;
		event_task_b0 =	g_event_task_b0;

		switch (state_task_b0)
		{
			case STATE_TASK_B0_0:

				if (EVENT_TASK_B0_1 == event_task_b0){
					HAL_GPIO_WritePin(LED_D1_PORT, LED_D1_PIN, LED_OFF);
					state_task_b0 =STATE_TASK_B0_1;
				}

				break;

			case STATE_TASK_B0_1:
				
				if (EVENT_TASK_B0_0 == event_task_b0){	
					HAL_GPIO_WritePin(LED_D1_PORT, LED_D1_PIN, LED_ON);
					state_task_b0 = STATE_TASK_B0_0;
				}

				break;

			default:

				break;
		}
	}
}

/********************** end of file ******************************************/
