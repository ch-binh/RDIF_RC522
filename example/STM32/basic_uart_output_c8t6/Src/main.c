/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "sysconfig.h"
#include "board_config.h"

#include "../../../../inc/rc522.h"
#include "../../../../inc/rc522_spi.h"


void rc522_init(void) {
  rc522_ops_t ops = {
      .spi_write = stm32_spi_write, // replace this
      .spi_read = stm32_spi_read  // replace this
  };
  rc522_set_spi_spec(&ops);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();

  rc522_init();
  while (1)
  {
    HAL_GPIO_TogglePin(BUILTIN_LED_PORT, BUILTIN_LED_PIN);
    rc522_get_chip_id();
    HAL_Delay(1000);
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}