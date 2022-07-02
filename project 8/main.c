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
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// LCD Pins
#define RS (0)
#define RW (1) 
#define E (2)

#define MASK(x) (1UL << (x))
#define NMASK(x) (~(1UL <<(x)))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

typedef enum {OPERAND1 , OPERAND2, OPERATOR2, ASSIGN} state;

//Variables
volatile  static state opState = OPERAND1;
volatile static unsigned int zero_counter = 0;
static volatile int operand1 = 0, operand2 = 0, keyPadColIndex = 0;
static volatile int result = 0;
static volatile char op = '+', tempOperator = '+';
uint8_t character, rx_Data=' ';
int first = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void  Delay(int);
void  ChangeKeyPadColIndex(void);
void  calculateResult(void);
void InitLCD(void);
void clearFirstLine(void);
void LCD_data(unsigned char );
void LCDCmd(unsigned char );
void LCD_display(long long int num);
long long int reverse(long long int a);
void initPrint();

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
    {
    HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,100);
        
        return ch;
        
    }
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	InitLCD();
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		ChangeKeyPadColIndex();

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

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC4 PC5 PC6 PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 4 */



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(rx_Data==' '){
		switch (GPIO_Pin){
			case GPIO_PIN_3:{switch(keyPadColIndex){
		//Clear Key
		case 0:{
			character = 'C';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			opState = OPERAND1;
			operand1 = 0;
			operand2 = 0;
			break;
		}
		//0 Key
		case 1:{
			character = '0';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			if(opState == OPERAND1){
				operand1 *= 10;
				operand1 += 0;
			}
			else if (opState==OPERAND2){
				operand2 *= 10;
				operand2 += 0;
			}
			break;
		}
		//= Key
		case 2:{
			opState = ASSIGN;
			character = '=';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}

			break;
		}
		// + Key
		case 3:{
			
			if(opState==OPERAND1){
					op = '+';
					opState = OPERAND2;
			}
			// If Second Operator Clicked
			else if(opState == OPERAND2){
					tempOperator = '+';
					opState = OPERATOR2;
			}
			character = '+';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			break;
		}	
	}
				while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3) == GPIO_PIN_SET);
			break;}
			case GPIO_PIN_2:{switch(keyPadColIndex){
		// 1 Key
		case 0:{
			character = '1';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			if(opState == OPERAND1){
				operand1 *= 10;
				operand1 += 1;
			}
			else if (opState == OPERAND2){
				operand2 *= 10;
				operand2 += 1;
			}
			break;
		}
		// 2 Key
		case 1:{
			character = '2';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			if(opState ==OPERAND1){
				operand1 *= 10;
				operand1 += 2;
			}
			else if(opState == OPERAND2){
				operand2 *= 10;
				operand2 += 2;
			}
			break;
		}
		// 3 Key
		case 2:{
			if(opState ==OPERAND1){
				operand1 *= 10;
				operand1 += 3;
			}
			else if (opState == OPERAND2){
				operand2 *= 10;
				operand2 += 3;
			}
			character = '3';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}

			break;
		}
		// - Key
		case 3:{
			if(opState==OPERAND1){
					op = '-';
					opState = OPERAND2;	
			}
			// If Second Operator Clicked
			else if(opState== OPERAND2){
					tempOperator = '-';
					opState = OPERATOR2;
			}
			character = '-';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			break;
		}
	}
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2) == GPIO_PIN_SET);
			break;}
			case GPIO_PIN_1:{switch(keyPadColIndex){
		// 4 Key
		case 0:{
			character = '4';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			if(opState == OPERAND1){
				operand1 *= 10;
				operand1 += 4;
			}
			else if (opState == OPERAND2){
				operand2 *= 10;
				operand2 += 4;
			}
			break;
		}
		// 5 Key
		case 1:{
			character = '5';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			if(opState == OPERAND1){
				operand1 *= 10;
				operand1 += 5;
			}
			else if ( opState == OPERAND2){
				operand2 *= 10;
				operand2 += 5;
			}
			break;
		}
		// 6 Key
		case 2:{
			character = '6';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			if(opState == OPERAND1){
				operand1 *= 10;
				operand1 += 6;
			}
			else if (opState == OPERAND2){
				operand2 *= 10;
				operand2 += 6;
			}
			break;
		}
		// * Key
		case 3:{
			if(opState==OPERAND1){
					op = '*';
					opState = OPERAND2;

			}
			// If Second Operator Clicked
			else if(opState==OPERAND2){
					tempOperator = '*';
					opState = OPERATOR2;
			}
			character = '*';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			break;
		}
	}
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1) == GPIO_PIN_SET);
			break;}
			case GPIO_PIN_0:{switch(keyPadColIndex){
		// 7 Key
		case 0:{
			character = '7';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			if(opState == OPERAND1){
				operand1 *= 10;
				operand1 += 7;
			}
			else if (opState == OPERAND2){
				operand2 *= 10;
				operand2 += 7;
			}
			break;
		}
		// 8 Key
		case 1:{
			character = '8';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			if(opState == OPERAND1){
				operand1 *= 10;
				operand1 += 8;
			}
			else if( opState == OPERAND2){
				operand2 *= 10;
				operand2 += 8;
			}
			break;
		}
		// 9 Key
		case 2:{
			character = '9';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			if(opState == OPERAND1){
				operand1 *= 10;
				operand1 += 9;
			}
			else if (opState == OPERAND2){
				operand2 *= 10;
				operand2 += 9;
			}
			break;
		}
		// / key
		case 3:{
			if(opState==OPERAND1){
					op = '/';
					opState = OPERAND2;

			}
			// If Second Operator Clicked
			else if(opState==OPERAND2){
					tempOperator = '/';
					opState = OPERATOR2;
			}
			character = '/';
			if(HAL_UART_Transmit(&huart1,&character,1,100) == HAL_OK){
				HAL_UART_Receive_IT(&huart1,&rx_Data,1);
			}
			break;
		}
	}
	while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0) == GPIO_PIN_SET);
			break;}
		}
	}
}

