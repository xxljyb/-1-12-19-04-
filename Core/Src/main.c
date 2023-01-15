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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx_it.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define  UART_ENABLE_RE(USARTx)       USARTx.Instance->CR1|= (uint32_t)0x0004            
#define  UART_DISABLE_RE(USARTx)      USARTx.Instance->CR1&= (~(uint32_t)0x0004)  
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Data_Processing(void);  //串口数据处理函数
void GPIO_CONTROL_STATUS(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t status);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t Recevice_Buff[USART_REC_LEN];   //接收缓冲区
//接收状态
//bit15，	接收完成标志
//bit14，	接收到帧头
//bit13，  接收到正确数据类型
//bit12，  接收到数据长度
//bit11，  接收到帧尾
//bit10~0，	接收到的有效字节数目
uint16_t USART_RX_STA = 0;       //接收状态标记	
#define RXBUFFERSIZE  1
extern uint8_t aRxBuffer[RXBUFFERSIZE];
uint8_t Recevice_Len = 0;
uint8_t Recevice_Len_temp = 0;
uint8_t Usart_Flag = 0;   //0: 串口一发送的数据 1： 串口二发送的数据
uint8_t USART_Hex[] = {0x70,0x61,0x67,0x65,0x20,0x6C,0x6F,0x63,0x61,0x6C,0xff,0xff,0xff};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t i = 0;  //循环变量
	uint8_t sum=0;
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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(USART_RX_STA&0x8000) //接收到数据
		{
			//数据解析 以防数据覆盖关闭串口接收
			#if 1
			UART_DISABLE_RE(huart1);
			UART_DISABLE_RE(huart2);
			UART_DISABLE_RE(huart3);
			#endif
			//发送接收到的数据
			if(Usart_Flag == 0)
			{
				HAL_UART_Transmit(&huart1,(uint8_t *)Recevice_Buff,USART_RX_STA&0x0fff,1000);
				while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);		//等待发送结束
				HAL_UART_Transmit(&huart2,(uint8_t *)USART_Hex,sizeof(USART_Hex),100);
				while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)!=SET);		//等待发送结束
			}else if(Usart_Flag == 1)
			{
				HAL_UART_Transmit(&huart1,(uint8_t *)Recevice_Buff,USART_RX_STA&0x0fff,1000);
				while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);		//等待发送结束
			}
			else if(Usart_Flag == 2)
			{
				HAL_UART_Transmit(&huart3,(uint8_t *)Recevice_Buff,USART_RX_STA&0x0fff,1000);
				while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)!=SET);		//等待发送结束
			}
			/* 0x01: 衰减1 通道控制
			 * 0x02: 衰减2 通道控制
			 * 0x03: 开关控制
			 * 0x04: 上位机直接设置值控制衰减
			 */
			switch(Recevice_Buff[0])
			{
				case 0x01: 
				{
					GPIO_CONTROL_STATUS(KB1A_GPIO_Port,KB1A_Pin,(Recevice_Buff[2]&0x02)>>1);
					GPIO_CONTROL_STATUS(KB1B_GPIO_Port,KB1B_Pin,(Recevice_Buff[2]&0x01)>>0);
					
					GPIO_CONTROL_STATUS(KB1C_GPIO_Port,KB1C_Pin,(Recevice_Buff[3]&0x20)>>5);
					GPIO_CONTROL_STATUS(KB1D_GPIO_Port,KB1D_Pin,(Recevice_Buff[3]&0x10)>>4);
					GPIO_CONTROL_STATUS(KB2A_GPIO_Port,KB2A_Pin,(Recevice_Buff[3]&0x08)>>3);
					GPIO_CONTROL_STATUS(KB2B_GPIO_Port,KB2B_Pin,(Recevice_Buff[3]&0x04)>>2);
					GPIO_CONTROL_STATUS(KB2C_GPIO_Port,KB2C_Pin,(Recevice_Buff[3]&0x02)>>1);
					GPIO_CONTROL_STATUS(KB2D_GPIO_Port,KB2D_Pin,(Recevice_Buff[3]&0x01)>>0);
					
					GPIO_CONTROL_STATUS(KB3A_GPIO_Port,KB3A_Pin,(Recevice_Buff[4]&0x02)>>1);
					GPIO_CONTROL_STATUS(KB3B_GPIO_Port,KB3B_Pin,(Recevice_Buff[4]&0x01)>>0);
				}break;
				case 0x02:
				{
					GPIO_CONTROL_STATUS(KB4A_GPIO_Port,KB4A_Pin,(Recevice_Buff[2]&0x02)>>1);
					GPIO_CONTROL_STATUS(KB4B_GPIO_Port,KB4B_Pin,(Recevice_Buff[2]&0x01)>>0);
					
					GPIO_CONTROL_STATUS(KB4C_GPIO_Port,KB4C_Pin,(Recevice_Buff[3]&0x20)>>5);
					GPIO_CONTROL_STATUS(KB4D_GPIO_Port,KB4D_Pin,(Recevice_Buff[3]&0x10)>>4);
					GPIO_CONTROL_STATUS(KB5A_GPIO_Port,KB5A_Pin,(Recevice_Buff[3]&0x08)>>3);
					GPIO_CONTROL_STATUS(KB5B_GPIO_Port,KB5B_Pin,(Recevice_Buff[3]&0x04)>>2);
					GPIO_CONTROL_STATUS(KB5C_GPIO_Port,KB5C_Pin,(Recevice_Buff[3]&0x02)>>1);
					GPIO_CONTROL_STATUS(KB5D_GPIO_Port,KB5D_Pin,(Recevice_Buff[3]&0x01)>>0);
					
					GPIO_CONTROL_STATUS(KB13A_GPIO_Port,KB13A_Pin,(Recevice_Buff[4]&0x02)>>1);
					GPIO_CONTROL_STATUS(KB13B_GPIO_Port,KB13B_Pin,(Recevice_Buff[4]&0x01)>>0);	
				}break;
				/* 链路控制
				 */
				case 0x03:
				{
					for(i =0; i<Recevice_Buff[1]; i++)
					{
						switch(Recevice_Buff[i+2]) //共30条链路选择
						{
							case 0x01: HAL_GPIO_WritePin(KB6A_GPIO_Port,KB6A_Pin,GPIO_PIN_RESET); break;
							case 0x02: HAL_GPIO_WritePin(KB6A_GPIO_Port,KB6A_Pin,GPIO_PIN_SET);   break;
							
							case 0x03: HAL_GPIO_WritePin(KB6B_GPIO_Port,KB6B_Pin,GPIO_PIN_RESET); break;
							case 0x04: HAL_GPIO_WritePin(KB6B_GPIO_Port,KB6B_Pin,GPIO_PIN_SET);   break;
							
							case 0x05: HAL_GPIO_WritePin(KB6C_GPIO_Port,KB6C_Pin,GPIO_PIN_RESET); break;
							case 0x06: HAL_GPIO_WritePin(KB6C_GPIO_Port,KB6C_Pin,GPIO_PIN_SET);   break;
							
							case 0x07: 
								HAL_GPIO_WritePin(KB7C_GPIO_Port,KB7C_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB7A_GPIO_Port,KB7A_Pin,GPIO_PIN_RESET); break;
							case 0x08: 
								HAL_GPIO_WritePin(KB7C_GPIO_Port,KB7C_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB7A_GPIO_Port,KB7A_Pin,GPIO_PIN_SET);   break;
							case 0x09:
								HAL_GPIO_WritePin(KB7C_GPIO_Port,KB7C_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB7B_GPIO_Port,KB7B_Pin,GPIO_PIN_RESET); break;
							case 0x0a:
								HAL_GPIO_WritePin(KB7C_GPIO_Port,KB7C_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB7B_GPIO_Port,KB7B_Pin,GPIO_PIN_SET);   break;
							
							case 0x0b: 
								HAL_GPIO_WritePin(KB8C_GPIO_Port,KB8C_Pin,GPIO_PIN_RESET); 
								HAL_GPIO_WritePin(KB8A_GPIO_Port,KB8A_Pin,GPIO_PIN_RESET); break;
							case 0x0c: 
								HAL_GPIO_WritePin(KB8C_GPIO_Port,KB8C_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB8A_GPIO_Port,KB8A_Pin,GPIO_PIN_SET);   break;
							case 0x0d: 
								HAL_GPIO_WritePin(KB8C_GPIO_Port,KB8C_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB8B_GPIO_Port,KB8B_Pin,GPIO_PIN_RESET); break;
							case 0x0e: 
								HAL_GPIO_WritePin(KB8C_GPIO_Port,KB8C_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB8B_GPIO_Port,KB8B_Pin,GPIO_PIN_SET);   break;
							
							case 0x0f: HAL_GPIO_WritePin(KB8D_GPIO_Port,KB8D_Pin,GPIO_PIN_RESET); break;
							case 0x10: HAL_GPIO_WritePin(KB8D_GPIO_Port,KB8D_Pin,GPIO_PIN_SET);   break;
							
							case 0x11: 	
								HAL_GPIO_WritePin(KB12A_GPIO_Port,KB12A_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB12B_GPIO_Port,KB12B_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB12C_GPIO_Port,KB12C_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB11A_GPIO_Port,KB11A_Pin,GPIO_PIN_RESET); break;
							case 0x12: 
								HAL_GPIO_WritePin(KB12A_GPIO_Port,KB12A_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB12B_GPIO_Port,KB12B_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB12C_GPIO_Port,KB12C_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB11A_GPIO_Port,KB11A_Pin,GPIO_PIN_SET);   break;
							case 0x13: 
								HAL_GPIO_WritePin(KB12A_GPIO_Port,KB12A_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB12B_GPIO_Port,KB12B_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB12C_GPIO_Port,KB12C_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB11B_GPIO_Port,KB11B_Pin,GPIO_PIN_RESET); break;
							case 0x14: 
								HAL_GPIO_WritePin(KB12A_GPIO_Port,KB12A_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB12B_GPIO_Port,KB12B_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB12C_GPIO_Port,KB12C_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB11B_GPIO_Port,KB11B_Pin,GPIO_PIN_SET);   break;
							case 0x15: 
								HAL_GPIO_WritePin(KB12A_GPIO_Port,KB12A_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB12B_GPIO_Port,KB12B_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB12C_GPIO_Port,KB12C_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB11C_GPIO_Port,KB11C_Pin,GPIO_PIN_RESET); break;
							case 0x16:
								HAL_GPIO_WritePin(KB12A_GPIO_Port,KB12A_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB12B_GPIO_Port,KB12B_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(KB12C_GPIO_Port,KB12C_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(KB11C_GPIO_Port,KB11C_Pin,GPIO_PIN_SET);   break;
							
							case 0x17: HAL_GPIO_WritePin(KB10A_GPIO_Port,KB10A_Pin,GPIO_PIN_RESET); break;
							case 0x18: HAL_GPIO_WritePin(KB10A_GPIO_Port,KB10A_Pin,GPIO_PIN_SET);   break;
							
							case 0x19: HAL_GPIO_WritePin(KB10B_GPIO_Port,KB10B_Pin,GPIO_PIN_RESET); break;
							case 0x1a: HAL_GPIO_WritePin(KB10B_GPIO_Port,KB10B_Pin,GPIO_PIN_SET);   break;
							
							case 0x1b: HAL_GPIO_WritePin(KB10C_GPIO_Port,KB10C_Pin,GPIO_PIN_RESET); break;
							case 0x1c: HAL_GPIO_WritePin(KB10C_GPIO_Port,KB10C_Pin,GPIO_PIN_SET);   break;
							
							case 0x1d: HAL_GPIO_WritePin(KB10D_GPIO_Port,KB10D_Pin,GPIO_PIN_RESET); break;
							case 0x1e: HAL_GPIO_WritePin(KB10D_GPIO_Port,KB10D_Pin,GPIO_PIN_SET);   break;
							default:  break; 
						}
					}
				} break;
				case 0x04:
				{
					if(Recevice_Buff[2]<=30)
					{
						HAL_GPIO_WritePin(KB1A_GPIO_Port,KB1A_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB1B_GPIO_Port,KB1B_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(KB3A_GPIO_Port,KB3A_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB3B_GPIO_Port,KB3B_Pin,GPIO_PIN_SET);
						sum = Recevice_Buff[2]*2;  //取得原本的十六进制值
						GPIO_CONTROL_STATUS(KB1C_GPIO_Port,KB1C_Pin,sum>>5&0x01);
						GPIO_CONTROL_STATUS(KB1D_GPIO_Port,KB1D_Pin,sum>>4&0x01);
						GPIO_CONTROL_STATUS(KB2A_GPIO_Port,KB2A_Pin,sum>>3&0x01);
						GPIO_CONTROL_STATUS(KB2B_GPIO_Port,KB2B_Pin,sum>>2&0x01);
						GPIO_CONTROL_STATUS(KB2C_GPIO_Port,KB2C_Pin,sum>>1&0x01);
						GPIO_CONTROL_STATUS(KB2D_GPIO_Port,KB2D_Pin,sum>>0&0x01);
					}
					else if(Recevice_Buff[2]>30&&Recevice_Buff[2]<=60)
					{
						HAL_GPIO_WritePin(KB1A_GPIO_Port,KB1A_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(KB1B_GPIO_Port,KB1B_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB3A_GPIO_Port,KB3A_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB3B_GPIO_Port,KB3B_Pin,GPIO_PIN_SET);
						sum = (Recevice_Buff[2]-30)*2;  //取得原本的十六进制值
						GPIO_CONTROL_STATUS(KB1C_GPIO_Port,KB1C_Pin,sum>>5&0x01);
						GPIO_CONTROL_STATUS(KB1D_GPIO_Port,KB1D_Pin,sum>>4&0x01);
						GPIO_CONTROL_STATUS(KB2A_GPIO_Port,KB2A_Pin,sum>>3&0x01);
						GPIO_CONTROL_STATUS(KB2B_GPIO_Port,KB2B_Pin,sum>>2&0x01);
						GPIO_CONTROL_STATUS(KB2C_GPIO_Port,KB2C_Pin,sum>>1&0x01);
						GPIO_CONTROL_STATUS(KB2D_GPIO_Port,KB2D_Pin,sum>>0&0x01);
					}
					else if(Recevice_Buff[2]>60&&Recevice_Buff[2]<=90)
					{
						HAL_GPIO_WritePin(KB1A_GPIO_Port,KB1A_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(KB1B_GPIO_Port,KB1B_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB3A_GPIO_Port,KB3A_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(KB3B_GPIO_Port,KB3B_Pin,GPIO_PIN_RESET);
						sum = (Recevice_Buff[2]-60)*2;  //取得原本的十六进制值
						GPIO_CONTROL_STATUS(KB1C_GPIO_Port,KB1C_Pin,sum>>5&0x01);
						GPIO_CONTROL_STATUS(KB1D_GPIO_Port,KB1D_Pin,sum>>4&0x01);
						GPIO_CONTROL_STATUS(KB2A_GPIO_Port,KB2A_Pin,sum>>3&0x01);
						GPIO_CONTROL_STATUS(KB2B_GPIO_Port,KB2B_Pin,sum>>2&0x01);
						GPIO_CONTROL_STATUS(KB2C_GPIO_Port,KB2C_Pin,sum>>1&0x01);
						GPIO_CONTROL_STATUS(KB2D_GPIO_Port,KB2D_Pin,sum>>0&0x01);
					}
					if(Recevice_Buff[6]<=30)
					{
						HAL_GPIO_WritePin(KB4A_GPIO_Port,KB4A_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB4B_GPIO_Port,KB4B_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(KB13A_GPIO_Port,KB13A_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB13B_GPIO_Port,KB13B_Pin,GPIO_PIN_SET);
						sum = Recevice_Buff[6]*2;  //取得原本的十六进制值
						
						GPIO_CONTROL_STATUS(KB4C_GPIO_Port,KB4C_Pin,sum>>5&0x01);
						GPIO_CONTROL_STATUS(KB4D_GPIO_Port,KB4D_Pin,sum>>4&0x01);
						GPIO_CONTROL_STATUS(KB5A_GPIO_Port,KB5A_Pin,sum>>3&0x01);
						GPIO_CONTROL_STATUS(KB5B_GPIO_Port,KB5B_Pin,sum>>2&0x01);
						GPIO_CONTROL_STATUS(KB5C_GPIO_Port,KB5C_Pin,sum>>1&0x01);
						GPIO_CONTROL_STATUS(KB5D_GPIO_Port,KB5D_Pin,sum>>0&0x01);
					}
					else if(Recevice_Buff[6]>30&&Recevice_Buff[6]<=60)
					{
						HAL_GPIO_WritePin(KB4A_GPIO_Port,KB4A_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(KB4B_GPIO_Port,KB4B_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB13A_GPIO_Port,KB13A_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB13B_GPIO_Port,KB13B_Pin,GPIO_PIN_SET);
						sum = (Recevice_Buff[6]-30)*2;  //取得原本的十六进制值
						
						GPIO_CONTROL_STATUS(KB4C_GPIO_Port,KB4C_Pin,sum>>5&0x01);
						GPIO_CONTROL_STATUS(KB4D_GPIO_Port,KB4D_Pin,sum>>4&0x01);
						GPIO_CONTROL_STATUS(KB5A_GPIO_Port,KB5A_Pin,sum>>3&0x01);
						GPIO_CONTROL_STATUS(KB5B_GPIO_Port,KB5B_Pin,sum>>2&0x01);
						GPIO_CONTROL_STATUS(KB5C_GPIO_Port,KB5C_Pin,sum>>1&0x01);
						GPIO_CONTROL_STATUS(KB5D_GPIO_Port,KB5D_Pin,sum>>0&0x01);
					}
					else if(Recevice_Buff[6]>60&&Recevice_Buff[6]<=90)
					{
						HAL_GPIO_WritePin(KB4A_GPIO_Port,KB4A_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(KB4B_GPIO_Port,KB4B_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(KB13A_GPIO_Port,KB13A_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(KB13B_GPIO_Port,KB13B_Pin,GPIO_PIN_RESET);
						sum = (Recevice_Buff[6]-60)*2;  //取得原本的十六进制值
						
						GPIO_CONTROL_STATUS(KB4C_GPIO_Port,KB4C_Pin,sum>>5&0x01);
						GPIO_CONTROL_STATUS(KB4D_GPIO_Port,KB4D_Pin,sum>>4&0x01);
						GPIO_CONTROL_STATUS(KB5A_GPIO_Port,KB5A_Pin,sum>>3&0x01);
						GPIO_CONTROL_STATUS(KB5B_GPIO_Port,KB5B_Pin,sum>>2&0x01);
						GPIO_CONTROL_STATUS(KB5C_GPIO_Port,KB5C_Pin,sum>>1&0x01);
						GPIO_CONTROL_STATUS(KB5D_GPIO_Port,KB5D_Pin,sum>>0&0x01);
					}
				} break;
				default :  break; //将所有链路IO拉低
			}
			//清除数据及接收标准位
			memset(Recevice_Buff,0x00,USART_RX_STA&0x0fff);
			USART_RX_STA = 0;
			//使能串口接收
			#if 1
			UART_ENABLE_RE(huart1);
			UART_ENABLE_RE(huart2);
			UART_ENABLE_RE(huart3);
			#endif
		}
		else
		{
			__WFI();  //未接受到数据进入睡眠模式
		}
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void GPIO_CONTROL_STATUS(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t status)
{
	if(status == 1)
	{
		HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
	}
	else if(status == 0)
	{
		HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
	}
}



//接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	if(huart->Instance==USART1)//如果是串口1
	{
		Usart_Flag = 0;
		Data_Processing();
		HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE);
	}
	else if(huart->Instance==USART2)
	{
		Usart_Flag = 1;
		Data_Processing();
		HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, RXBUFFERSIZE);
	}
	else if(huart->Instance==USART3)
	{
		Usart_Flag = 2;
		Data_Processing();
		HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, RXBUFFERSIZE);
	}
}
//bit15，	接收完成标志
//bit14，	接收到帧头
//bit13，  接收到正确数据类型
//bit12，  接收到数据长度 OR 开关选择位
//bit11~0，	接收到的有效字节数目
//串口接收处理函数
void Data_Processing(void)
{
	if((USART_RX_STA&0x8000)==0)//接收未完成
	{
		if(USART_RX_STA&0x4000)//接收到了帧头
		{
			if(USART_RX_STA&0x2000)  //接收到正确数据类型
			{
				if(USART_RX_STA&0x1000)  //接收到数据长度
				{
					if(Recevice_Len_temp>0)
					{
						//接收数据
						Recevice_Buff[USART_RX_STA&0x0fff] = aRxBuffer[0];
						USART_RX_STA++;
						Recevice_Len_temp--;
					}
					else{
						if(aRxBuffer[0] == 0xC1) //检测到帧尾
						{
							//接收完成
							USART_RX_STA |= 0x8000;
						}
						else{
							memset(Recevice_Buff,0x00,USART_RX_STA&0x0fff);
							USART_RX_STA = 0;  //接收错误
						}
					}
				}
				else
				{
					if(aRxBuffer[0]>USART_REC_LEN)
					{
						memset(Recevice_Buff,0x00,USART_RX_STA&0x0fff);
						USART_RX_STA=0; //接收错误，数据长度超出限制
					}
					else
					{
						Recevice_Len = aRxBuffer[0];
						Recevice_Len_temp = Recevice_Len;
						Recevice_Buff[USART_RX_STA&0x0fff] = aRxBuffer[0];
						USART_RX_STA++;
						USART_RX_STA |= 0x1000;
					}
				}
			}
			else
			{
				switch(aRxBuffer[0])
				{
					case 0x01: USART_RX_STA|=0x2000; Recevice_Buff[USART_RX_STA&0x0fff]=aRxBuffer[0]; USART_RX_STA++; break;
					case 0x02: USART_RX_STA|=0x2000; Recevice_Buff[USART_RX_STA&0x0fff]=aRxBuffer[0]; USART_RX_STA++; break;
					case 0x03: USART_RX_STA|=0x2000; Recevice_Buff[USART_RX_STA&0x0fff]=aRxBuffer[0]; USART_RX_STA++; break;
					case 0x04: USART_RX_STA|=0x2000; Recevice_Buff[USART_RX_STA&0x0fff]=aRxBuffer[0]; USART_RX_STA++; break;
					default: USART_RX_STA=0; break; //接收数据类型错误，重新接收
				}
			}
		}
		else //还没收到0XC0 帧头
		{	
			if(aRxBuffer[0]==0xC0)USART_RX_STA|=0x4000;
			else
			{
				USART_RX_STA = 0;  //接收错误
			}		 
		}
	}
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
