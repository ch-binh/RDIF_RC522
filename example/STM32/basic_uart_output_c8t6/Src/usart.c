/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include "../Inc/usart.h"
#include "../Inc/board_config.h"

UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1_INSTANCE;
  huart1.Init.BaudRate = USART1_BUADRATE;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (uartHandle->Instance == USART1_INSTANCE)
  {
    USART1_CLK_ENABLE();
    USART1_GPIO_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = USART1_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USART1_RX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART1_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USART1_TX_PORT, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

  if (uartHandle->Instance == USART1_INSTANCE)
  {
    /* Peripheral clock disable */
    USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(USART1_RX_PORT, USART1_RX_PIN);
    HAL_GPIO_DeInit(USART1_TX_PORT, USART1_TX_PIN);
    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
}
#define UART_BUFFER_SIZE 64
uint8_t uart_rx_buffer[UART_BUFFER_SIZE]; // Buffer for received data

void uart_send(uint8_t *data, uint16_t size)
{
  HAL_UART_Transmit(&huart1, data, size, HAL_MAX_DELAY);
}

void uart_read(void)
{
  HAL_UART_Receive_IT(&huart1, uart_rx_buffer, UART_BUFFER_SIZE);
}

// Callback function for UART reception complete interrupt
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  { // Ensure it's the correct UART instance
    // Process received data (for example, send it back as an echo)
    // uart_send(uart_rx_buffer, UART_BUFFER_SIZE);

    // Restart UART reception (needed for continuous reception)
    HAL_UART_Receive_IT(&huart1, uart_rx_buffer, UART_BUFFER_SIZE);
  }
}