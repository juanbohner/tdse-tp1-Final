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
 * @file   : task_a.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"
#include <stdbool.h>

/* Demo includes. */
//#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "task_a_fsm.h"
#include "task_b0_fsm.h"
#include "task_b0.h"

#include "menu.h"
#include "display.h"

#include "task_a_queue.h"

/********************** macros and definitions *******************************/
#define G_TASK_A_CNT_INI	0u

#define TASK_A_DEL_INI		0u
#define TASK_A_DEL_MAX		50u

#define DISPLAY_REFRESH_TIME_MS  500
#define SYSTEM_TIME_INCREMENT_MS 5


////Sensor de temperatura
//#define V25 1.43
//#define VSEN 3.3/4096
//#define AVG_SLOPE 0.0043



/********************** internal data declaration ****************************/
s_task_a_t	state_task_a;
e_task_a_t	event_task_a;

/********************** internal functions declaration ***********************/
static void task_a_display_init(void);
static void task_a_display_update(float temp);

/********************** internal data definition *****************************/
//const char *p_task_a 		= "Task A (System Modeling)";

/********************** external data declaration *****************************/
uint32_t g_task_a_cnt;
uint32_t next_menu;

//volatile uint32_t ADC_VAL = 0;
//
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//	temp = ( (V25 - VSEN*ADC_VAL)/AVG_SLOPE) + 25;
//}



/********************** external functions definition ************************/
void task_a_init(void *parameters){
	//float *temp = (float *) parameters;
	task_a_display_init();
}

void task_a_update(void *parameters){
	float *temp = (float *) parameters;
	task_a_display_update(*temp);
}


static void task_a_display_init(void){
//	displayInit( DISPLAY_CONNECTION_GPIO_8BITS );
	displayInit( DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER );
//	displayInit( DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER );

	state_task_a = STATE_TASK_A_0;
	event_task_a = EVENT_TASK_A_0;
	next_menu =	0;


	init_queue_event_task_a();

	/*Print out: Initiation Menu*/
	imprimir_menu_0(next_menu);

}

