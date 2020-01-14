/*
 * control.c
 *
 *  Created on: Dec 5, 2019
 *      Author: alex
 */

#include "main.h"

/**
  * @brief Function to find the waveform phase based on the hall effect values
  * @param none
  * @retval none
  */
void FindWaveformPhase(void)
{
	if(GPIOB->IDR & HALL_PHASE_U_Pin)
	{
		if(GPIOB->IDR & HALL_PHASE_V_Pin)
		{
			waveformPhase = waveform_Phase4;
		}
		else if(GPIOB->IDR & HALL_PHASE_W_Pin)
		{
			waveformPhase = waveform_Phase6;

		}
		else
		{
			waveformPhase = waveform_Phase5;
		}
	}
	else
	{
		if(GPIOB->IDR & HALL_PHASE_V_Pin)
		{
			if(GPIOB->IDR & HALL_PHASE_W_Pin)
			{
				waveformPhase = waveform_Phase2;

			}
			else
			{
				waveformPhase = waveform_Phase3;

			}
		}
		else
		{
			if(GPIOB->IDR & HALL_PHASE_W_Pin)
			{
				waveformPhase = waveform_Phase1;
			}
			// If none of the hall effect outputs are active then there is a problem
			else
			{
				waveformPhase = waveform_NoWaveform;
			}

		}
	}
}

/**
  * @brief Function to start the waveforms and turn on the output
  * @param none
  * @retval none
  */
void StartWaveforms(void)
{
	// Look at the hall effects to determine the phase
	FindWaveformPhase();
	// If the waveform phase is valid, then turn on output
//	if(waveformPhase != waveform_NoWaveform)
//	{
		// Update the waveforms based on hall effect values
		UpdateWaveforms();
		// Update output status
		outputState = outputOn;
//	}


}

/**
  * @brief Function to stop the waveforms and turn off the output
  * @param none
  * @retval none
  */
void StopWaveforms(void)
{
	// Turn off all high side
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;

	// Turn on all low side
	HAL_GPIO_WritePin(PWM_PHASE_U_LOW_GPIO_Port, PWM_PHASE_U_LOW_Pin, RESET);
	HAL_GPIO_WritePin(PWM_PHASE_V_LOW_GPIO_Port, PWM_PHASE_V_LOW_Pin, RESET);
	HAL_GPIO_WritePin(PWM_PHASE_W_LOW_GPIO_Port, PWM_PHASE_W_LOW_Pin, RESET);

	// Update phase states
	phaseU_State = phaseLow;
	phaseV_State = phaseLow;
	phaseW_State = phaseLow;

	// Update output status
	outputState = outputOff;

}

/**
  * @brief Function to update the three waveforms
  * @param none
  * @retval none
  */
