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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RS (0)
#define RW (1) 
#define E (2)
#define MASK(x) (1UL << (x))

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
 int minute = 0;
 int second = 0;
 int milisecond = 0;
 int isTurnOff = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void 	InitLCD(void);
void Delay(long long );
void EnterLCDData(unsigned char);
void EnterLCDCommand(unsigned char);
void InitButtons(void);
void InitTimer(void);
void ClearLine(void);
void ShowTimer(void);
void ShowNumber(int);
void ResetCursor(void);
void IncrementTime(void);
void DelayMs(int);
void ResetTimer(void);
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
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	InitLCD();

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 249;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 7999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 499;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Button1_Pin Button2_Pin */
  GPIO_InitStruct.Pin = Button1_Pin|Button2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Button3_Pin */
  GPIO_InitStruct.Pin = Button3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Button3_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 10, 1);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 10, 1);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 10, 1);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	
	
	HAL_NVIC_SetPriority(SysTick_IRQn, 0 ,0);

}

/* USER CODE BEGIN 4 */

// To Be Called When TIM3 Overflows (250 ms)
void TIM3_IRQHandler(void){
			// Reset Cursor To The Beginning Of The Line
			ResetCursor();
			// Increment Timer  +250 ms
			IncrementTime();
			// Print Time 
			ShowTimer();
	
}
// To Be Called When TIM4 Overflows (500 ms)
void TIM4_IRQHandler(void){
	// Clear Line
	ClearLine();
	// Reset Cursor To The Beginning Of The Line
	ResetCursor();
	// Delay For Blinking
	Delay(10);
	// Print Turn Off
	EnterLCDData('T');
	EnterLCDData('U');
	EnterLCDData('R');
	EnterLCDData('N');
	EnterLCDData(' ');
	EnterLCDData('O');
	EnterLCDData('F');
	EnterLCDData('F');
}

// To Be Called When One Of The Buttons Pushed
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	
	// When First Button Clicked
	if(GPIO_Pin == Button1_Pin){
		// When The State Is not in Turn Off Mode
		if(isTurnOff == 0){
		// When First Button Pushed
		if(GPIOB->IDR){
		// Start TIM3 To Counting
		HAL_TIM_Base_Start_IT(&htim3);
		}	}}
		// When Second Button Clicked
	else if(GPIO_Pin == Button2_Pin){
				// When The State Is not in Turn Off Mode
			if(isTurnOff == 0){
						// When Second Button Pushed
		if(GPIOB->IDR){
					// Stop TIM3 
		HAL_TIM_Base_Stop_IT(&htim3);
		}	}}
			// When Third Button Clicked
	else if (GPIO_Pin == Button3_Pin){
		HAL_NVIC_SetPendingIRQ(EXTI2_IRQn);
		// Stop TIM3
		HAL_TIM_Base_Stop_IT(&htim3);
		// Reset Timer To 00:00:000
		ResetTimer();
		int pushedFor3000ms = 1;
		// Get Current HAL_Tick In ms
		uint32_t start = HAL_GetTick();
		// Wait For 3 seconds
		while((HAL_GetTick()- start)<500){
				// No Need For Switching To Turn Off Mode If Release The Third Button
				if(HAL_GPIO_ReadPin(GPIOB, MASK(2) ) != GPIO_PIN_SET){
					pushedFor3000ms = 0;
					break;
				}}
		// Switching To Turn Off Mode If Third Button Remains Clicked For 3 seconds
		if(pushedFor3000ms == 1){
					HAL_TIM_Base_Start_IT(&htim4);
					isTurnOff = 1;
			//Wait Until Third Button Get Released
				while(HAL_GPIO_ReadPin(GPIOB, MASK(2) ) == GPIO_PIN_SET){		}
	}else{
			// Stop Turn Off Mode 
			HAL_TIM_Base_Stop_IT(&htim4);
			isTurnOff = 0;	}
		HAL_NVIC_ClearPendingIRQ(EXTI2_IRQn);
	}
}





// Reset Timer To 00:00:000
void ResetTimer(){
	milisecond = 0;
	second = 0;
	minute = 0;
	ResetCursor();
	ShowTimer();
}



