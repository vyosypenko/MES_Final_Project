/*
 * button.h
 *
 *  Created on: Jan 4, 2024
 *      Author: yosypenko
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"
#include "gpio.h"

typedef enum  {
	RELEASED,
	PRESSED
} button_state_t;

void mode_button_init(void);
button_state_t get_mode_button_state(void);
void reset_mode_button_state(void);

#endif /* BUTTON_H_ */
