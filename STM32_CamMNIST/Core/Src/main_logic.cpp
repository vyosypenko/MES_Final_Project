/*
 * main_logic.cpp
 *
 *  Created on: Dec 6, 2023
 *      Author: yosypenko
 */

#include "main_logic.h"

#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "model-parameters/model_metadata.h"

#include "ei_printf.h"
#include "camera.h"
#include "test_feature.h"
#include "button.h"
#include "led.h"

using namespace ei;

static uint8_t camera_buff[EI_CLASSIFIER_INPUT_WIDTH*EI_CLASSIFIER_INPUT_HEIGHT];


static int get_feature_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, test_features + offset, length * sizeof(float));
    return 0;
}

// Callback: fill a section of the out_ptr buffer when requested
int get_camera_data(size_t offset, size_t length, float *out_ptr)
{
    uint8_t pixel;
    float pixel_f;

    // Loop through requested pixels, copy grayscale to RGB channels
    for (size_t i = 0; i < length; i++)
    {
        pixel = (camera_buff + offset)[i];
        pixel_f = (pixel << 16) + (pixel << 8) + pixel;
        out_ptr[i] = pixel_f;
    }

    return EIDSP_OK;
}


inline void run_inference(signal_t* signal)
{
    ei_impulse_result_t result = { 0 };

    EI_IMPULSE_ERROR res = run_classifier(signal, &result, false /* debug disable */);
    ei_printf("run_classifier returned: %d\r\n", res);

    ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \r\n",
    result.timing.dsp, result.timing.classification, result.timing.anomaly);

    // print the predictions
    ei_printf("[");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf_float(result.classification[ix].value);

        if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
            ei_printf(", ");
        }
    }
    ei_printf("]\r\n\n\n");

      // human-readable predictions
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("%s: ", result.classification[ix].label);
        ei_printf_float(result.classification[ix].value);
        ei_printf("\r\n");
    }
    ei_printf("\r\n");
}


void init(void)
{
    HAL_Delay(500);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    ei_printf("\x1b[2J\x1b[;H");
    ei_printf("Welcome to Embedded ML handwritten digits recognition system\r\n\r\n");

    camera_init(camera_buff, EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT);
}

void run(void)
{
    signal_t signal;

    //ToDO: Add code to button check

    run_mode_t app_mode = M_RUN;

    switch (app_mode)
    {
    case M_DEBUG:
        ei_printf("DEBUG MODE\r\n");
        signal.total_length = sizeof(test_features) / sizeof(test_features[0]);
        signal.get_data = &get_feature_data;
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
        run_inference(&signal);
        break;

    case M_RUN:
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
        switch (get_camera_data_available_flag())
        {
        case NO:
            break;
        case YES:
            ei_printf("RUN MODE\r\n");
            signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
            signal.get_data = &get_camera_data;
            run_inference(&signal);
            reset_camera_data_available_flag();
            break;
        }
        break;

    default:
        break;
    }
}
