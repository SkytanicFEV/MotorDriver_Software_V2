/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

// Enum for phases
typedef enum
{
	phase_U = 0,
	phase_V,
	phase_W,
}phase_t;

// Enum describing output state
typedef enum
{
	outputOff = 0,
	outputOn,
}outputState_t;

// Enum describing the state of each phase
typedef enum
{
	phaseOff = 0,
	phaseHigh,
	phaseLow,
}phaseState_t;

// Enum describing the phase of the waveform
typedef enum
{
	waveform_NoWaveform = 0,
	waveform_Phase1 = 1,
	waveform_Phase2,
	waveform_Phase3,
	waveform_Phase4,
	waveform_Phase5,
	waveform_Phase6,
}waveformPhase_t;
/* Private defines -----------------------------------------------------------*/

//#define HALL_EFFECT_ACTIVE


#define VOLTAGE_PHASE_U_Pin 			GPIO_PIN_0
#define VOLTAGE_PHASE_U_GPIO_Port 		GPIOA
#define VOLTAGE_PHASE_V_Pin 			GPIO_PIN_1
#define VOLTAGE_PHASE_V_GPIO_Port 		GPIOA
#define VOLTAGE_PHASE_W_Pin 			GPIO_PIN_2
#define VOLTAGE_PHASE_W_GPIO_Port 		GPIOA
#define CURRENT_PHASE_U_Pin 			GPIO_PIN_3
#define CURRENT_PHASE_U_GPIO_Port 		GPIOA
#define CURRENT_PHASE_V_Pin 			GPIO_PIN_4
#define CURRENT_PHASE_V_GPIO_Port 		GPIOA
#define CURRENT_PHASE_W_Pin 			GPIO_PIN_5
#define CURRENT_PHASE_W_GPIO_Port 		GPIOA
#define THROTTLE_Pin 					GPIO_PIN_6
#define THROTTLE_GPIO_Port 				GPIOA
#define PWM_PHASE_W_LOW_Pin 			GPIO_PIN_0
#define PWM_PHASE_W_LOW_GPIO_Port 		GPIOB
#define HALL_PHASE_U_Pin 				GPIO_PIN_12
#define HALL_PHASE_U_GPIO_Port 			GPIOB
#define HALL_PHASE_V_Pin 				GPIO_PIN_13
#define HALL_PHASE_V_GPIO_Port 			GPIOB
#define HALL_PHASE_W_Pin 				GPIO_PIN_14
#define HALL_PHASE_W_GPIO_Port 			GPIOB
#define PWM_PHASE_U_LOW_Pin 			GPIO_PIN_6
#define PWM_PHASE_U_LOW_GPIO_Port 		GPIOC
#define PWM_PHASE_V_LOW_Pin 			GPIO_PIN_7
#define PWM_PHASE_V_LOW_GPIO_Port 		GPIOC
#define PWM_PHASE_U_HIGH_Pin 			GPIO_PIN_8
#define PWM_PHASE_U_HIGH_GPIO_Port 		GPIOA
#define PWM_PHASE_V_HIGH_Pin 			GPIO_PIN_9
#define PWM_PHASE_V_HIGH_GPIO_Port 		GPIOA
#define PWM_PHASE_W_HIGH_Pin 			GPIO_PIN_10
#define PWM_PHASE_W_HIGH_GPIO_Port 		GPIOA

// If using the motor driver in single phase mode uncomment this
#define SINGLE_PHASE_MODE

/* USER CODE BEGIN Private defines */
// Frequency of the processor in KHz
#define PROC_FREQ_KHZ					40000U

// Desired frequency of the timers in KHz
#define DESIRED_TIM_FREQ_KHZ			10U

// Waveform defines
#define WAVEFORM_FREQ_HZ				30U
#define WAVEFORM_MAX_COUNT				(uint32_t) ((DESIRED_TIM_FREQ_KHZ * 1000) / WAVEFORM_FREQ_HZ)

// Pre-calculated values of PI for Sine function
#define PI 								(3.14159265)								// Value of Pi
#define TWO_PI							(6.28318531)								// 2 * Pi
#define PI_OVER_TWO						(1.57079632)								// Pi / 2
#define FOUR_OVER_PI					(1.27323954)								// 4 / Pi
#define FOUR_OVER_PI2					(.405284735)								// 4 / (Pi^2)

// Sine calculation constants
#define SINE_ACCURACY_CONSTANT			(.225)										// Constant used when increasing accuracy of sine

// Amount of dead time in between waveform polarities in ticks of TIM3
#define WAVEFORM_DEADTIME				1U
#define PWM_DEADTIME					250U

#define MAX_PULSE_WIDTH					3225U

#define NUM_ADC_CHANNEL					7U

#define THROTTLE_MAX_VALUE				(uint16_t) 4095

// Global waveform variables
uint16_t waveform_frequency;
uint32_t waveformU_switchCount;
uint32_t waveformV_switchCount;
uint32_t waveformW_switchCount;
uint32_t waveform_maxSwitches;

// Create a lookup table for sine values
float sine_lookup[WAVEFORM_MAX_COUNT];

// Switch state variables
phaseState_t phaseU_State;
phaseState_t phaseW_State;
phaseState_t phaseV_State;

// ADC buffer
uint32_t adc_buffer[NUM_ADC_CHANNEL];

// Throttle variables
uint16_t throttleValue;

// Max value of output waveform
uint32_t waveformAmplitude;

// Current phase of the output waveforms
waveformPhase_t waveformPhase;

// Current state of the output
outputState_t outputState;

/**
  * @brief Function to find the waveform phase based on the hall effect values
  * @param none
  * @retval none
  */
void FindWaveformPhase(void);

/**
  * @brief Function to start the waveforms and turn on the output
  * @param none
  * @retval none
  */
void StartWaveforms(void);

/**
  * @brief Function to stop the waveforms and turn off the output
  * @param none
  * @retval none
  */
void StopWaveforms(void);

/**
  * @brief Function to update the three waveforms
  * @param none
  * @retval none
  */
void UpdateWaveforms(void);

/**
  * @brief Use the fast sin algorithm to calculate sin(x)
  * @param x = input to sin(x)
  * @retval floating point value of result
  */
float fast_sin(float x);

/**
  * @brief Creates a lookup table for the sine wave
  * @param none
  * @retval none
  */
void Create_SineTable(void);

void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