// Init LCD
void InitLCD(void) {

	// Init LCD
	Delay(15000);
	EnterLCDCommand(0x30);
	Delay(5000);
	EnterLCDCommand(0x30);
	Delay(500);
	EnterLCDCommand(0x30);
	Delay(500);
	EnterLCDCommand(0x30);
	EnterLCDCommand(0x30);
	EnterLCDCommand(0x30);
	Delay(500);
	// Init LCD 2 Lines  8 Bits Mode
	EnterLCDCommand(0x38); 
	// Entry Mode
		Delay(500);
	EnterLCDCommand(0x06); 
	// Clear The Ram And Screen
	EnterLCDCommand(0x01); 
		Delay(500);
	// Move Cursor To Start Of First Line
	EnterLCDCommand(0x80);
	Delay(500);	
	// Start The LCD With Blinking Cursor
	EnterLCDCommand(0x0F); 
	Delay(500);
	
	// Set Welcome In First Line
	EnterLCDData('W');
	EnterLCDData('E');
	EnterLCDData('L');
	EnterLCDData('C');
	EnterLCDData('O');
	EnterLCDData('M');
	EnterLCDData('E');
	Delay(500);

}



void EnterLCDCommand(unsigned char command) {
	//Set  RW, RS and Command Bits
	GPIOA->ODR &= ~(MASK(RS)); //RS = 0
	GPIOA->ODR &= ~(MASK(RW));//RW = 0
	GPIOA->ODR &= ~(0xFF<<3);
	//Set Command On PA3 To PA10
	GPIOA->ODR |= (command<<3);
	//Enable E
	GPIOA->ODR |= MASK(E);
	Delay(25);
	//Disable E
	GPIOA->ODR &=  ~(MASK(E));
}


//Execute DATA On LCD
void EnterLCDData(unsigned char data) {
	//Set  RW, RS and DATA Bits
	GPIOA->ODR |= MASK(RS);   // RS = 1
	GPIOA->ODR &= ~(MASK(RW));  // RW = 0
	GPIOA->ODR &= ~(0xFF<<3);
	//Set Data On PA3 To PA10
	GPIOA->ODR |= (data <<3);
	//Enable E
	GPIOA->ODR |= MASK(E);
	Delay(50);
	//Disable E
	GPIOA->ODR &= ~(MASK(E));
}

// Show Timer In 00:00:000 Format
void ShowTimer(void){
	ShowNumber(minute/10);
	ShowNumber(minute%10);
	EnterLCDData(':');
	ShowNumber(second/10);
	ShowNumber(second%10);
	EnterLCDData(':');
	ShowNumber(milisecond/100);
	ShowNumber(milisecond%100 / 10);
	ShowNumber(milisecond%10);
}


void ShowNumber(int input){
	// Enter 0 To LCD
	if(input == 0){
		EnterLCDData('0');
		return;
	}
	
	// Make Input Negative If Input < 0 && Enter - To LCD
	if(input < 0){
		EnterLCDData('-');
		input *= (-1);
	}
	// Reverse Input To Show Properly In LCD
	int reversed = 1;
	while(input > 0){
		reversed *= 10;
		reversed += input%10;
		input /= 10;
	}
	// Enter Input Digit By Digit
	while(reversed > 1){
		EnterLCDData('0' + (reversed%10));
		reversed /= 10;
	}
	

}



void ClearLine(void){

		EnterLCDCommand(0x80); 
	  EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDData(' ');
		EnterLCDCommand(0x80); 
}

void ResetCursor(){
			EnterLCDCommand(0x80); 
}



// Increment Time By 250 ms
void IncrementTime(){
	milisecond+=250;
	if(milisecond > 999){
		milisecond=0;
		second++;
	}
	if(second>59){
		second = 0;
		minute++;
	}

}





// Delay
void Delay(long long t){
	for(volatile long long i=0;i<t;i++);
}

// Delay ms With SysTick
void DelayMs(int n){
	int i;
	SysTick->LOAD = 8000;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x5;
	
	for(i = 0; i<n; i++){
		while((SysTick->CTRL & 0x10000)==0){}
		}
		SysTick->CTRL = 0;
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

