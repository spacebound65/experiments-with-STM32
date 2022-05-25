/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
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
 ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

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
	int adcThreshold;
	char adcThresh_ask[30] = "What is the target temperature?";
	char adc_msg[10];
	char heatCount_msg[20];
	char cycleCount_msg[20];
	char temp_msg[20] = "Temperature";
	char heater_msg_on[5] = "1,";
	char heater_msg_off[5] = "0,";
	char heater_msg[20] = "Heatcount";
	char cycle_msg[20] = "Cyclecount";
	uint16_t cycleCount = 0x0000;       // Number of times (cycles) the while loop has run (each cycle is ~ 5s), initialized to 0x00000000
	uint16_t heatCount = 0x0000; 		// Number of cycles the heater has been on (multiply this by 5s to get heater on time), initialized to 0x00000000
	uint16_t adcValue = 0x0000; 		// Value read by ADC, initialized to 0x0000
	uint16_t numCycles = 100; 				// placeholder
	//uint32_t R_2 = 28700;
	//uint32_t adcResistance;
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
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // ASK USER FOR THRESHOLD TEMPERATURE AND NUMBER OF CYCLES (TIME)
  //HAL_UART_Transmit(&huart2, (uint8_t*)adcThresh_ask, strlen(adcThresh_ask), HAL_MAX_DELAY);
  //scanf("%d", &adcThreshold);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (cycleCount < numCycles)
	  {
		  // Add to cycle count
		  ++cycleCount;
          // Announce cycle count, can now separate each fresh cycle by the new line sequence
          HAL_UART_Transmit(&huart2, (uint8_t*)cycle_msg, strlen(cycle_msg), HAL_MAX_DELAY);
          sprintf(cycleCount_msg, "%hu,", cycleCount);
          HAL_UART_Transmit(&huart2, (uint8_t*)cycleCount_msg, strlen(cycleCount_msg), HAL_MAX_DELAY);
		  // Get ADC value
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		  adcValue = HAL_ADC_GetValue(&hadc1);
		  HAL_ADC_Stop(&hadc1);
		  // adcValue is between 0 and 4095 (0V to 3.3V)
		  if (adcValue < adcThreshold)	// placeholder, this depends on the thermistor
		  	  {
			  	  ++heatCount;
			  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			  	  // write heater pin high, connect to high side NMOS switch
			  	  // voltage divider in use now: 34k thermistor from lab with 28k metal film resistor
			  	  HAL_UART_Transmit(&huart2, (uint8_t*)heater_msg, strlen(heater_msg), HAL_MAX_DELAY);
			  	  sprintf(heatCount_msg, "%hu,", heatCount);
			  	  HAL_UART_Transmit(&huart2, (uint8_t*)heatCount_msg, strlen(heatCount_msg), HAL_MAX_DELAY);
			  	  HAL_UART_Transmit(&huart2, (uint8_t*)heater_msg_on, strlen(heater_msg_on), HAL_MAX_DELAY);
		  	  }
		  else
		  	  {
		  		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		  		  // Port A Pin 5 connects to SCK/D13
		  		  HAL_UART_Transmit(&huart2, (uint8_t*)heater_msg, strlen(heater_msg), HAL_MAX_DELAY);
		  		  sprintf(heatCount_msg, "%hu,", heatCount);
		  		  HAL_UART_Transmit(&huart2, (uint8_t*)heatCount_msg, strlen(heatCount_msg), HAL_MAX_DELAY);
		  		  HAL_UART_Transmit(&huart2, (uint8_t*)heater_msg_off, strlen(heater_msg_off), HAL_MAX_DELAY);
		  	  }

		  HAL_UART_Transmit(&huart2, (uint8_t*)temp_msg, strlen(temp_msg), HAL_MAX_DELAY);
		  sprintf(adc_msg, "%hu,\n", adcValue);
		  HAL_UART_Transmit(&huart2, (uint8_t*)adc_msg, strlen(adc_msg), HAL_MAX_DELAY);
		  // Cycle delay
		  HAL_Delay(3000);
	  }
	  else
	  {
		  // Turn the heater OFF after the test is complete
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

		  // test complete message, blink led?
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
