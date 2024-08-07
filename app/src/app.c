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
 * @file   : app.c
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
#include "task_a.h"
#include "task_b0.h"

#include "task_c0.h"
#include "task_c1.h"
#include "task_c2.h"
#include "task_c3.h"
#include <stdint.h>

/********************** macros and definitions *******************************/
#define G_APP_CNT_INI	0u
#define TASK_QTY 		5u

//
////Sensor de temperatura
//#define V25 1.43
//#define VSEN 3.3/4096
//#define AVG_SLOPE 0.0043



typedef struct {
	void (*p_task_x)(void *);	// Pointer to task
								// (must be a 'void (void *)' function)
	void *parameters;			// Pointer to parameters
} task_x_t;

/********************** internal data declaration ****************************/
volatile uint32_t value = 0;
float temp_a = 0;

task_x_t task_x_init_list[TASK_QTY]		= {{task_a_init, 	NULL},
										   {task_b0_init, 	NULL},
										   {task_c0_init,	NULL},
										   {task_c1_init,	NULL},
										   {task_c2_init,	NULL}};

task_x_t task_x_update_list[TASK_QTY]	= {{task_a_update, 	&temp_a},
										   {task_b0_update, NULL},
										   {task_c0_update,	NULL},
										   {task_c1_update,	NULL},
										   {task_c2_update,	NULL}};

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
//const char *p_sys	= " Bare Metal - Event-Triggered Systems (ETS)\r\n";
//const char *p_app	= " App - Model Integration\r\n";

/********************** external data declaration *****************************/
uint32_t g_app_cnt;

volatile uint32_t g_tick_cnt;



/********************** external functions definition ************************/


void app_init(void)
{
	uint32_t index;

	//float temp = 0;


//	/* Print out: Application Initialized */
//	LOGGER_LOG("\r\n");
//	LOGGER_LOG("%s is running - Tick [mS] = %d\r\n", GET_NAME(app_init), (int)HAL_GetTick());
//
//	LOGGER_LOG(p_sys);
//	LOGGER_LOG(p_app);

	g_app_cnt = G_APP_CNT_INI;

//	/* Print out: Application execution counter */
//	LOGGER_LOG(" %s = %d\r\n", GET_NAME(g_app_cnt), (int)g_app_cnt);

	for (index = 0; index < (sizeof(task_x_init_list)/sizeof(task_x_t)); index++)
	{
		/* C Functions (https://www.geeksforgeeks.org/) */
		/*
		 * A function call is a statement that instructs the compiler to execute
		 * the function.
		 * We use the function name and parameters in the function call.
		 */
		/* Run task_x_init */
		(*task_x_init_list[index].p_task_x)(task_x_init_list[index].parameters);
	}

	cycle_counter_init();
	//HAL_ADC_Start_IT(&hadc1);
}

void app_update(float temp)
{
	uint32_t index;
//	uint32_t cycle_counter;
//	uint32_t cycle_counter_time_us;
	temp_a = temp;
	/* Update App Counter */
	//g_app_cnt++;

    /* Print out: Application Update */
//	LOGGER_LOG("\r\n");
//	LOGGER_LOG("%s is running - Tick [mS] = %d\r\n", GET_NAME(app_update), (int)HAL_GetTick());

	/* Print out: Application execution counter */
//	LOGGER_LOG(" %s = %d\r\n", GET_NAME(g_app_cnt), (int)g_app_cnt);

	for (index = 0; index < (sizeof(task_x_update_list)/sizeof(task_x_t)); index++)
	{
		/* Print out: Cycle Counter Reset */
//		LOGGER_LOG("\r\n");
//		LOGGER_LOG(" %s\r\n", GET_NAME(cycle_counter_reset));

		//HAL_GPIO_TogglePin(LED_A_PORT, LED_A_PIN);
//		cycle_counter_reset();

		/* C Functions (https://www.geeksforgeeks.org/) */
		/*
		 * A function call is a statement that instructs the compiler to execute
		 * the function.
		 * We use the function name and parameters in the function call.
		 */
		/* Run task_x_update */
		(*task_x_update_list[index].p_task_x)(task_x_update_list[index].parameters);

//		cycle_counter = cycle_counter_get();
//		cycle_counter_time_us = cycle_counter_time_us();
		//HAL_GPIO_TogglePin(LED_A_PORT, LED_A_PIN);

		/* Print out: Cycle Counter */
//		LOGGER_LOG(" %s: %lu - %s: %lu uS\r\n", GET_NAME(cycle_counter), cycle_counter, GET_NAME(cycle_counter_time_us), cycle_counter_time_us);
	}
}

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//	temp = ( (V25 - VSEN*ADC_VAL)/AVG_SLOPE) + 25;
//}
//
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc, ADC_HandleTypeDef hadc1){
//	if (hadc == &hadc1){
//		value = HAL_ADC_GetValue(&hadc1);
//		temp = ( (V25 - VSEN*value)/AVG_SLOPE) + 25;
//		HAL_ADC_Stop_IT(&hadc1);
//		//LOGGER_LOG("value = %d \r\n", (int) value);
//	}
//}

//void HAL_SYSTICK_Callback(void)
//{
//	g_tick_cnt++;
//}

/********************** end of file ******************************************/
