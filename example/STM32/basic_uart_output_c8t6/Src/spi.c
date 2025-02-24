/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    spi.c
 * @brief   This file provides code for the configuration
 *          of the SPI instances.
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
#include "../Inc/spi.h"
#include "../Inc/board_config.h"

SPI_HandleTypeDef hspi1;

void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1_INSTANCE;
  hspi1.Init.Mode = SPI1_MODE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI1_BUARDRATEPRESCALER;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI1_CRC;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (spiHandle->Instance == SPI1)
  {
    /* SPI1 clock enable */
    SPI1_CLK_ENABLE();
    SPI1_GPIO_CLK_ENABLE();

    /**SPI1 GPIO Configuration
    PA2     ------> SPI1_CS
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    /*Configure GPIO pin : PA2 */
    GPIO_InitStruct.Pin = RC522_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RC522_CS_GPIO, &GPIO_InitStruct);
    HAL_GPIO_WritePin(RC522_CS_GPIO, RC522_CS_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = SPI1_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI1_SCK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SPI1_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SPI1_MISO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SPI1_MISO_PORT, &GPIO_InitStruct);

    /* SPI1 interrupt Init */
    HAL_NVIC_SetPriority(SPI1_INTERRUPT, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_INTERRUPT);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{

  if (spiHandle->Instance == SPI1)
  {
    /* Peripheral clock disable */
    SPI1_CLK_DISABLE();

    HAL_GPIO_DeInit(RC522_CS_GPIO, RC522_CS_PIN);
    HAL_GPIO_DeInit(SPI1_SCK_PORT, SPI1_SCK_PIN);
    HAL_GPIO_DeInit(SPI1_MISO_PORT, SPI1_MISO_PIN);
    HAL_GPIO_DeInit(SPI1_MOSI_PORT, SPI1_MOSI_PIN);

    /* SPI1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(SPI1_INTERRUPT);
  }
}




int stm32_spi_write(uint8_t reg, uint8_t *data, uint8_t size)
{
    uint8_t txBuffer[size + 1];
    
    // First byte: Register address (MSB = 0 for write)
    txBuffer[0] = (reg << 1) & 0x7E; // Ensure MSB is 0

    // Copy data to send buffer
    for (uint8_t i = 0; i < size; i++)
    {
        txBuffer[i + 1] = data[i];
    }

    HAL_GPIO_WritePin(RC522_CS_GPIO, RC522_CS_PIN, GPIO_PIN_RESET);

    if (HAL_SPI_Transmit(&hspi1, txBuffer, size + 1, HAL_MAX_DELAY) != HAL_OK)
    {
        HAL_GPIO_WritePin(RC522_CS_GPIO, RC522_CS_PIN, GPIO_PIN_SET);
        return -1; // Error
    }

    // Pull CS high
    HAL_GPIO_WritePin(RC522_CS_GPIO, RC522_CS_PIN, GPIO_PIN_SET);

    return 0; // Success
}


int stm32_spi_read(uint8_t reg, uint8_t *data, uint8_t size)
{
    uint8_t txBuffer = ((reg << 1) & 0x7E) | 0x80; // MSB = 1 for read
    uint8_t rxBuffer[size];

    // Pull CS low
    HAL_GPIO_WritePin(RC522_CS_GPIO, RC522_CS_PIN, GPIO_PIN_RESET);
    
    // Send register address
    if (HAL_SPI_Transmit(&hspi1, &txBuffer, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        HAL_GPIO_WritePin(RC522_CS_GPIO, RC522_CS_PIN, GPIO_PIN_SET);
        return -1; // Error
    }

    // Receive data
    if (HAL_SPI_Receive(&hspi1, rxBuffer, size, HAL_MAX_DELAY) != HAL_OK)
    {
        HAL_GPIO_WritePin(RC522_CS_GPIO, RC522_CS_PIN, GPIO_PIN_SET);
        return -1; // Error
    }

    // Pull CS high
    HAL_GPIO_WritePin(RC522_CS_GPIO, RC522_CS_PIN, GPIO_PIN_SET);

    // Copy received data
    for (uint8_t i = 0; i < size; i++)
    {
        data[i] = rxBuffer[i];
    }

    return 0; // Success
}