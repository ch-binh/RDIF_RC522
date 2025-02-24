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
#include "../../../../inc/rc522_cfg.h"

void rc522_init(void)
{
  rc522_ops_t ops = {
      .spi_write = stm32_spi_write,
      .spi_read = stm32_spi_read,
      .rst_pin_write = rc522_rstpin_wrt,
      .rst_pin_read = rc522_rst_pin_rd
  };
  rc522_set_spi_spec(&ops);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  //MX_USART1_UART_Init();

  HAL_Delay(500);

  rc522_init();
  
  rc522_init_reg();
  rc522_pcd_init();

  rc522_get_chip_sts();

  uint8_t result = 0;
  while (1)
  {
    HAL_GPIO_TogglePin(BUILTIN_LED_PORT, BUILTIN_LED_PIN);
    result = rc522_get_chip_id();
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