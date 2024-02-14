/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
void MX_GPIO_Init(void)
{

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_LED1_CLK_ENABLE; // ��ʼ��LED1 GPIOʱ��

    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET); // LED1��������ͣ�������LED1

    GPIO_InitStruct.Pin = LED1_PIN;              // LED1����
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // �������ģʽ
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // ��������
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // ����ģʽ
    HAL_GPIO_Init(LED1_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET); // LED1��������ߣ���Ϩ��LED1
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
