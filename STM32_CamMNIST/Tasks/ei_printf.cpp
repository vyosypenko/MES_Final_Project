/*
 * ei_printf.cpp
 *
 *  Created on: Jan 4, 2024
 *      Author: yosypenko
 */

#include "ei_printf.h"
#include "usbd_cdc_if.h"

#include <cstdarg>

static void vprint(const char *fmt, va_list argp)
{
    char string[200];
    if(0 < vsprintf(string, fmt, argp)) // build string
    {
        //HAL_UART_Transmit(&huart3, (uint8_t*)string, strlen(string), HAL_MAX_DELAY);
        CDC_Transmit_FS((uint8_t*)string, strlen(string));
        HAL_Delay(1);
    }
}

void ei_printf(const char *format, ...) {
    va_list myargs;
    va_start(myargs, format);
    vprint(format, myargs);
    va_end(myargs);
}
