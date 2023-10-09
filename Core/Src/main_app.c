
#include<string.h>
#include "main.h"
#include "stm32f4xx_hal.h"


UART_HandleTypeDef huart2;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

uint32_t counter = 0;
// Variables for software debounce
uint32_t lastButtonState = GPIO_PIN_SET;
uint32_t debounceStartTime = 0;
uint32_t debounceDelay = 50; // Adjust the debounce delay as needed

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    while (1)
    {
    	uint32_t currentButtonState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    	if (currentButtonState == GPIO_PIN_RESET && lastButtonState == GPIO_PIN_SET)
    		{
    		// Button is pressed, start debounce timer
    		debounceStartTime = HAL_GetTick();
    	    }
    	else if (currentButtonState == GPIO_PIN_SET && lastButtonState == GPIO_PIN_RESET)
    		{
    	    	// Button is released, check debounce timer
    			if ((HAL_GetTick() - debounceStartTime) > debounceDelay)
    			{
    				counter++;
    				// Transmit counter value over UART
    				char data[20];
    				snprintf(data, sizeof(data), "Counter: %d\r\n", counter);
    				HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), HAL_MAX_DELAY);
    			}
    	    }
    	// Update the button state
    	lastButtonState = currentButtonState;
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
