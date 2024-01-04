/*
 * main_logic.h
 *
 *  Created on: Dec 6, 2023
 *      Author: yosypenko
 */

#ifndef INC_MAIN_LOGIC_H_
#define INC_MAIN_LOGIC_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
	M_DEBUG,
	M_RUN
} run_mode_t;


void init(void);
void run(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_MAIN_LOGIC_H_ */


