/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.h
 * @brief   This file contains all the function prototypes for
 *          the gpio.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef enum
{
  PIN_MODE_OUTPUT,
  PIN_MODE_INPUT
} pin_mode_e;

void MX_GPIO_Init(void);

int rc522_rstpin_wrt(uint8_t val);
int rc522_rst_pin_rd(void);

#endif /*__ GPIO_H__ */
