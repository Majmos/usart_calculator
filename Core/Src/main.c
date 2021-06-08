/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile double number1 = 0, number2 = 0, result = 0;
char text[100];
char input[64];
uint16_t size;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

HAL_StatusTypeDef uart_send(char* text) {
	return HAL_UART_Transmit(&huart1, (uint8_t*)text, strlen(text), 1000);
}

HAL_StatusTypeDef uart_recv(char* text) {
	HAL_StatusTypeDef status;
	for(int i = 0; i < 64; ++i) {
		char c;
		status = HAL_UART_Receive(&huart1, (uint8_t*)&c, 1, 200000);
		if(status == HAL_TIMEOUT) break;
		if(c == '\r') {
			input[i] = '\n';
			uart_send("\n");
			break;
		}
		HAL_UART_Transmit(&huart1, (uint8_t*)&c, 1, 20);
		input[i] = c;
	}
	return status;
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == GPIO_PIN_0){
		uart_send("Wpisz liczbe 1: ");
		uart_recv(input);
		sscanf(input, "%lf", &number1);
		sprintf(text, "liczba_1 = %lf\n", number1);
		uart_send(text);
		//size = sprintf( (char*) text, "Wprowadz dwie liczby, a nastepnie wybierz dzialanie\n\rPin 4: '+'; Pin 5: '-'; Pin 6: '*'; Pin 7: '/';\n\r");
	} else if(GPIO_Pin == GPIO_PIN_1){
		uart_send("Wpisz liczbe 2: ");
		uart_recv(input);
		sscanf(input, "%lf", &number2);
		sprintf(text, "liczba_2 = %lf\n", number2);
		uart_send(text);
	} else if (GPIO_Pin == GPIO_PIN_2) {
		number1 = result;
		sprintf(text, "liczba_1 = %lf\n", number1);
		uart_send(text);
	} else if (GPIO_Pin == GPIO_PIN_3) {
		number2 = result;
		sprintf(text, "liczba_2 = %lf\n", number2);
		uart_send(text);
	} else if(GPIO_Pin == GPIO_PIN_4){
		result = number1 + number2;
		size = sprintf(text, "%lf + %lf = %lf\n\r", number1, number2, result);
		HAL_UART_Transmit_IT(&huart1, (uint8_t*) text, size);
	} else if(GPIO_Pin == GPIO_PIN_5) {
		result = number1 - number2;
		size = sprintf(text, "%lf - %lf = %lf\n\r", number1, number2, result);
		HAL_UART_Transmit_IT(&huart1, (uint8_t*) text, size);
	} else if(GPIO_Pin == GPIO_PIN_6) {
		result = number1 * number2;
		size = sprintf(text, "%lf * %lf = %lf\n\r", number1, number2, result);
		HAL_UART_Transmit_IT(&huart1, (uint8_t*) text, size);
	} else if(GPIO_Pin == GPIO_PIN_7) {
		if(number2 != 0){
			result = number1 / number2;
			size = sprintf(text, "%lf / %lf = %lf\n\r", number1, number2, number1 / number2);
		} else size = sprintf(text, "Nie wolno dzielic przez 0\n\r");
		HAL_UART_Transmit_IT(&huart1, (uint8_t*) text, size);
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
