/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "../Inc/gpio.h"
#include "../Inc/board_config.h"

void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = BUILTIN_LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BUILTIN_LED_PORT, &GPIO_InitStruct);
}

static void rc522_rst_pin_init(pin_mode_e io)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = RC522_RST_PIN;
  if (io == PIN_MODE_INPUT)
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  else if (io == PIN_MODE_OUTPUT)
  {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  }
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RC522_RST_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(RC522_RST_PORT, RC522_RST_PIN, GPIO_PIN_RESET);
}

static void rc522_rst_pin_deinit(void)
{
  HAL_GPIO_DeInit(RC522_RST_PORT, RC522_RST_PIN);
}

int rc522_rstpin_wrt(uint8_t val)
{
  rc522_rst_pin_init(PIN_MODE_OUTPUT);
  HAL_GPIO_WritePin(RC522_RST_PORT, RC522_RST_PIN, val);
  rc522_rst_pin_deinit();
  return 0;
}

int rc522_rst_pin_rd(void)
{
  rc522_rst_pin_init(PIN_MODE_INPUT);
  int result;
  result = HAL_GPIO_ReadPin(RC522_RST_PORT, RC522_RST_PIN);
  rc522_rst_pin_deinit();
  return result;
}
