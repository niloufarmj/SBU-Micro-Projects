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

#include <stdbool.h>
#include <time.h> 
#include <stdlib.h>

//GPIOA  GPIOB  GPIOC 
#define LED_GREEN_PWM (0)
#define LED_RED_PWM (1)
#define BLUE (1)
#define C1 (2)
#define  GREEN (2) 
#define C2 (3)
#define  YELLOW (3) 
#define C3 (4)
#define RED (4) 
#define RESET (5)
#define LED_BLUE (6)
#define SA (7)
#define LED_GREEN (7)
#define SB (8)
#define LED_YELLOW (8)
#define RS (8) 
#define SC (9)
#define LED_RED (9)
#define RW (9) 
#define SD (10)
#define E  (10)
#define GREEN_PWM_ENABLE (10)
#define RED_PWM_ENABLE (11)
#define A (11)
#define SE (12)
#define B (12)
#define SF (13)
#define C (13)
#define SG (14)
#define D (14) 
#define RESET_IC (15)
#define MASK(x)   (1UL << (x))

static bool printedEND_OF_GAMEType = false; 
volatile static unsigned int zero_counter = 0;
const uint16_t timeOfGame = 60, numberOfAllowedMisses = 3; 
enum State { START,
							PRINTING_NAME,
							RUNNING,
							END_OF_GAME };
enum END_OF_GAMEType {  PLAYING,
												WIN,
												LOOSE};
volatile enum State gameState = START; 
volatile enum END_OF_GAMEType END_OF_GAMEType = PLAYING; 
volatile char inputID[8] ,currentColor='b'; 
volatile uint16_t misses = 0, inputIDIndex = 0, remainingTime = timeOfGame, currentRow = 0;
char showingName[30]; 
uint32_t count = 0; 
bool errorPassed = false;
uint32_t lastCaptured = 0;

void clearFirstLine();
int32_t charToInt(char ch);
char intToChar(int32_t digit);
void printInLine(char* string);
void printNum(int N);
void init_lcd();
void delay(uint16_t ms);
void commitCommand(unsigned char command);
void commitChar(char data);
uint16_t sevenSegment(uint16_t value);
void loose();
void win();

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
 TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t sevenSegment(uint16_t value) {
	if(value == 0){
		return MASK(SA) | MASK(SB) | MASK(SC) | MASK(SD) | MASK(SE) | MASK(SF);}
	else if (value==1){
		return MASK(SB) | MASK(SC);}
	else if (value==2){
		return MASK(SA) | MASK(SB) | MASK(SD) | MASK(SE) | MASK(SG);}
	else if (value==3){
		return MASK(SA) | MASK(SB) | MASK(SC) | MASK(SD) | MASK(SG);}
	else if (value==4){
		return MASK(SB) | MASK(SC) | MASK(SF) | MASK(SG);}
	else if (value==5){
		return MASK(SA) | MASK(SC) | MASK(SD) | MASK(SF) | MASK(SG);}
	else if (value==6){
		return MASK(SA) | MASK(SC) | MASK(SD) | MASK(SE) | MASK(SF) | MASK(SG);}
	else if (value==7){
		return MASK(SA) | MASK(SB) | MASK(SC);}
	else if (value== 8 ){
		return MASK(SA) | MASK(SB) | MASK(SC) | MASK(SD) | MASK(SE) | MASK(SF) | MASK(SG);}
	else if (value==9){
		return MASK(SA) | MASK(SB) | MASK(SC) | MASK(SD) | MASK(SF) | MASK(SG);}
	else{
		return MASK(SA) | MASK(SB) | MASK(SC) | MASK(SD) | MASK(SE) | MASK(SF);}
}
void clearFirstLine(){
	commitCommand(0x80); 
	printInLine("                ");
}

