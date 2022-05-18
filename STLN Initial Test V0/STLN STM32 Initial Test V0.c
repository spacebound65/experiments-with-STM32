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
	char adc_msg[10];
	char heatCount_msg[20];
	char temp_msg[30] = "\r\n Printing temperature: \r\n";
	char heater_msg_1[30] = "\r\n Heater on \r\n";
	char heater_msg_2[30] = "\r\n Heater off \r\n";
	uint32_t cycleCount = 0x00000000;       // Number of times (cycles) the while loop has run (each cycle is ~ 5s), initialized to 0x00000000
	uint16_t heatCount = 0x0000; 		// Number of cycles the heater has been on (multiply this by 5s to get heater on time), initialized to 0x00000000
	uint16_t adcValue = 0x0000; 		// Value read by ADC, initialized to 0x0000
	uint32_t numCycles = 10; 				// placeholder
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

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (cycleCount < numCycles)
	  {
		  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		  // Debug for cycle start: set GPIO pin HIGH
		  ++cycleCount;

		  // Get ADC value
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		  adcValue = HAL_ADC_GetValue(&hadc1);
		  HAL_ADC_Stop(&hadc1);
		  // adcValue is between 0 and 4095 (0V to 3.3V)
		  if (adcValue < 2000)	// exact value may vary
		  	  {
			  	  ++heatCount;
			  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			  	  // write heater pin high, connect to an LED on breadboard for now
			  	  // voltage divider in use now: 34k thermistor from lab with 28k mf resistor
			  	  HAL_UART_Transmit(&huart2, (uint8_t*)heater_msg_1, strlen(heater_msg_1), HAL_MAX_DELAY);
			  	  sprintf(heatCount_msg, "%hu\r\n", heatCount);
			  	  HAL_UART_Transmit(&huart2, (uint8_t*)heatCount_msg, strlen(heatCount_msg), HAL_MAX_DELAY);
		  	  }
		  else
		  	  {
		  		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		  		  HAL_UART_Transmit(&huart2, (uint8_t*)heater_msg_2, strlen(heater_msg_2), HAL_MAX_DELAY);
		  	  }
		  	  // Debug for completion of previous step: set GPIO pin LOW
		  	  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

		  HAL_UART_Transmit(&huart2, (uint8_t*)temp_msg, strlen(temp_msg), HAL_MAX_DELAY);
		  	  // Convert to string and print
		  sprintf(adc_msg, "%hu\r\n", adcValue);
		  	  //printf(msg, "Hello world!\r\n");
		  HAL_UART_Transmit(&huart2, (uint8_t*)adc_msg, strlen(adc_msg), HAL_MAX_DELAY);
		  	  // Cycle delay, inelegant at the moment
		  HAL_Delay(5000);
	  }
	  else
	  {
		  // test complete message, blink led?
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
