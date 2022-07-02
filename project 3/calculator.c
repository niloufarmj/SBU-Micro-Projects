#include <stm32f4xx.h>
#define RS 0x01     // Pin mask for reg select 
#define RW 0x02     // Pin mask for read/write
#define EN 0x04     // Pin mask for enable 
#define MASK(x) (1UL << (x))

volatile static long long int firstOperand = 0 , secondOperand = 0 ,result = 0 ,isFirstOPExist = 0;
volatile static unsigned int zero_counter = 0;
volatile static char operation = '0';
volatile static unsigned int equalClicked = 0;
volatile static unsigned int LCD_line = 0;
volatile static int numIncBtn1Cick = 0, numOfDecCick = 0;

void delay(int n); 
void printNames(void);
void LCD_command(unsigned char command); 
void LCD_data(char data);
void setGPIOConfig(void);
void enableInterrupts(void);
void initializingLCD(void);
void initializingPorts(void);
long long int reverse(long long int);
void LCD_display(long long int );
void calculateResult(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void increseResult(long long int num);
void decreaseResult(long long int num);
void clearSecondRow(void);

void increseResult(long long int num){
	num += numIncBtn1Cick;
	clearSecondRow();
	LCD_display(num);
	numIncBtn1Cick = 0 ;
	result = num;
}

void decreaseResult(long long int num){
	num -= numOfDecCick;
	clearSecondRow();
	LCD_display(num);
	numOfDecCick = 0;
	result = num;
}

void clearSecondRow(){
	LCD_command(0xC0);
	for(int i =0 ; i<16;i++){
		LCD_data(' ');
	}
}

void printNames(void){
	char source[] = "Mousavi-MoradiJ";
	int len = strlen(source);
	for (int i = 0; i < len; i++) {
		LCD_data(source[i]);
	}
	delay(500);
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
void LCD_display(long long int num){	
	LCD_command(0xC0);
	delay(13);
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
void calculateResult(void){
	switch(operation){
				case '*':result = firstOperand * secondOperand;break;
				case '+':result = firstOperand + secondOperand;break;
				case '/':result = firstOperand / secondOperand;break;
				case '-':result = firstOperand - secondOperand;break;
			}
			//result is the first number for continue typing or calculating with it
			firstOperand = result;
			isFirstOPExist = 0;
			secondOperand = 0;
			equalClicked = 0;
			
			LCD_command(0xC0);
			for(int i =0 ; i<16;i++){
				LCD_data(' ');
			}
			LCD_display(result);   
}

void EXTI0_IRQHandler(void){
	
	//clearing pending bits
	EXTI->PR |= MASK(0);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	switch(LCD_line){
		case 0:{
			LCD_data('7');
			if(isFirstOPExist == 0){
				firstOperand *= 10;
				firstOperand += 7;
			}
			else{
				secondOperand *= 10;
				secondOperand += 7;
			}
			break;
		}
		case 1:{
		LCD_data('8');
			if(isFirstOPExist == 0){
				firstOperand *= 10;
				firstOperand += 8;
			}
			else{
				secondOperand *= 10;
				secondOperand += 8;
			}
			break;
		}
		case 2:{
		LCD_data('9');
			if(isFirstOPExist ==0){
				firstOperand *= 10;
				firstOperand += 9;
			}
			else{
				secondOperand *= 10;
				secondOperand += 9;
			}
			break;
		}
		case 3:{
			LCD_data('/');
			isFirstOPExist = 1;
			operation = '/';
			break;
		}
	}
}
void EXTI1_IRQHandler(void){
	//clearing pending bits
	EXTI->PR |= MASK(1);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	switch(LCD_line){
		case 0:{
			LCD_data('4');
			if(isFirstOPExist == 0){
				firstOperand *= 10;
				firstOperand += 4;
			}
			else{
				secondOperand *= 10;
				secondOperand += 4;
			}
			break;
		}
		case 1:{
		LCD_data('5');
			if(isFirstOPExist == 0){
				firstOperand *= 10;
				firstOperand += 5;
			}
			else{
				secondOperand *= 10;
				secondOperand += 5;
			}
			break;
		}
		case 2:{
		LCD_data('6');
			if(isFirstOPExist ==0){
				firstOperand *= 10;
				firstOperand += 6;
			}
			else{
				secondOperand *= 10;
				secondOperand += 6;
			}
			break;
		}
		case 3:{
			LCD_data('*');
			isFirstOPExist = 1;
			operation = '*';
			break;
		}
	}
}
void EXTI2_IRQHandler(void){
	//clearing pending bits
	EXTI->PR |= MASK(2);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	switch(LCD_line){
		case 0:{
			LCD_data('1');
			if(isFirstOPExist == 0){
				firstOperand *= 10;
				firstOperand += 1;
			}
			else{
				secondOperand *= 10;
				secondOperand += 1;
			}
			break;
		}
		case 1:{
		LCD_data('2');
			if(isFirstOPExist == 0){
				firstOperand *= 10;
				firstOperand += 2;
			}
			else{
				secondOperand *= 10;
				secondOperand += 2;
			}
			break;
		}
		case 2:{
		LCD_data('3');
			if(isFirstOPExist ==0){
				firstOperand *= 10;
				firstOperand += 3;
			}
			else{
				secondOperand *= 10;
				secondOperand += 3;
			}
			break;
		}
		case 3:{
			LCD_data('-');
			isFirstOPExist = 1;
			operation = '-';
			break;
		}
	}
}
void EXTI3_IRQHandler(void){
	//clearing pending bits
	EXTI->PR |= MASK(3);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	
	switch(LCD_line){
		case 0:{
			LCD_command(1);
			firstOperand = 0;
			secondOperand = 0;
			isFirstOPExist = 0;
			break;
		}
		case 1:{
		LCD_data('0');
			if(isFirstOPExist == 0){
				firstOperand *= 10;
				firstOperand += 0;
			}
			else{
				secondOperand *= 10;
				secondOperand += 0;
			}
			break;
		}
		case 2:{
			equalClicked = 1;
			break;
		}
		case 3:{
			LCD_data('+');
			isFirstOPExist = 1;
			operation = '+';
			break;
		}
	}
}

void EXTI4_IRQHandler(void){
	EXTI->PR |= MASK(EXTI_PR_PR4_Pos);
	NVIC_ClearPendingIRQ(EXTI4_IRQn);
	//numOfDecCick++;
	//decreaseResult(result);
		firstOperand--;
		isFirstOPExist = 0;
		secondOperand = 0;
		equalClicked = 0;
		LCD_display(firstOperand);  
}

void EXTI9_5_IRQHandler(void){
	EXTI->PR |= MASK(EXTI_PR_PR5_Pos);
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
//numIncBtn1Cick++;
	//increseResult(result);
	firstOperand++;
		isFirstOPExist = 0;
		secondOperand = 0;
		equalClicked = 0;
	LCD_display(firstOperand);  
}




void initializingLCD(void) {
    initializingPorts();
	  // initialization sequence
    delay(400);            
    LCD_command(0x30);
    delay(100);
    LCD_command(0x30);
    delay(10);
    LCD_command(0x30);
		// set 8-bit data, 2-line, 5x7 font
		LCD_command(0x38);
		//ENTRY MODE	
		LCD_command(0x06);
		// clear screen, move cursor to home	
		LCD_command(0x01);     
		LCD_command(0x80);
		// turn on display
		LCD_command(0x0F);				
}

void setGPIOConfig(){
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN; /* Initialize needed GPIOs and set ports mode appropriately  */
		GPIOA->MODER = 0x00555555;
		GPIOA->OTYPER = 0x0000;
		GPIOA->PUPDR = 0x00000000;
		GPIOA->ODR = 0x00000000;
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
		GPIOB->MODER = 0x00005500;
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;
		GPIOC->MODER = 0x00000000;
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
		//CONNECTING from PB0 tO PB3, to ICR 
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB; 
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PB; 
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB; 
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB;
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PC; 
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PC;
}

void enableInterrupts(){ // ENABLING INTERRUPTS
		EXTI->IMR |= (MASK(0));
		EXTI->IMR |= (MASK(1));
		EXTI->IMR |= (MASK(2));
		EXTI->IMR |= (MASK(3));
		EXTI->IMR |= (MASK(4));
		EXTI->IMR |= (MASK(5));
		EXTI->RTSR |= (MASK(0));
		EXTI->RTSR |= (MASK(1));
		EXTI->RTSR |= (MASK(2));
		EXTI->RTSR |= (MASK(3));
		EXTI->RTSR |= (MASK(4));
		EXTI->RTSR |= (MASK(5));
	__enable_irq();
		NVIC_ClearPendingIRQ(EXTI0_IRQn);
		NVIC_ClearPendingIRQ(EXTI1_IRQn);
		NVIC_ClearPendingIRQ(EXTI2_IRQn);
		NVIC_ClearPendingIRQ(EXTI3_IRQn);
		NVIC_ClearPendingIRQ(EXTI4_IRQn);
		NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
		NVIC_EnableIRQ(EXTI0_IRQn);
		NVIC_EnableIRQ(EXTI1_IRQn);
		NVIC_EnableIRQ(EXTI2_IRQn);
		NVIC_EnableIRQ(EXTI3_IRQn);
		NVIC_EnableIRQ(EXTI4_IRQn);
		NVIC_EnableIRQ(EXTI9_5_IRQn);
}
void initializingPorts(void) {
		setGPIOConfig();
		enableInterrupts();
}

void LCD_command(unsigned char command) {
		// RS = R/W = 0
	  GPIOA->ODR &= ~(RS|RW);		
		GPIOA->ODR &= (0X007);
		// putting command on data bus
    GPIOA->ODR |= command << 3;   
		// pulse EN high and clear it	
    GPIOA->ODR |= EN;						
    delay(13);
    GPIOA->ODR &= ~(EN);  
		//The first and second commands just only  needs more delay
    if (command < 4)
        delay(26);         
    else
        delay(13);  
}

void LCD_data(char data) {
		// RS = 1
    GPIOA->ODR |= RS;
		// R/W = 0	
    GPIOA->ODR &= ~(RW);            
		GPIOA->ODR &= (0X007);
		// put data on data bus
    GPIOA->ODR |= data << 3;
		// pulse EN high
    GPIOA->ODR |= EN;						
    delay(13);
		// clear EN 
    GPIOA->ODR &= ~(EN); 
    delay(13);
}


void delay(int n) {
    volatile int i;
    for (; n > 0; n--)
        for (i = 0; i < 3000; i++) ;
}

int main(void) {
  initializingLCD();
	printNames();
	LCD_command(0xC0); // Go to second Line
  while(1) {
			
				GPIOB->ODR &= 0xF; //activating proper columns
        LCD_line = (LCD_line+1)%4;
				GPIOB->ODR |= MASK(LCD_line)<<4;
				if(equalClicked == 1){
						calculateResult();
				}
        delay(25);
    }
}