static void task_a_display_update(float temp){
	/* Update Task A Counter */
	static int accumulatedDisplayTime = 0;
	g_task_a_cnt++;

//	static uint32_t then_task_a = TASK_A_DEL_INI;
//	static uint32_t now_task_a  = TASK_A_DEL_INI;
//
//	now_task_a = HAL_GetTick();
//	if ((now_task_a - then_task_a) >= TASK_A_DEL_MAX)
//	{
//		/* Reset then = now */
//		then_task_a = now_task_a;
//	}

	if( accumulatedDisplayTime >= DISPLAY_REFRESH_TIME_MS ) {

		accumulatedDisplayTime = 0;

		if (true == any_event_task_a())
		{
			event_task_a = get_event_task_a();
			switch (state_task_a)
			{
				case STATE_TASK_A_0:

					//Apagar el LED al volver al menu 0 (D2)
					g_b_event_task_b0 = true;
					g_event_task_b0= EVENT_TASK_B0_0;

					/* Si se presiona v */
					if (EVENT_TASK_A_1 == event_task_a){
						if(next_menu == 2){
							next_menu = 0;
							displayCharPositionWrite ( 0, 2);
							displayStringWrite( " " );
						}
						else{
							displayCharPositionWrite ( 0, next_menu);
							displayStringWrite( " " );
							next_menu++;
						}
						imprimir_menu_0(next_menu);

						break;
					}

					/* Si se presiona ^ */
					else if (EVENT_TASK_A_3 == event_task_a){
						if(next_menu == 0){
							next_menu = 2;
							displayCharPositionWrite ( 0, 0);
							displayStringWrite( " " );
						}
						else{
							displayCharPositionWrite ( 0, next_menu);
							displayStringWrite( " " );
							next_menu--;
						}
						imprimir_menu_0(next_menu);

						break;
					}

					/* Si se presiona > */
					else if (EVENT_TASK_A_5 == event_task_a){
						if (next_menu == 0){
							state_task_a = STATE_TASK_A_1;

							/*Se borra el > del menu 0*/
							displayCharPositionWrite ( 0, 0);
							displayStringWrite( " " );
							imprimir_menu_1();
							break;
						}
						else if(next_menu == 1){
							state_task_a = STATE_TASK_A_2;

							/*Se borra el > del menu 0*/
							displayCharPositionWrite ( 0, 1);
							displayStringWrite( " " );

							next_menu = 0;
							imprimir_menu_2(next_menu);
							break;
						}
						else if(next_menu == 2){
							state_task_a = STATE_TASK_A_3;

							/*Se borra el > del menu 0*/
							displayCharPositionWrite ( 0, 2);
							displayStringWrite( " " );

							next_menu = 0;
							imprimir_menu_3(next_menu, temp);
							break;
						}
					}

					imprimir_menu_0(next_menu);
					break;


				case STATE_TASK_A_1:

					/* Si se presiona > */
					if (EVENT_TASK_A_5 == event_task_a){
						state_task_a = STATE_TASK_A_0;

						displayCharPositionWrite ( 0, 3);
						displayStringWrite( " " );
						next_menu = 0;
						imprimir_menu_0(next_menu);

						break;
					}

					imprimir_menu_1();
					break;


				case STATE_TASK_A_2:

					/* Si se presiona v */
					if (EVENT_TASK_A_1 == event_task_a){
						if(next_menu == 1){
							displayCharPositionWrite ( 0, 1);
							displayStringWrite( " " );
							next_menu = 3;
						}
						else if(next_menu == 3){
							displayCharPositionWrite ( 0, 3);
							displayStringWrite( " " );
							next_menu = 0;
						}
						else{
							displayCharPositionWrite (0, next_menu);
							displayStringWrite( " " );
							next_menu++;
						}

						imprimir_menu_2(next_menu);
						break;
					}

					/* Si se presiona ^ */
					else if (EVENT_TASK_A_3 == event_task_a){
						if(next_menu == 0){
							displayCharPositionWrite ( 0, 0);
							displayStringWrite( " " );
							next_menu = 3;
						}
						else if(next_menu == 3){
							displayCharPositionWrite ( 0, 3);
							displayStringWrite( " " );
							next_menu = 1;
						}
						else{
							displayCharPositionWrite ( 0, next_menu);
							displayStringWrite( " " );
							next_menu--;
						}

						imprimir_menu_2(next_menu);

						break;
					}

					/* Si se presiona > */
					else if (EVENT_TASK_A_5 == event_task_a){
						if (next_menu == 0){
							/*PRENDER LED*/
							g_b_event_task_b0 = true;
							g_event_task_b0= EVENT_TASK_B0_1;

							imprimir_menu_2(next_menu);
							break;
						}
						else if (next_menu == 1){
							/*APAGAR LED*/
							g_b_event_task_b0 = true;
							g_event_task_b0= EVENT_TASK_B0_0;

							imprimir_menu_2(next_menu);
							break;
						}

						else if(next_menu == 3){
							state_task_a = STATE_TASK_A_0;

							/*Apago el LED*/
							g_b_event_task_b0 = true;
							g_event_task_b0= EVENT_TASK_B0_0;

							displayCharPositionWrite ( 0, 3);
							displayStringWrite( " " );

							next_menu = 0;
							imprimir_menu_0(next_menu);

							break;
						}
					}

					imprimir_menu_2(next_menu);
					break;


				case STATE_TASK_A_3:

					/* Si se presiona v */
					if (EVENT_TASK_A_1 == event_task_a){
						if(next_menu == 3){
							displayCharPositionWrite ( 0, 3);
							displayStringWrite( " " );

							next_menu = 0;
						}
						else{
							displayCharPositionWrite ( 0, next_menu);
							displayStringWrite( " " );
							next_menu++;
						}

						imprimir_menu_3(next_menu, temp);
						break;
					}

					/* Si se presiona ^ */
					else if (EVENT_TASK_A_3 == event_task_a){
						if(next_menu == 0){
							displayCharPositionWrite ( 0, 0);
							displayStringWrite( " " );
							next_menu = 3;
						}
						else{
							displayCharPositionWrite ( 0, next_menu);
							displayStringWrite( " " );
							next_menu--;
						}

						imprimir_menu_3(next_menu, temp);

						break;
					}


					/* Si se presiona > */
					else if (EVENT_TASK_A_5 == event_task_a){
						if(next_menu == 3){
							state_task_a = STATE_TASK_A_0;

							displayCharPositionWrite ( 0, 3);
							displayStringWrite( " " );

							next_menu = 0;
							imprimir_menu_0(next_menu);

							break;
						}
					}


//					/* Si se presiona < */
//					else if (EVENT_TASK_A_7 == event_task_a){
//						state_task_a = STATE_TASK_A_0;
//
//						displayCharPositionWrite ( 0, next_menu);
//						displayStringWrite( " " );
//
//						next_menu = 0;
//						imprimir_menu_0(next_menu);
//
//						break;
//					}
//
//					imprimir_menu_3(next_menu, temp);
//					break;


				default:

					break;
			}
		}
		else{
			switch(state_task_a){
				case STATE_TASK_A_0:
					imprimir_menu_0(next_menu);
					break;
					
				case STATE_TASK_A_1:
					imprimir_menu_1();
					break;

				case STATE_TASK_A_2:
					imprimir_menu_2(next_menu);
					break;

				case STATE_TASK_A_3:
					imprimir_menu_3(next_menu, temp);
					break;
			}
		}
	}
	else{
		accumulatedDisplayTime = accumulatedDisplayTime + SYSTEM_TIME_INCREMENT_MS;
	}
}

/********************** end of file ******************************************/