void commitCommand(unsigned char command){  //Committing commands or chars
	
	HAL_GPIO_WritePin(GPIOA, 0xFF, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, MASK(RS) | MASK(RW), GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, MASK(E) | command, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, MASK(E), GPIO_PIN_RESET);
	if(command < 4)
		delay(2); 
	else 
		delay(1);
}
void commitChar(char data){
	HAL_GPIO_WritePin(GPIOA, 0xFF | MASK(RW), GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, MASK(E) | MASK(RS) | data , GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, MASK(E) , GPIO_PIN_RESET);
	delay(1);
}
void init_lcd(){ // turn on lcd  
	commitCommand(0x0C); 
	commitCommand(0x38);
}
void clearLEDs(){
	HAL_GPIO_WritePin(GPIOC, MASK(LED_GREEN) | MASK(LED_RED) |MASK(LED_YELLOW) | MASK(LED_BLUE) , GPIO_PIN_RESET);
}
void printInLine(char* string){
	while(*string != '\0'){
		char ch = (char) (*string); 
		commitChar(ch); 
		string++;
	}
	commitCommand(0x80);
}

int32_t charToInt(char ch){
	if(ch=='0'){
		return 0;
	}
	if(ch=='1'){
		return 1;
	}
	if(ch=='2'){
		return 2;
	}
	if(ch=='3'){
		return 3;
	}
	if(ch=='4'){
		return 4;
	}
	if(ch=='5'){
		return 5;
	}
	if(ch=='6'){
		return 6;
	}
	if(ch=='7'){
		return 7;
	}
	if(ch=='8'){
		return 8;
	}
	if(ch=='9'){
		return 9;
	}
	return 0; 
}

char intToChar(int32_t dig){
	
	if(dig==0){
		return '0';
	}
	if(dig==1){
		return '1';
	}
	if(dig==2){
		return '2';
	}
	if(dig==3){
		return '3';
	}
	if(dig==4){
		return '4';
	}
	if(dig==5){
		return '5';
	}
	if(dig==6){
		return '6';
	}
	if(dig==7){
		return '7';
	}
	if(dig==8){
		return '8';
	}
	if(dig==9){
		return '9';
	}
	return '0';
}

