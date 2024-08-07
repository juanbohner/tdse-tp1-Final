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
 * @file   : task_c2.c
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
#include "task_c2_fsm.h"
#include "task_c2.h"
#include "task_a_fsm.h"
#include "task_a_queue.h"

/********************** macros and definitions *******************************/
#define G_TASK_C2_CNT_INI	0u

#define TASK_C2_DEL_INI		0u
#define TASK_C2_DEL_MAX		50u

/********************** internal data declaration ****************************/
s_task_c2_t state_task_c2;
e_task_c2_t event_task_c2;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_c2 		= "Task C (Sensor Modeling)";

/********************** external data declaration *****************************/
uint32_t g_task_c2_cnt;

/********************** external functions definition ************************/
void task_c2_init(void *parameters)
{
//	/* Print out: Task Initialized */
//	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_c2_init), p_task_c2);

	g_task_c2_cnt = G_TASK_C2_CNT_INI;

//	/* Print out: Task execution counter */
//	LOGGER_LOG("   %s = %d\r\n", GET_NAME(g_task_c2_cnt), (int)g_task_c2_cnt);

	state_task_c2 = STATE_TASK_C2_0;
	event_task_c2 = EVENT_TASK_C2_0;
	//put_event_task_a(EVENT_TASK_A_4);

	//g_b_event_task_c = false;

//	/* Print out: Task execution FSM */
//	LOGGER_LOG("   %s = %d\r\n", GET_NAME(state_task_c2), (int)state_task_c2);
//	LOGGER_LOG("   %s = %d\r\n", GET_NAME(event_task_c2), (int)event_task_c2);
}

void task_c2_update(void *parameters)
{
	/* Update Task C Counter */
	g_task_c2_cnt++;

	static uint32_t then_task_c2 = TASK_C2_DEL_INI;
	static uint32_t now_task_c2  = TASK_C2_DEL_INI;
	

	if (BUTTON_PRESSED == HAL_GPIO_ReadPin(BUTTON_S3_PORT, BUTTON_S3_PIN)){
		event_task_c2 = EVENT_TASK_C2_1;	//Se toca el botón
	}
	else{
		event_task_c2 = EVENT_TASK_C2_0;	//NO Se toca el botón
	}

	switch (state_task_c2)
	{
		case STATE_TASK_C2_0:

			if (EVENT_TASK_C2_1 == event_task_c2){
				then_task_c2 = HAL_GetTick();
				state_task_c2 = STATE_TASK_C2_1;
			}

			break;

		case STATE_TASK_C2_1:
			now_task_c2 = HAL_GetTick();
			if ((now_task_c2 - then_task_c2) >= TASK_C2_DEL_MAX)
			{
				/* Reset then = now */
				state_task_c2 =STATE_TASK_C2_2;
			}

			break;

		case STATE_TASK_C2_2:

			if (EVENT_TASK_C2_0 == event_task_c2){
				state_task_c2 = STATE_TASK_C2_0;
			}

			if (EVENT_TASK_C2_1 == event_task_c2){
				put_event_task_a(EVENT_TASK_A_5);
				state_task_c2 =STATE_TASK_C2_3;
			}
			break;

		case STATE_TASK_C2_3:

			if (EVENT_TASK_C2_0 == event_task_c2){
				//put_event_task_a(EVENT_TASK_A_4);

				state_task_c2 = STATE_TASK_C2_0;
			}

		default:

			break;
	}
}

/********************** end of file ******************************************/
