/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32g4xx_hal_pwr.h"
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

/* USER CODE BEGIN PV */
int contOn = 0, contOff = 0, contInit, contInitView = 0, contOnView = 0, contOffView = 0;
uint8_t dadoOut[256];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void writeMemo(uint8_t date, uint16_t adress, int i);
uint8_t readMemo(uint16_t adress,int i);
void EXTI15_10_IRQHandler(void);
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
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	contInitView = readMemo(0,4);
	contInit++;
    writeMemo(contInit,0,4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_GPIO_WritePin(GPIOA,Pin_7_Pin, SET);
	  HAL_GPIO_WritePin(GPIOA,Pin_8_Pin, RESET);
	  contOn++;
	  writeMemo(contOn,5,4);
	  HAL_Delay(1000);
	  HAL_GPIO_WritePin(GPIOA,Pin_7_Pin, RESET);
	  HAL_GPIO_WritePin(GPIOA,Pin_8_Pin, SET);
	  contOff++;
	  writeMemo(contOff,9,4);
	  HAL_Delay(1000);


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if ( GPIO_Pin == B1_Pin)
    {
    	  if(HAL_GPIO_ReadPin(GPIOA,B1_Pin) == 0)
    	  {
    	  	  while(HAL_GPIO_ReadPin(GPIOA,B1_Pin) == 0);
    	  	  contInitView = readMemo(0,4);
    	  	  uint8_t bufferOut1[10];
    	  	  sprintf(bufferOut1,"%d",contInitView);
    	  	  HAL_UART_Transmit(&huart1,bufferOut1,10,1);
    	  	  contOnView = readMemo(5,4);
    	  	  uint8_t bufferOut2[10];
    	  	  sprintf(bufferOut2,"%d",contOnView);
    	  	  HAL_UART_Transmit(&huart1,bufferOut2,10,1);
    	  	  contOffView = readMemo(9,4);
    	  	  uint8_t bufferOut3[10];
    	  	  sprintf(bufferOut3,"%d",contOffView);
    	  	  HAL_UART_Transmit(&huart1,bufferOut3,10,1);
    	  }
    }
}
void writeMemo(uint8_t date, uint16_t adress, int i)
{
	   uint8_t bufferOut[4];
	   uint8_t dado;


		 dado = 0x00 & dado;
		 dado = 0x06 | dado; //EnableWrite

	   bufferOut[0] = dado;
	   bufferOut[1] = adress >> 8;
	   bufferOut[2] = adress;

	   GPIOA->BSRR = (uint32_t)CS_EEPROM_Pin << 16U;//desliga o cs1
	   HAL_SPI_Transmit(&hspi1,bufferOut, 3, 1);
	   HAL_Delay(1);
		 GPIOA->BSRR = CS_EEPROM_Pin;//liga o cs1

	   dado = 0x00 & dado;
		 dado = 0x02 | dado;  //comando para escrita
	   bufferOut[0] = dado;

	   bufferOut[3] = date;
	   GPIOA->BSRR = (uint32_t)CS_EEPROM_Pin << 16U;//desliga o cs1
		 HAL_SPI_Transmit(&hspi1,bufferOut, 4, 1);
		 HAL_Delay(1);
		 GPIOA->BSRR = CS_EEPROM_Pin;//liga o cs1
		 dadoOut[i] = bufferOut[3];

}
uint8_t readMemo(uint16_t adress,int i)
{
	   HAL_Delay(1);
	   uint8_t bufferOut[4];
	   uint8_t dado;
       uint8_t resposta[2];
	   dado = 0x00 & dado;
	   dado = 0x03 | dado;  //comando para leitura
	   bufferOut[0] = dado;
	   bufferOut[1] = adress >> 8;
	   bufferOut[2] = adress;
	   GPIOA->BSRR = (uint32_t)CS_EEPROM_Pin << 16U;//desliga o cs1
	   HAL_SPI_Transmit(&hspi1,bufferOut, 3, 1);
	   HAL_SPI_Receive(&hspi1,&bufferOut[3],1,1);
	   GPIOA->BSRR = CS_EEPROM_Pin;//liga o cs1
	   resposta[0] = bufferOut[3];
	   dadoOut[i] = resposta[0];
	   return dadoOut[i];
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
