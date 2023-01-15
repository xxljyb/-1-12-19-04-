/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, KB10D_Pin|KB10C_Pin|KB10B_Pin|KB10A_Pin
                          |KB4C_Pin|KB4A_Pin|KB3D_Pin|KB3C_Pin
                          |KB3A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, KB12C_Pin|KB12B_Pin|KB11C_Pin|KB11B_Pin
                          |KB11A_Pin|KB6C_Pin|KB6B_Pin|KB2B_Pin
                          |KB2A_Pin|KB1D_Pin|KB1C_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, KB12A_Pin|KB1B_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, KB6A_Pin|KB5D_Pin|KB5C_Pin|KB5B_Pin
                          |KB7C_Pin|KB7B_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, KB5A_Pin|KB8D_Pin|KB8C_Pin|KB8B_Pin
                          |KB8A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, KB7A_Pin|KB2D_Pin|KB2C_Pin|KB13A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, KB1A_Pin|KB4D_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, KB4B_Pin|KB3B_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(KB13B_GPIO_Port, KB13B_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                           PEPin PEPin PEPin PEPin
                           PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = KB10D_Pin|KB10C_Pin|KB10B_Pin|KB10A_Pin
                          |KB4C_Pin|KB4B_Pin|KB4A_Pin|KB3D_Pin
                          |KB3C_Pin|KB3B_Pin|KB3A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PFPin
                           PFPin PFPin PFPin PFPin
                           PFPin PFPin PFPin PFPin
                           PFPin */
  GPIO_InitStruct.Pin = KB12C_Pin|KB12B_Pin|KB12A_Pin|KB11C_Pin
                          |KB11B_Pin|KB11A_Pin|KB6C_Pin|KB6B_Pin
                          |KB2B_Pin|KB2A_Pin|KB1D_Pin|KB1C_Pin
                          |KB1B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin */
  GPIO_InitStruct.Pin = KB6A_Pin|KB5D_Pin|KB5C_Pin|KB5B_Pin
                          |KB7C_Pin|KB7B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin */
  GPIO_InitStruct.Pin = KB5A_Pin|KB8D_Pin|KB8C_Pin|KB8B_Pin
                          |KB8A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = KB7A_Pin|KB2D_Pin|KB2C_Pin|KB13A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PGPin PGPin PGPin */
  GPIO_InitStruct.Pin = KB1A_Pin|KB4D_Pin|KB13B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
//½«ËùÓÐIOÖÃµÍ
void All_GPIO_RESET(void)
{
	#if LINK_IO_ENABLE
	HAL_GPIO_WritePin(KB6A_GPIO_Port,KB6A_Pin,GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(KB6B_GPIO_Port,KB6B_Pin,GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(KB6C_GPIO_Port,KB6C_Pin,GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(KB7A_GPIO_Port,KB7A_Pin,GPIO_PIN_RESET);  
  HAL_GPIO_WritePin(KB7B_GPIO_Port,KB7B_Pin,GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(KB7C_GPIO_Port,KB7C_Pin,GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(KB8A_GPIO_Port,KB8A_Pin,GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(KB8B_GPIO_Port,KB8B_Pin,GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(KB8C_GPIO_Port,KB8C_Pin,GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(KB8D_GPIO_Port,KB8D_Pin,GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(KB11A_GPIO_Port,KB11A_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(KB11B_GPIO_Port,KB11B_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(KB11C_GPIO_Port,KB11C_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(KB12A_GPIO_Port,KB12A_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB12B_GPIO_Port,KB12B_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB12C_GPIO_Port,KB12C_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB10A_GPIO_Port,KB10A_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB10B_GPIO_Port,KB10B_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB10C_GPIO_Port,KB10C_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(KB10D_GPIO_Port,KB10D_Pin,GPIO_PIN_RESET);
	#endif
	#if Attenuation_ONE_IO_ENABLE
	HAL_GPIO_WritePin(KB1A_GPIO_Port,KB1A_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB1B_GPIO_Port,KB1B_Pin,GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(KB1C_GPIO_Port,KB1C_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB1D_GPIO_Port,KB1D_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB2A_GPIO_Port,KB2A_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB2B_GPIO_Port,KB2B_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB2C_GPIO_Port,KB2C_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB2D_GPIO_Port,KB2D_Pin,GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(KB3A_GPIO_Port,KB3A_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB3B_GPIO_Port,KB3B_Pin,GPIO_PIN_RESET);
	#endif
	#if Attenuation_TWO_IO_ENABLE
	HAL_GPIO_WritePin(KB4A_GPIO_Port,KB4A_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB4B_GPIO_Port,KB4B_Pin,GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(KB4C_GPIO_Port,KB4C_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB4D_GPIO_Port,KB4D_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB5A_GPIO_Port,KB5A_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB5B_GPIO_Port,KB5B_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB5C_GPIO_Port,KB5C_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB5D_GPIO_Port,KB5D_Pin,GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(KB13A_GPIO_Port,KB13A_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KB13B_GPIO_Port,KB13B_Pin,GPIO_PIN_RESET);
	#endif
}
/* USER CODE END 2 */