void delay(uint16_t n){  // Make Delay
    for(int i = 0; i < n; i++){
			for(int j = 0; j < 200 ;j++);
		}
}
void delayms(uint16_t ms){
	uint32_t start = HAL_GetTick(); 
	while( (HAL_GetTick() - start) < ms);
}

 int reverse( int a){
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

void printNum(int num){
	if(num==0){
		commitChar('0');
		return;
	}else if(num < 0){
		commitChar('-');
		num *= -1;
		
	}
	int reversedNumber= reverse(num);
	for(;reversedNumber > 0;reversedNumber/=10){
				commitChar(reversedNumber%10 + '0');
		}
		while(zero_counter > 0){
				commitChar('0');
				zero_counter--;
		}
}
void setName(char* name){
	  uint16_t index = 0;
		while(*name != '\0'){
		showingName[index++] = (char) (*name); 
		name++;
	}
}
char clickKeyPadBtn(){
		if( HAL_GPIO_ReadPin( GPIOB, MASK(C1)) == GPIO_PIN_SET) {
			while(( HAL_GPIO_ReadPin( GPIOB, MASK(C1)) == GPIO_PIN_SET));
			switch(currentRow) {
				case 0:
					return '1';
				case 1:
					return '4';
				case 2:
					return '7';
				case 3:
					return '*';
				default:
					return '!';
			}
		}
		else if(HAL_GPIO_ReadPin( GPIOB, MASK(C2)) == GPIO_PIN_SET){
			while(( HAL_GPIO_ReadPin( GPIOB, MASK(C2)) == GPIO_PIN_SET));
			switch(currentRow) {
				case 0:
					return '2';
				case 1:
					return '5';
				case 2:
					return '8';
				case 3:
					return '0';
				default:
					return '!';
			}
		}
		else if(HAL_GPIO_ReadPin( GPIOB, MASK(C3)) == GPIO_PIN_SET){
			while(( HAL_GPIO_ReadPin( GPIOB, MASK(C3)) == GPIO_PIN_SET));
			switch(currentRow) {
				case 0:
					return '3';
				case 1:
					return '6';
				case 2:
					return '9';
				case 3:
					return '#';
				default:
					return '!';
			}
		}
		else { // RESET has been clicked
			if (HAL_GPIO_ReadPin( GPIOC, MASK(RESET)) == GPIO_PIN_SET){
					uint32_t start = HAL_GetTick(); 
					uint32_t delay = 300; 
					bool long_push = true; 
					while( (HAL_GetTick() - start) < delay){
							if(HAL_GPIO_ReadPin( GPIOC, MASK(RESET)) == GPIO_PIN_RESET){
								long_push = false; 
								break; 
							}
						
						if(!long_push) 
							break; 
					}
					
					if(long_push)
						return 'l';
					else 
						return 'e'; 
					
			}

				
		}
}

// reset all program states & vars. 
void resetTheProgram(){
	END_OF_GAMEType = PLAYING;
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	HAL_GPIO_WritePin(GPIOC, MASK(RED_PWM_ENABLE), GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, MASK(GREEN_PWM_ENABLE), GPIO_PIN_RESET);
	gameState = START;
	clearFirstLine();
	clearLEDs(); 
	remainingTime = timeOfGame;
	misses= count= inputIDIndex = 0;
	//Show misses
	HAL_GPIO_WritePin( GPIOB, 0xFF << 7, GPIO_PIN_RESET); 
	HAL_GPIO_WritePin( GPIOB, sevenSegment(misses), GPIO_PIN_SET);
	
	commitCommand(0x80);
	MX_TIM3_Init();
	HAL_TIM_Base_Start_IT(&htim3);
}
 
void showColor(char color){
	if(color == 'g'){ // show green color on leds.
			HAL_GPIO_WritePin(GPIOC, MASK(LED_BLUE) | MASK(LED_RED) |MASK(LED_YELLOW) , GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, MASK(LED_GREEN), GPIO_PIN_SET);
	}else if(color == 'y'){// show yellow color on leds.
			HAL_GPIO_WritePin(GPIOC, MASK(LED_BLUE) | MASK(LED_RED) |MASK(LED_GREEN) , GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, MASK(LED_YELLOW), GPIO_PIN_SET);
	}else if(color == 'b'){// show blue color on leds.
			HAL_GPIO_WritePin(GPIOC, MASK(LED_GREEN) | MASK(LED_RED) |MASK(LED_YELLOW) , GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, MASK(LED_BLUE), GPIO_PIN_SET);
	}else if (color =='r'){// show red color on leds.
			HAL_GPIO_WritePin(GPIOC, MASK(LED_BLUE) | MASK(LED_GREEN) |MASK(LED_YELLOW) , GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, MASK(LED_RED), GPIO_PIN_SET);
	}
}

char clickedLED(){
	if( HAL_GPIO_ReadPin( GPIOC, MASK(BLUE)) == GPIO_PIN_SET) 
		return 'b';
	else if (HAL_GPIO_ReadPin( GPIOC, MASK(GREEN)) == GPIO_PIN_SET)
		return 'r';
	else if (HAL_GPIO_ReadPin( GPIOC, MASK(YELLOW)) == GPIO_PIN_SET)
		return 'y';
	else if (HAL_GPIO_ReadPin( GPIOC, MASK(RED)) == GPIO_PIN_SET)
		return 'g';
}

char generateRandomColor(){
	int val = (int) rand() % 4; 
	int val2 = 0;
	switch(currentColor){
		case 'b':
			val2 = 0;break;
		case 'y':
			val2 = 1;break;
		case 'g' :
			val2 = 2;break;
		case 'r':
			val2 = 3;break;
	}
	if(val == val2){ // Different color
		val = (val+1) % 4;
	}
	switch(val){
		case 0: 
			currentColor= 'b';break;
		case 1:
			currentColor= 'y';break;
		case 2:
			currentColor= 'g'; break;
		case 3:
			currentColor= 'r';break;
	}
	return currentColor;
}

void checkingAndShowStatus(uint16_t delayms){
	if( gameState != RUNNING) 
		return;
	bool passed = false;
	char rand = generateRandomColor();
	showColor(rand);
	uint32_t start = HAL_GetTick(); 
	while( (HAL_GetTick() - start) < delayms){
		if(clickedLED() == rand)
			passed = true;
	}
	if(!passed){
		misses++; 
		
		//show misses
		HAL_GPIO_WritePin( GPIOB, 0xFF << 7, GPIO_PIN_RESET); 
		HAL_GPIO_WritePin( GPIOB, sevenSegment(misses), GPIO_PIN_SET);
		
		if(misses >= numberOfAllowedMisses){
			loose();
		}
	}
}

void runGame(){
	checkingAndShowStatus(3000/8);
	checkingAndShowStatus(3000/8);
	checkingAndShowStatus(3000/8);
	checkingAndShowStatus(2000/8);
	checkingAndShowStatus(2000/8);
	checkingAndShowStatus(2000/8);
	uint16_t delay = 1000; 
		while(time>0) {
			if(gameState != RUNNING)
				break;
			for(int i=0; i < 10; i++)
				checkingAndShowStatus(delay/8);
			delay -= 100;
		}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_PIN_0) {
			char clickedButton = clickKeyPadBtn();
			if( gameState != START ){
				if(clickedButton == '1' || clickedButton == '*')
					resetTheProgram();
			}
			else{
				if( clickedButton != 'e') 
					inputID[inputIDIndex++] = clickedButton;
				if(inputIDIndex == 8){
					clearFirstLine();
					commitCommand(0x80);
					if(inputID[0] == '9' && inputID[1] == '7' 
						&& inputID[2] == '2' && inputID[3] == '4' 
						&& inputID[4] == '3'&& inputID[5] == '0' 	
						&& inputID[6] == '6' && inputID[7] == '3')
						setName("Moradi Jam");
				  else if(inputID[0] == '9' && inputID[1] == '8' 
									&& inputID[2] == '2' && inputID[3] == '4' 
									&& inputID[4] == '3' && inputID[5] == '0' 	
									&& inputID[6] == '5' && inputID[7] == '8')
						setName("Mousavi");
					
					gameState = PRINTING_NAME; 
				}
				else if( clickedButton != 'e')
						commitChar(clickedButton);
					
			}
	}
}

