/*
 * button.cpp
 *
 *  Created on: Jan 4, 2024
 *      Author: yosypenko
 */

#include "button.h"
#include "tim.h"
#include "gpio.h"

static volatile button_state_t mode_button;

static uint8_t current_button_st = RELEASED;
static uint8_t prev_button_st  = RELEASED;

void mode_button_init(void)
{
	HAL_TIM_Base_Start_IT(&htim14);

	mode_button = RELEASED;
}

button_state_t get_mode_button_state(void)
{
	return mode_button;
}


void reset_mode_button_state(void)
{
	mode_button = RELEASED;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(MODE_BNT_GPIO_Port, MODE_BNT_Pin))
		current_button_st = PRESSED;
	else
	    current_button_st = RELEASED;

	if ((current_button_st == PRESSED) && (prev_button_st == RELEASED))
	{
		mode_button = PRESSED;
	}

	prev_button_st = current_button_st;
}
