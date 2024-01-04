/*
 * camera.c
 *
 *  Created on: Jan 4, 2024
 *      Author: yosypenko
 */

#include "camera.h"


static uint8_t* camera_buff_ptr;
static uint8_t camera_weight;
static uint8_t camera_height;


static volatile data_availible_t data_available = NO;

data_availible_t get_camera_data_available_flag(void)
{
	return data_available;
}

void set_camera_data_available_flag(void)
{
	data_available = YES;
}

void reset_camera_data_available_flag(void)
{
	data_available = NO;
}



void camera_init(uint8_t* camera_buff, uint8_t weight, uint8_t height)
{
	camera_buff_ptr = camera_buff;
	camera_weight = weight;
	camera_height = height;

	HAL_UART_Receive_IT(&huart3, camera_buff_ptr, (camera_weight * camera_height));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	set_camera_data_available_flag();

    // Start next reading
    HAL_UART_Receive_IT(&huart3, camera_buff_ptr, (camera_weight * camera_height));
}
