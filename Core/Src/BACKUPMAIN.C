/*
 * BACKUPMAIN.C
 *
 *  Created on: 09-Oct-2023
 *      Author: hp
 */




#include<string.h>
#include "main.h"
#include "stm32f4xx_hal.h"

UART_HandleTypeDef huart2;



void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);


int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    uint32_t counter = 0;




    while (1) {
    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
    		 // Check if enough time has passed since the last button press
    		            if ((HAL_GetTick() - lastButtonPressTime) > debounceDelay) {
    		                counter++;

    		                // Button is pressed
    		                char data[20];
    		                snprintf(data, sizeof(data), "Counter: %d\r\n", counter);
    		                HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), HAL_MAX_DELAY);

    		                // Update the last button press time
    		                lastButtonPressTime = HAL_GetTick();

    		                // Wait for the button to be released
    		                while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
    		                    // Add a delay or other processing if needed
    		                }

    		                // Debounce delay
    		                HAL_Delay(100);
    		            }
    	        }
    }
}

void SystemClock_Config(void) {

}

static void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}

void Error_Handler(void) {
    while (1) {
    }
}
