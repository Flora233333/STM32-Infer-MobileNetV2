
/**
 ******************************************************************************
 * @file    app_x-cube-ai.c
 * @author  X-CUBE-AI C code generator
 * @brief   AI program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/*
 * Description
 *   v1.0 - Minimum template to show how to use the Embedded Client API
 *          model. Only one input and one output is supported. All
 *          memory resources are allocated statically (AI_NETWORK_XX, defines
 *          are used).
 *          Re-target of the printf function is out-of-scope.
 *   v2.0 - add multiple IO and/or multiple heap support
 *
 *   For more information, see the embeded documentation:
 *
 *       [1] %X_CUBE_AI_DIR%/Documentation/index.html
 *
 *   X_CUBE_AI_DIR indicates the location where the X-CUBE-AI pack is installed
 *   typical : C:\Users\<user_name>\STM32Cube\Repository\STMicroelectronics\X-CUBE-AI\7.1.0
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /* Includes ------------------------------------------------------------------*/

#if defined(__ICCARM__)
#elif defined(__CC_ARM) || (__GNUC__)
#endif

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "app_x-cube-ai.h"
#include "main.h"
#include "ai_datatypes_defines.h"
#include "network.h"
#include "network_data.h"

    /* USER CODE BEGIN includes */
    /* USER CODE END includes */

    /* IO buffers ----------------------------------------------------------------*/

#if !defined(AI_NETWORK_INPUTS_IN_ACTIVATIONS)
    AI_ALIGNED(4)
    ai_i8 data_in_1[AI_NETWORK_IN_1_SIZE_BYTES];
    ai_i8 *data_ins[AI_NETWORK_IN_NUM] = {
        data_in_1};
#else
ai_i8 *data_ins[AI_NETWORK_IN_NUM] = {
    NULL};
#endif

#if !defined(AI_NETWORK_OUTPUTS_IN_ACTIVATIONS)
    AI_ALIGNED(4)
    ai_i8 data_out_1[AI_NETWORK_OUT_1_SIZE_BYTES];
    ai_i8 *data_outs[AI_NETWORK_OUT_NUM] = {
        data_out_1};
#else
ai_i8 *data_outs[AI_NETWORK_OUT_NUM] = {
    NULL};
