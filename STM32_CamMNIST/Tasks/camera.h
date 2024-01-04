/*
 * camera.h
 *
 *  Created on: Jan 4, 2024
 *      Author: yosypenko
 */

#ifndef CAMERA_H_
#define CAMERA_H_

//#ifdef __cplusplus
//extern "C"
//{
//#endif

#include "usart.h"

typedef enum  {
	NO,
	YES
} data_availible_t;

data_availible_t get_camera_data_available_flag(void);
void set_camera_data_available_flag(void);
void reset_camera_data_available_flag(void);


void camera_init(uint8_t* camera_buff, uint8_t weight, uint8_t height);

//#ifdef __cplusplus
//}
//#endif

#endif /* CAMERA_H_ */
