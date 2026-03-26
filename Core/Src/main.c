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
#include "stm32f1xx_hal_uart.h"
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
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t rx_data;   // received byte
uint8_t msg_f[] = "Forward\r\n";
uint8_t msg_r[] = "Right\r\n";
uint8_t msg_l[] = "Left\r\n";
uint8_t msg_b[] = "Reverse\r\n";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

void lcd_send_cmd(char cmd)
{
    HAL_GPIO_WritePin(GPIOA, RS_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOA, D4_Pin, (cmd>>4)&1);
    HAL_GPIO_WritePin(GPIOA, D5_Pin, (cmd>>5)&1);
    HAL_GPIO_WritePin(GPIOA, D6_Pin, (cmd>>6)&1);
    HAL_GPIO_WritePin(GPIOA, D7_Pin, (cmd>>7)&1);

    HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOA, D4_Pin, cmd&1);
    HAL_GPIO_WritePin(GPIOA, D5_Pin, (cmd>>1)&1);
    HAL_GPIO_WritePin(GPIOA, D6_Pin, (cmd>>2)&1);
    HAL_GPIO_WritePin(GPIOA, D7_Pin, (cmd>>3)&1);

    HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
}

void lcd_send_data(char data)
{
    HAL_GPIO_WritePin(GPIOA, RS_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOA, D4_Pin, (data>>4)&1);
    HAL_GPIO_WritePin(GPIOA, D5_Pin, (data>>5)&1);
    HAL_GPIO_WritePin(GPIOA, D6_Pin, (data>>6)&1);
    HAL_GPIO_WritePin(GPIOA, D7_Pin, (data>>7)&1);

    HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOA, D4_Pin, data&1);
    HAL_GPIO_WritePin(GPIOA, D5_Pin, (data>>1)&1);
    HAL_GPIO_WritePin(GPIOA, D6_Pin, (data>>2)&1);
    HAL_GPIO_WritePin(GPIOA, D7_Pin, (data>>3)&1);

    HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
}

	void lcd_init(void)
	{
	    HAL_Delay(50);
	    lcd_send_cmd(0x02);
	    HAL_Delay(5);
	    lcd_send_cmd(0x28);
	    HAL_Delay(1);
	    lcd_send_cmd(0x0C);
	    HAL_Delay(1);
	    lcd_send_cmd(0x06);
	    HAL_Delay(1);
	    lcd_send_cmd(0x01);
	    HAL_Delay(2);
	}


void lcd_send_string(char *str)
{
    while (*str) lcd_send_data(*str++);
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01);
    HAL_Delay(2);
}

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
  /* USER CODE BEGIN 2 */
  lcd_init();
  /* USER CODE END 2 */
  lcd_send_string("Hello STM32");
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */
//	  lcd_send_string("Hello STM32");
  while (1)
   {
     /* USER CODE END WHILE */
 	       if(HAL_UART_Receive(&huart1, &rx_data, 1, 10) == HAL_OK)
 	       {
 	    	  switch(rx_data)
 	    	  {
 	    	      case 'F': // Forward
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

 	    	          lcd_clear();
 	    	          lcd_send_string("FORWARD");

 	    	          HAL_UART_Transmit(&huart1, (uint8_t*)"Forward\r\n", 9, 100);
 	    	          break;

 	    	      case 'B': // Reverse
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

 	    	          lcd_clear();
 	    	          lcd_send_string("REVERSE");

 	    	          HAL_UART_Transmit(&huart1, (uint8_t*)"Reverse\r\n", 9, 100);
 	    	          break;

 	    	      case 'L': // Left
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

 	    	          lcd_clear();
 	    	          lcd_send_string("LEFT");

 	    	          HAL_UART_Transmit(&huart1, (uint8_t*)"Left\r\n", 6, 100);
 	    	          break;

 	    	      case 'R': // Right
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

 	    	          lcd_clear();
 	    	          lcd_send_string("RIGHT");

 	    	          HAL_UART_Transmit(&huart1, (uint8_t*)"Right\r\n", 7, 100);
 	    	          break;

 	    	      case 'S': // Stop
 	    	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_11|GPIO_PIN_10, GPIO_PIN_RESET);

 	    	          lcd_clear();
 	    	          lcd_send_string("STOP");

 	    	          HAL_UART_Transmit(&huart1, (uint8_t*)"Stop\r\n", 6, 100);
 	    	          break;
 	    	  }
 	       }
 	   }
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


	__HAL_RCC_USART1_CLK_ENABLE();

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
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
  /* USER CODE END USART1_Init 1 */
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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