void UpdateWaveforms(void)
{
	switch(waveformPhase)
	{
	case waveform_Phase1:
		// Turn on phase U high side
		TIM1->CCR1 = waveformAmplitude;
		// Turn off phase U low side (will get turned back on in interrupts)
    	HAL_GPIO_WritePin(PWM_PHASE_U_LOW_GPIO_Port, PWM_PHASE_U_LOW_Pin, RESET);
    	// Update phase state
    	phaseU_State = phaseHigh;

    	// Turn off phase V high side
    	TIM1->CCR2 = 0;
    	// Turn on phase V low side
    	HAL_GPIO_WritePin(PWM_PHASE_V_LOW_GPIO_Port, PWM_PHASE_V_LOW_Pin, SET);
    	// Update phase state
    	phaseV_State = phaseLow;

    	// Turn off phase W high side
    	TIM1->CCR3 = 0;
    	// Turn off phase W low side
    	HAL_GPIO_WritePin(PWM_PHASE_W_LOW_GPIO_Port, PWM_PHASE_W_LOW_Pin, RESET);
    	// Update phase state
    	phaseW_State = phaseOff;
		return;
	case waveform_Phase2:
		// Turn off phase U high side
		TIM1->CCR1 = 0;
		// Turn off phase U low side
    	HAL_GPIO_WritePin(PWM_PHASE_U_LOW_GPIO_Port, PWM_PHASE_U_LOW_Pin, RESET);
    	// Update phase state
    	phaseU_State = phaseOff;

    	// Turn off phase V high side
    	TIM1->CCR2 = 0;
    	// Turn on phase V low side
    	HAL_GPIO_WritePin(PWM_PHASE_V_LOW_GPIO_Port, PWM_PHASE_V_LOW_Pin, SET);
    	// Update phase state
    	phaseV_State = phaseLow;

    	// Turn on phase W high side
    	TIM1->CCR3 = waveformAmplitude;
    	// Turn off phase W low side (will get turned back on in interrupts)
    	HAL_GPIO_WritePin(PWM_PHASE_W_LOW_GPIO_Port, PWM_PHASE_W_LOW_Pin, RESET);
    	// Update phase state
    	phaseW_State = phaseHigh;
		return;
	case waveform_Phase3:
		// Turn off phase U high side
		TIM1->CCR1 = 0;
		// Turn on phase U low side
    	HAL_GPIO_WritePin(PWM_PHASE_U_LOW_GPIO_Port, PWM_PHASE_U_LOW_Pin, SET);
    	// Update phase state
    	phaseU_State = phaseLow;

    	// Turn off phase V high side
    	TIM1->CCR2 = 0;
    	// Turn off phase V low side
    	HAL_GPIO_WritePin(PWM_PHASE_V_LOW_GPIO_Port, PWM_PHASE_V_LOW_Pin, RESET);
    	// Update phase state
    	phaseV_State = phaseOff;

    	// Turn on phase W high side
    	TIM1->CCR3 = waveformAmplitude;
    	// Turn off phase W low side (will get turned back on in interrupts)
    	HAL_GPIO_WritePin(PWM_PHASE_W_LOW_GPIO_Port, PWM_PHASE_W_LOW_Pin, RESET);
    	// Update phase state
    	phaseW_State = phaseHigh;
		return;
	case waveform_Phase4:
		// Turn off phase U high side
		TIM1->CCR1 = 0;
		// Turn on phase U low side
    	HAL_GPIO_WritePin(PWM_PHASE_U_LOW_GPIO_Port, PWM_PHASE_U_LOW_Pin, SET);
    	// Update phase state
    	phaseU_State = phaseLow;

    	// Turn on phase V high side
    	TIM1->CCR2 = waveformAmplitude;
    	// Turn on phase V low side (will get turned back on in interrupts)
    	HAL_GPIO_WritePin(PWM_PHASE_V_LOW_GPIO_Port, PWM_PHASE_V_LOW_Pin, RESET);
    	// Update phase state
    	phaseV_State = phaseHigh;

    	// Turn off phase W high side
    	TIM1->CCR3 = 0;
    	// Turn off phase W low side
    	HAL_GPIO_WritePin(PWM_PHASE_W_LOW_GPIO_Port, PWM_PHASE_W_LOW_Pin, RESET);
    	// Update phase state
    	phaseW_State = phaseOff;
		return;
	case waveform_Phase5:
		// Turn off phase U high side
		TIM1->CCR1 = 0;
		// Turn off phase U low side
    	HAL_GPIO_WritePin(PWM_PHASE_U_LOW_GPIO_Port, PWM_PHASE_U_LOW_Pin, RESET);
    	// Update phase state
    	phaseU_State = phaseOff;

    	// Turn on phase V high side
    	TIM1->CCR2 = waveformAmplitude;
    	// Turn on phase V low side (will get turned back on in interrupts)
    	HAL_GPIO_WritePin(PWM_PHASE_V_LOW_GPIO_Port, PWM_PHASE_V_LOW_Pin, RESET);
    	// Update phase state
    	phaseV_State = phaseHigh;

    	// Turn off phase W high side
    	TIM1->CCR3 = 0;
    	// Turn on phase W low side
    	HAL_GPIO_WritePin(PWM_PHASE_W_LOW_GPIO_Port, PWM_PHASE_W_LOW_Pin, SET);
    	// Update phase state
    	phaseW_State = phaseLow;
		return;
	case waveform_Phase6:
		// Turn on phase U high side
		TIM1->CCR1 = waveformAmplitude;
		// Turn off phase U low side  (will get turned back on in interrupts)
    	HAL_GPIO_WritePin(PWM_PHASE_U_LOW_GPIO_Port, PWM_PHASE_U_LOW_Pin, RESET);
    	// Update phase state
    	phaseU_State = phaseHigh;

    	// Turn off phase V high side
    	TIM1->CCR2 = 0;
    	// Turn on phase V low side
    	HAL_GPIO_WritePin(PWM_PHASE_V_LOW_GPIO_Port, PWM_PHASE_V_LOW_Pin, RESET);
    	// Update phase state
    	phaseV_State = phaseOff;

    	// Turn off phase W high side
    	TIM1->CCR3 = 0;
    	// Turn on phase W low side
    	HAL_GPIO_WritePin(PWM_PHASE_W_LOW_GPIO_Port, PWM_PHASE_W_LOW_Pin, SET);
    	// Update phase state
    	phaseW_State = phaseLow;
		return;
	default:
		// If none of the phases are detected then turn off the output
		StopWaveforms();

		return;
	}

}


/**
  * @brief Use the fast sin algorithm to calculate sin(x)
  * @param x = input to sin(x)
  * @retval floating point value of result
  */
float fast_sin(float x)
{
	// If a value outside of -Pi - Pi is given, correct it
	if(x < -(PI))
	{
		x += TWO_PI;
	}
	else if(x > PI)
	{
		x -= TWO_PI;
	}

	// Compute the approximation of sine
	float sine;
	// Compute the brunt of the calculation using sin(x) ~ (4/pi)x +- (4/(pi^2))x^2
	if(x < 0)
	{
		sine = (FOUR_OVER_PI * x) + (FOUR_OVER_PI2 * x * x);
	}
	else
	{
		sine = (FOUR_OVER_PI * x) - (FOUR_OVER_PI2 * x * x);
	}
	// Make the calculation more accurate by = .225 * (+-(sine)^2 - sine) + sine
    if (sine < 0)
    {
        sine = SINE_ACCURACY_CONSTANT * (sine * -1 * sine - sine) + sine;
    }
    else
    {
        sine = SINE_ACCURACY_CONSTANT * (sine * sine - sine) + sine;
    }
    // Return the calculated value
    return sine;
}

/**
  * @brief Creates a lookup table for the sine wave
  * @param none
  * @retval none
  */
void Create_SineTable(void)
{
	for(uint16_t i = 0; i < WAVEFORM_MAX_COUNT; i++)
	{
		sine_lookup[i] = fast_sin((float)i * PI / (float) WAVEFORM_MAX_COUNT);
	}
}
