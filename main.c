/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
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
RNG_HandleTypeDef hrng;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_RNG_Init(void);

static void vTaskBatterySender(void * pvParameters);
static void vTaskMotorSender(void * pvParameters);
static void vTaskErrorSender(void * pvParameter);
static void vTaskReceiver(void * pvParameters);
/* USER CODE BEGIN PFP */
typedef struct {
	double voltage;
}battery;

typedef struct {
	uint16_t rpm;
	double heat;
}motor;

typedef struct {
	char error[16]; //unknown error
}error;

QueueHandle_t xQueue1=NULL;
QueueHandle_t xQueue2=NULL;
QueueHandle_t xQueue3=NULL;
QueueSetHandle_t xQueueSet=NULL;

battery xBattery;
motor xMotor;
error xError;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void vTaskBatterySender(void * pvParameters){
	BaseType_t xStatus;
	const TickType_t xTickToWait = pdMS_TO_TICKS(1000);
	battery* myBattery = (battery*) pvParameters;
	uint32_t random;
	for(;;){
		HAL_RNG_GenerateRandomNumber(&hrng, &random);
		myBattery->voltage = 11.4 + ((double)(random%121)/100);
		xStatus = xQueueSendToBack(xQueue1, &myBattery, 0);
		if(xStatus!=pdPASS){
			HAL_UART_Transmit(&huart1, (uint8_t * )"Cannot write to queue\r\n", 23, 0xFFFF);
		}else{
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		}
		vTaskDelay(xTickToWait);
	}
}

static void vTaskMotorSender(void * pvParameters){
	BaseType_t xStatus;
	const TickType_t xTickToWait = pdMS_TO_TICKS(500);
	motor* myMotor = (motor*) pvParameters;
	uint32_t random;
	for(;;){
		HAL_RNG_GenerateRandomNumber(&hrng, &random);
		myMotor->heat = 80.0 + (double)(random%41);
		HAL_RNG_GenerateRandomNumber(&hrng, &random);
		myMotor->rpm = (random%100) + 7000;
		xStatus = xQueueSendToBack(xQueue2, &myMotor, 0);
		if(xStatus!=pdPASS){
			HAL_UART_Transmit(&huart1, (uint8_t * )"Cannot write to queue\r\n", 23, 0xFFFF);
		}else{
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		}
		vTaskDelay(xTickToWait);
	}
}

static void vTaskErrorSender(void * pvParameter){
	BaseType_t xStatus;
	const TickType_t xTickToWait = pdMS_TO_TICKS(100);
	error* myError = (error * ) pvParameter;
	strcpy(myError->error, "Unknown Error!");
	vTaskDelay(xTickToWait);
	for(;;){
		vTaskDelay(xTickToWait);
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==GPIO_PIN_SET){
			xStatus = xQueueSendToBack(xQueue3, &myError, 0);
			if(xStatus!=pdPASS){
				HAL_UART_Transmit(&huart1, (uint8_t * )"Cannot write error to queue\r\n", 29, 0xFFFF);
			}else{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
			}
		}else{
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		}
	}
}

static void vTaskReceiver(void * pvParameters){
	QueueHandle_t xQueueThatContainsData;
	BaseType_t xStatus;
	char buffer[100];
	for(;;){
		xQueueThatContainsData = (QueueHandle_t) xQueueSelectFromSet(xQueueSet, portMAX_DELAY);
		if(xQueueThatContainsData == xQueue1){
			battery *dataBattery;
			xStatus = xQueueReceive(xQueueThatContainsData, &dataBattery, 0);
			if(xStatus==pdPASS){
				uint16_t msg_len = sprintf(buffer, "Battery's voltage: %.2f \t\n", dataBattery->voltage);
				HAL_UART_Transmit(&huart1, (uint8_t * )buffer, msg_len, 0xFFFF);
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			}else{
				HAL_UART_Transmit(&huart1, (uint8_t * )"Battery's data cannot receive!\r\n", 32, 0xFFFF);
			}
		}else if(xQueueThatContainsData == xQueue2){
			motor *dataMotor;
			xStatus = xQueueReceive(xQueueThatContainsData, &dataMotor, 0);
			if(xStatus==pdPASS){
				uint16_t msg_len = sprintf(buffer, "Motor's temperature: %.2f \r\nMotor's RPM: %d \t\n", dataMotor->heat, dataMotor->rpm);
				HAL_UART_Transmit(&huart1, (uint8_t * )buffer, msg_len, 0xFFFF);
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			}else{
				HAL_UART_Transmit(&huart1, (uint8_t * )"Motor's data cannot receive!\r\n", 30, 0xFFFF);
			}
		}else{
			error *dataError;
			xStatus = xQueueReceive(xQueueThatContainsData, &dataError, 0);
			if(xStatus==pdPASS){
				uint16_t msg_len = sprintf(buffer, "Something went wrong: %s \t\n", dataError->error);
				HAL_UART_Transmit(&huart1, (uint8_t * )buffer, msg_len, 0xFFFF);
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			}else{
				HAL_UART_Transmit(&huart1, (uint8_t * )"There's an error but cannot receive error message!\r\n", 52, 0xFFFF);
			}
		}
	}
}



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
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */
  xQueue1 = xQueueCreate(5, sizeof(battery*));
  xQueue2 = xQueueCreate(5, sizeof(motor*));
  xQueue3 = xQueueCreate(1, sizeof(error*));

  xQueueSet = xQueueCreateSet(11);

  xQueueAddToSet(xQueue1, xQueueSet);
  xQueueAddToSet(xQueue2, xQueueSet);
  xQueueAddToSet(xQueue3, xQueueSet);

  if(xQueue1!=NULL && xQueue2!=NULL && xQueue3!=NULL && xQueueSet!=NULL){
	  xTaskCreate(vTaskBatterySender,
			  "batterySender",
			  512,
			  (void * )&xBattery,
			  1,
			  NULL);
	  xTaskCreate(vTaskMotorSender,
			  "motorSender",
			  512,
			  (void * )&xMotor,
			  1,
			  NULL);
	  xTaskCreate(vTaskErrorSender,
			  "errorSender",
			  512,
			  (void * )&xError,
			  3,
			  NULL);
	  xTaskCreate(vTaskReceiver,
			  "receiver",
			  512,
			  NULL,
			  2,
			  NULL);
	  vTaskStartScheduler();
  }else{
	  HAL_UART_Transmit(&huart1, (uint8_t * )"Cannot create queue\r\n", 21, 0xFFFF);
  }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
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
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, led_green_Pin|led_orange_Pin|led_red_Pin|led_blue_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : blue_button_Pin */
  GPIO_InitStruct.Pin = blue_button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(blue_button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : led_green_Pin led_orange_Pin led_red_Pin led_blue_Pin */
  GPIO_InitStruct.Pin = led_green_Pin|led_orange_Pin|led_red_Pin|led_blue_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
#ifdef USE_FULL_ASSERT
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