void win(){
	if(END_OF_GAMEType == LOOSE)
		return;
	END_OF_GAMEType = WIN; 
	clearLEDs();
	gameState = END_OF_GAME; 
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	HAL_GPIO_WritePin(GPIOC, MASK(GREEN_PWM_ENABLE), GPIO_PIN_SET);
	clearFirstLine();
	//printInLine("Winner"); 
}

void loose(){
	if(END_OF_GAMEType == WIN)
		return;
	END_OF_GAMEType = LOOSE;
	clearLEDs();
	gameState = END_OF_GAME; 
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_GPIO_WritePin(GPIOC, MASK(RED_PWM_ENABLE), GPIO_PIN_SET);
	clearFirstLine();
	//printInLine("Looser");
}




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// decrementing time and checking win-conditions
	if( htim->Instance == TIM3){
		if( gameState == RUNNING) {
			clearFirstLine();
			remainingTime--;
			if(remainingTime >0){
				printNum(remainingTime);
			}
			else {
				if(misses == numberOfAllowedMisses ) 
						loose();
				else 
						win();
			}			
		} else if(gameState == END_OF_GAME){   
      if(END_OF_GAMEType == WIN){
        if(printedEND_OF_GAMEType)
					clearFirstLine();
				else 
					printInLine("Winner");
				printedEND_OF_GAMEType = !printedEND_OF_GAMEType;
      }else{
        if(printedEND_OF_GAMEType)
        clearFirstLine();
      else 
        printInLine("Looser");
      printedEND_OF_GAMEType = !printedEND_OF_GAMEType;
      }
    }		
	}
	
	// for toggling name 
	if( htim->Instance == TIM4) {
		if( gameState == PRINTING_NAME){
			static bool printed = false; 
			if(printed)
				clearFirstLine();
			else 
				printInLine(showingName);
			printed = !printed;
		}
	}
	
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if( !errorPassed){
		errorPassed = true; 
		return; 
	}
	if( gameState != PRINTING_NAME) 
		return; 
	
	count++;
	// first and third edges (rising)
	if(count == 1 ){
		lastCaptured = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	}
	// second edge  (falling)
	else if(count == 2){
		uint32_t currentCaptured = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		if( currentCaptured - lastCaptured < 2000/16) 
			count = 0;
			lastCaptured = 0; 
	}
	else if(count == 3){
		lastCaptured = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	}
	// check wether appropriate pulse has been seen or not. 
	else if(count == 4) {
		uint32_t currentCaptured = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		if( currentCaptured - lastCaptured >= 2000/16) 
		 {
			count = 0;
			currentRow = 3; 
			lastCaptured = 0;
			gameState = RUNNING; 
			HAL_GPIO_WritePin(GPIOA, MASK(D), GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, MASK(A)|MASK(B)|MASK(C) , GPIO_PIN_RESET);
			remainingTime = timeOfGame; 
			runGame();
		}
		else 
			count = 0;
			lastCaptured = 0; 
	}
	
	
}

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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
			init_lcd();
		HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
		HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
		HAL_TIM_Base_Start_IT(&htim2);
		HAL_TIM_Base_Start_IT(&htim3);
		HAL_TIM_Base_Start_IT(&htim4);



		HAL_GPIO_WritePin( GPIOB, sevenSegment(misses), GPIO_PIN_SET);
		uint16_t rows[] = { A, B, C, D};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_GPIO_WritePin(GPIOA, MASK(rows[currentRow]), GPIO_PIN_RESET);
		currentRow = (currentRow+1)% 4;
		HAL_GPIO_WritePin(GPIOA, MASK(rows[currentRow]), GPIO_PIN_SET);
		delay(180);
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 15999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 2;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 15999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 124;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
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
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 60;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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
  htim4.Init.Prescaler = 15999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 124;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D0_Pin|D1_Pin|D2_Pin|D3_Pin
                          |D4_Pin|D5_Pin|D6_Pin|D7_Pin
                          |RS_Pin|RW_Pin|E_Pin|A_Pin
                          |B_Pin|C_Pin|D_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SD_Pin|SE_Pin|SF_Pin|SG_Pin
                          |SA_Pin|SB_Pin|SC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_Blue_Pin|LED_Red_Right_Pin|LED_Yellow_Pin|LED_Green_Right_Pin
                          |GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin : IO_Pin */
  GPIO_InitStruct.Pin = IO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(IO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Blue_Pin Green_Pin Yellow_Pin Red_Pin */
  GPIO_InitStruct.Pin = Blue_Pin|Green_Pin|Yellow_Pin|Red_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : D0_Pin D1_Pin D2_Pin D3_Pin
                           D4_Pin D5_Pin D6_Pin D7_Pin
                           RS_Pin RW_Pin E_Pin A_Pin
                           B_Pin C_Pin D_Pin */
  GPIO_InitStruct.Pin = D0_Pin|D1_Pin|D2_Pin|D3_Pin
                          |D4_Pin|D5_Pin|D6_Pin|D7_Pin
                          |RS_Pin|RW_Pin|E_Pin|A_Pin
                          |B_Pin|C_Pin|D_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Reset_Pin */
  GPIO_InitStruct.Pin = Reset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Reset_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : C1_Pin C2_Pin C3_Pin */
  GPIO_InitStruct.Pin = C1_Pin|C2_Pin|C3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SD_Pin SE_Pin SF_Pin SG_Pin
                           SA_Pin SB_Pin SC_Pin */
  GPIO_InitStruct.Pin = SD_Pin|SE_Pin|SF_Pin|SG_Pin
                          |SA_Pin|SB_Pin|SC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Blue_Pin LED_Red_Right_Pin LED_Yellow_Pin LED_Green_Right_Pin
                           PC10 PC11 */
  GPIO_InitStruct.Pin = LED_Blue_Pin|LED_Red_Right_Pin|LED_Yellow_Pin|LED_Green_Right_Pin
                          |GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

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
