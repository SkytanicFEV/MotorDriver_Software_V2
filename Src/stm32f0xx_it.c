/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

	int dummy;
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
	  dummy++;
	  dummy--;
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}

// Interrupt handler
void TIM3_IRQHandler(void)
{

	// Check for CC1 interrupt
	if(__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_CC1) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_CC1) !=RESET)
		{
			// Clear the interrupt
		    __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC1);
		    htim3.Channel = HAL_TIM_ACTIVE_CHANNEL_1;

//		    UpdateWaveform(phase_U);

			htim3.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
		}
	}

}

void ADC1_IRQHandler(void)
{

	HAL_ADC_IRQHandler(&hadc);
}

// External Interrupts Handler (Hall Effects)
void EXTI4_15_IRQHandler(void)
{
	uint32_t pr = EXTI->PR;
	EXTI->PR = pr;

//	if(__HAL_GPIO_EXTI_GET_IT(HALL_PHASE_U_Pin) != RESET)
//	{
//		__HAL_GPIO_EXTI_CLEAR_IT(HALL_PHASE_U_Pin);
//	}
//	if(__HAL_GPIO_EXTI_GET_IT(HALL_PHASE_V_Pin) != RESET)
//	{
//		__HAL_GPIO_EXTI_CLEAR_IT(HALL_PHASE_V_Pin);
//	}
//	if(__HAL_GPIO_EXTI_GET_IT(HALL_PHASE_V_Pin) != RESET)
//	{
//		__HAL_GPIO_EXTI_CLEAR_IT(HALL_PHASE_V_Pin);
//	}
	FindWaveformPhase();
	// Update the waveforms after determining the phase
	UpdateWaveforms();
//	HAL_GPIO_EXTI_IRQHandler(HALL_PHASE_U_Pin);
//	HAL_GPIO_EXTI_IRQHandler(HALL_PHASE_V_Pin);
//	HAL_GPIO_EXTI_IRQHandler(HALL_PHASE_W_Pin);

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