void calculateResult(void){
	if(opState == ASSIGN || opState == OPERATOR2){
			switch(op){
				case '*':result = operand1 * operand2;
							break;
				case '/':result = operand1 / operand2;
							break;
				case '+':result = operand1 + operand2;	
							break;
				case '-':result = operand1 - operand2;
							break;
			}
			operand2 = 0;
			operand1 = result;
			LCD_display(result);
			if(opState==OPERATOR2){
				op = tempOperator;
				opState = OPERAND2;
				character = tempOperator;
				LCD_data(tempOperator);
			}else{
				opState = OPERAND1;
			}
		}
}


void clearFirstLine(void){
	LCDCmd(0x80);
	for (int i=0 ;i<16; i++){
		LCD_data(' ');
	}
	LCDCmd(0x80);
}

void InitLCD(void) {
	Delay(1300000);
  LCDCmd(0x30);
  Delay(300000);
  LCDCmd(0x30);
  Delay(50000);
	LCDCmd(0x38);
  LCDCmd(0x06); 
  LCDCmd(0x80); 
  LCDCmd(0x0F); 
  LCDCmd(0x30);
  LCDCmd(0x30);
	LCDCmd(0x01); 
  LCDCmd(0x30);
  LCDCmd(0x30);
	initPrint();
	clearFirstLine();
}


void initPrint(){
	LCD_data('O');
  LCD_data('N');
  Delay(300000);
}



void LCDCmd(unsigned char command) {

	GPIOB->ODR &= ~(MASK(RS)); 
	GPIOB->ODR &= ~(MASK(RW));
	GPIOA->ODR &= ~(0xFF);
	GPIOA->ODR |= (command);
	GPIOB->ODR |= MASK(E);
	Delay(30000);
	GPIOB->ODR &=  ~(MASK(E));
	if (command < 4){
		Delay(100000);
	}
	else{
		Delay(30000);
	}
}

void LCD_data(unsigned char data) {
	GPIOB->ODR |= MASK(RS);  
	GPIOB->ODR &= ~(MASK(RW)); 
	GPIOA->ODR &= ~(0xFF);
	GPIOA->ODR |= (data);
	GPIOB->ODR |= MASK(E);
	Delay(30000);
	GPIOB->ODR &= ~(MASK(E));
	Delay(100000);
}



void ChangeKeyPadColIndex (void){
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
		keyPadColIndex = (keyPadColIndex + 1)%4;
		GPIOC->ODR |= MASK(4 + keyPadColIndex);
}

void LCD_display(long long int num){	
	clearFirstLine();
	Delay(30000);
	if(num==0){
		LCD_data('0');
		return;
	}else if(num < 0){
		LCD_data('-');
		num *= -1;
	}
	long long int reversedNumber= reverse(num);
	for(;reversedNumber > 0;reversedNumber/=10){
				LCD_data(reversedNumber%10 + '0');
		}
		while(zero_counter > 0){
				LCD_data('0');
				zero_counter--;
		}
}
long long int reverse(long long int a){
	long long int reversed = 0;
	zero_counter = 0;
	while(a % 10 == 0){
		a/=10;
		zero_counter++;
	}
	for(;a > 0;a/=10){
			reversed*=10;
			reversed+=a%10;
		}
	return reversed;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(first==0){
		first = 1;
		operand1 = 0;
		operand2 = 0;
		opState = OPERAND1;
		rx_Data=' ';
		return;
	}
	
	rx_Data == '=' || opState == OPERATOR2 ? calculateResult() : rx_Data == 'C'? clearFirstLine(): LCD_data(rx_Data);
	rx_Data=' ';

}



// Delay
void Delay(int n){
	  for(volatile long long i=0;i< n;i++);
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