#endif

    extern ai_u8 input_data[AI_NETWORK_IN_1_SIZE_BYTES];
    extern ai_float output_data[AI_NETWORK_OUT_1_SIZE];
    extern __IO uint16_t detect_img_data[img_size * img_size];
    extern char display_str[30];
    extern char conf_str[10];

    /* Activations buffers -------------------------------------------------------*/

    AI_ALIGNED(32)
    static uint8_t pool0[AI_NETWORK_DATA_ACTIVATION_1_SIZE];

    ai_handle data_activations0[] = {pool0};

    /* AI objects ----------------------------------------------------------------*/

    static ai_handle network = AI_HANDLE_NULL;

    static ai_buffer *ai_input;
    static ai_buffer *ai_output;

    uint8_t pixel_conv_lut[256];

    const char * class_names[] = {
        "Ambulance",
        "Armored_car",
        "Spontoon",
        "Telescope",
        "Bulletproof_vest",
        "Dagger",
        "Explosive",
        "Fire_axe",
        "Fire_engine",
        "First_aid_kit",
        "Flashlight",
        "Gun",
        "Helmet",
        "Motorcycle",
        "Intercom"
    };

    static void ai_log_err(const ai_error err, const char *fct)
    {
        /* USER CODE BEGIN log */
        if (fct)
            printf("TEMPLATE - Error (%s) - type=0x%02x code=0x%02x\r\n", fct,
                   err.type, err.code);
        else
            printf("TEMPLATE - Error - type=0x%02x code=0x%02x\r\n", err.type, err.code);

        do
        {
        } while (1);
        /* USER CODE END log */
    }

    ai_float ai_get_input_scale(void)
    {
        return AI_BUFFER_META_INFO_INTQ_GET_SCALE(ai_input[0].meta_info, 0);
    }

    ai_i32 ai_get_input_zero_point(void)
    {
        return AI_BUFFER_META_INFO_INTQ_GET_ZEROPOINT(ai_input[0].meta_info, 0);
    }

    uint32_t ai_get_input_quantization_scheme(void)
    {
        ai_float scale = ai_get_input_scale();

        ai_buffer_format fmt = AI_BUFFER_FORMAT(&ai_input[0]);
        ai_size sign = AI_BUFFER_FMT_GET_SIGN(fmt);

        printf("scale: %f, sign: %d\r\n", scale, sign);
        printf("AI_BUFFER_FMT_GET_SIGN(fmt): %d\r\n", AI_BUFFER_FMT_GET_SIGN(fmt));
        printf("AI_BUFFER_FMT_GET_TYPE(fmt): %d\r\n", AI_BUFFER_FMT_GET_TYPE(fmt));

        if (scale == 0)
        {
            return AI_FXP_Q;
        }
        else
        {
            if (sign == 0)
            {
                return AI_UINT_Q;
            }
            else
            {
                return AI_SINT_Q;
            }
        }
    }

    static void Precompute_8IntU(float scale, int32_t zp, float scale_prepro, int32_t zp_prepro)
    {
        uint8_t *lut = pixel_conv_lut;

        for (int32_t i = 0; i < 256; i++)
        {
            float tmp = (i - zp_prepro) * scale_prepro;
            *(lut + i) = _CLAMP(zp + _ROUND(tmp * scale, int32_t), 0, 255, uint8_t);
        }
    }

    static void Compute_pix_conv_tab()
    {
        float prepro_scale = 255.0f;
        int32_t prepro_zp = 0;

        /*Retrieve the quantization scheme used to quantize the neural network*/
        switch (ai_get_input_quantization_scheme())
        {
        case AI_FXP_Q:
            /*Pixel value convertion and normalisation*/
            // Precompute_8FXP(lut, ai_get_input_quantized_format());
            break;

        case AI_UINT_Q:
            /*Pixel value convertion and normalisation*/
            Precompute_8IntU(ai_get_input_scale(), ai_get_input_zero_point(), prepro_scale, prepro_zp);
            break;

        case AI_SINT_Q:
            /*Pixel value convertion and normalisation*/
            // Precompute_8IntS(lut, ai_get_input_scale(), ai_get_input_zero_point(), prepro_scale, prepro_zp);
            break;

        default:
            break;
        }
    }

    static int ai_boostrap(ai_handle *act_addr)
    {
        ai_error err;

        /* Create and initialize an instance of the model */
        err = ai_network_create_and_init(&network, act_addr, NULL);
        if (err.type != AI_ERROR_NONE)
        {
            ai_log_err(err, "ai_network_create_and_init");
            return -1;
        }

        ai_input = ai_network_inputs_get(network, NULL);
        ai_output = ai_network_outputs_get(network, NULL);

        uint8_t qte_scheme = ai_get_input_quantization_scheme();
        printf("Quantization scheme: %d\r\n", qte_scheme);

        Compute_pix_conv_tab(); 
        printf("Pixel conversion LUT Finished\r\n");


        return 0;
    }

    int ai_run(ai_u8 *data_in, ai_float *data_out)
    {
        ai_i32 batch;

        /* Update IO handlers with the data payload */
        ai_input[0].data = AI_HANDLE_PTR(data_in);
        ai_output[0].data = AI_HANDLE_PTR(data_out);

        batch = ai_network_run(network, ai_input, ai_output);
        if (batch != 1)
        {
            ai_log_err(ai_network_get_error(network),
                       "ai_network_run");
            return -1;
        }

        return 0;
    }

    /* USER CODE BEGIN 2 */
    int acquire_and_process_data()
    {
        // detect_img_data RGB565 to input_data RGB888
        uint16_t *p = (uint16_t *)detect_img_data;

        for (int i = 0; i < img_size * img_size; i++)
        {
            uint16_t pixel = *p++;
            uint8_t r = (pixel >> 11) & 0x1F;
            uint8_t g = (pixel >> 5) & 0x3F;
            uint8_t b = pixel & 0x1F;

            // input_data[i * 3] = pixel_conv_lut[r]; // 理论上应该是这样的，但是实际上如果input是uint8_t类型的话，就没必要了，直接赋值
            // input_data[i * 3 + 1] = pixel_conv_lut[g];
            // input_data[i * 3 + 2] = pixel_conv_lut[b];

            input_data[i * 3] = r;
            input_data[i * 3 + 1] = g;
            input_data[i * 3 + 2] = b;
        }

        return 0;
    }

    int post_process()
    {
        float max_conf = -1;
        float tmp = -1;

        for (uint8_t i = 0; i < AI_NETWORK_OUT_1_SIZE; i++)
        {

            if (max_conf < output_data[i])
            {
                tmp = i;
                max_conf = output_data[i];
            }
        }

        sprintf(display_str, "%s", class_names[(int)tmp]);
        sprintf(conf_str, "%.2f%%", max_conf * 100);
        
        return (int)tmp;
    }
    /* USER CODE END 2 */

    /* Entry points --------------------------------------------------------------*/

    void MX_X_CUBE_AI_Init(void)
    {
        /* USER CODE BEGIN 5 */
        printf("\r\nAI - Initialization\r\n");

        ai_boostrap(data_activations0);
        /* USER CODE END 5 */
    }

    int MX_X_CUBE_AI_Process(void)
    {
        /* USER CODE BEGIN 6 */
        int res = -1;
        int class = -1;


        if (network)
        {
            /* 1 - acquire and pre-process input data */
            res = acquire_and_process_data();
            /* 2 - process the data - call inference engine */
            if (res == 0)
                res = ai_run(input_data, output_data);
            /* 3- post-process the predictions */
            if (res == 0)
                class = post_process();
        }

        if (res)
        {
            ai_error err = {AI_ERROR_INVALID_STATE, AI_ERROR_CODE_NETWORK};
            ai_log_err(err, "Process has FAILED");
        }

        return class;
        /* USER CODE END 6 */
    }
#ifdef __cplusplus
}
#endif
