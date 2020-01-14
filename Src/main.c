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


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	// Initialize waveform variables to have a 60Hz waveform
	waveform_frequency = WAVEFORM_FREQ_HZ;
	waveform_maxSwitches = WAVEFORM_MAX_COUNT / 6;
	waveformU_switchCount = 0;
	waveformV_switchCount = 0;
	waveformW_switchCount = 0;

	// Initialize phases to off state
	phaseU_State = phaseOff;
	phaseV_State = phaseOff;
	phaseW_State = phaseOff;

	// Start with the output off
	outputState = outputOff;

	// Initialize the waveform amplitude
	waveformAmplitude = 0;

#ifndef HALL_EFFECT_ACTIVE
	waveformPhase = waveform_Phase1;
#endif

	// Fill the sine lookup table with values
	Create_SineTable();

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC_Init();
//	MX_I2C1_Init();
	MX_TIM1_Init();
//	MX_TIM3_Init();
//	MX_USART1_UART_Init();
	MX_TIM15_Init();

	// Start DMA transfer
//	if(HAL_ADC_Start_DMA(&hadc, adc_buffer, NUM_ADC_CHANNEL) != HAL_OK)
//	{
//		Error_Handler();
//	}

	// Start ADC interrupts
	if(HAL_ADC_Start_IT(&hadc) != HAL_OK)
	{
		Error_Handler();
	}

	// Start the waveforms
//	StartWaveforms();

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
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
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSI14CalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL10;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	// Get the current ADC conversion
	throttleValue = (ADC1->DR / 2) + (throttleValue / 2);
	waveformAmplitude = MAX_PULSE_WIDTH * throttleValue / (THROTTLE_MAX_VALUE);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if(phaseU_State == phaseHigh)
		{
	    	TIM1->CCR1 = (uint16_t) waveformAmplitude;
	    	HAL_GPIO_WritePin(PWM_PHASE_U_LOW_GPIO_Port, PWM_PHASE_U_LOW_Pin, SET);
		}

	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		if(phaseV_State == phaseHigh)
		{
	    	TIM1->CCR2 = (uint16_t) waveformAmplitude;
	    	HAL_GPIO_WritePin(PWM_PHASE_V_LOW_GPIO_Port, PWM_PHASE_V_LOW_Pin, SET);
		}

	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if(phaseW_State == phaseHigh)
		{
	    	TIM1->CCR3 = (uint16_t) waveformAmplitude;
	    	HAL_GPIO_WritePin(PWM_PHASE_W_LOW_GPIO_Port, PWM_PHASE_W_LOW_Pin, SET);
		}

	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		if(phaseU_State == phaseHigh)
		{
	    	HAL_GPIO_WritePin(PWM_PHASE_U_LOW_GPIO_Port, PWM_PHASE_U_LOW_Pin, RESET);
		}
		if(phaseV_State == phaseHigh)
		{
	    	HAL_GPIO_WritePin(PWM_PHASE_V_LOW_GPIO_Port, PWM_PHASE_V_LOW_Pin, RESET);
		}
		if(phaseW_State == phaseHigh)
		{
	    	HAL_GPIO_WritePin(PWM_PHASE_W_LOW_GPIO_Port, PWM_PHASE_W_LOW_Pin, RESET);
		}

		// Increase switch count
		waveformU_switchCount++;
		// Check to see if phase should be switched
		if(waveformU_switchCount >= waveform_maxSwitches)
		{
			waveformU_switchCount = 0;
			if(waveformPhase != waveform_Phase6){
				waveformPhase++;
			}
			else
			{
				waveformPhase = waveform_Phase1;
			}
			UpdateWaveforms();

		}

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	FindWaveformPhase();
	// Update the waveforms based on the state of the Hall effect pins
	// Check to see with pin interrupted
//	switch(GPIO_Pin)
//	{
//	case HALL_PHASE_U_Pin:
//		// Check to see if rising edge or falling edge
//		if(GPIOB->IDR & HALL_PHASE_U_Pin)
//		{
//			waveformPhase = waveform_Phase4;
//		}
//		else
//		{
//			waveformPhase = waveform_Phase1;
//		}
//		break;
//	case HALL_PHASE_V_Pin:
//		if(GPIOB->IDR & HALL_PHASE_V_Pin)
//		{
//			waveformPhase = waveform_Phase2;
//		}
//		else
//		{
//			waveformPhase = waveform_Phase5;
//		}
//		break;
//	case HALL_PHASE_W_Pin:
//		if(GPIOB->IDR & HALL_PHASE_W_Pin)
//		{
//			waveformPhase = waveform_Phase6;
//		}
//		else
//		{
//			waveformPhase = waveform_Phase3;
//		}
//		break;
//	default:
//		return;
//	}
	// Update the waveforms after determining the phase
	UpdateWaveforms();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
	while(1)
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
