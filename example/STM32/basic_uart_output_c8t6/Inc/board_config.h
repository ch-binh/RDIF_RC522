#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

/*======================| GPIO |======================*/
#define BUILTIN_LED_PORT GPIOC
#define BUILTIN_LED_PIN GPIO_PIN_13

/*======================| SPI |======================*/
#define SPI1_INSTANCE SPI1
#define SPI1_MODE SPI_MODE_MASTER
#define SPI1_BUARDRATEPRESCALER SPI_BAUDRATEPRESCALER_16
#define SPI1_CRC SPI_CRCCALCULATION_DISABLE

#define SPI1_SCK_PORT GPIOA
#define SPI1_SCK_PIN GPIO_PIN_5
#define SPI1_MISO_PORT GPIOA
#define SPI1_MISO_PIN GPIO_PIN_6
#define SPI1_MOSI_PORT GPIOA
#define SPI1_MOSI_PIN GPIO_PIN_7

#define SPI1_CLK_ENABLE() __HAL_RCC_SPI1_CLK_ENABLE()
#define SPI1_CLK_DISABLE() __HAL_RCC_SPI1_CLK_DISABLE()

#define SPI1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()

#define SPI1_INTERRUPT SPI1_IRQn
/*======================| UART |======================*/

#define USART1_INSTANCE USART1
#define USART1_BUADRATE 115200

#define USART1_RX_PORT GPIOA
#define USART1_RX_PIN GPIO_PIN_9
#define USART1_TX_PORT GPIOA
#define USART1_TX_PIN GPIO_PIN_10

#define USART1_IRQn USART1_IRQn

#define USART1_CLK_ENABLE()  __HAL_RCC_USART1_CLK_ENABLE()
#define USART1_CLK_DISABLE() __HAL_RCC_USART1_CLK_DISABLE()
#define USART1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()

#endif // BOARD_CONFIG_H