#include "stm32f4xx.h"
#include<stdio.h>

#define MASK(x) (1UL << (x))

void usart2_init(uint32_t pclk, uint32_t baudrate){
	uint32_t temp = 0x00;
	uint32_t integer = 0x00;
	uint32_t fraction= 0x00;
	integer = ((25*pclk*1000000)/(4*baudrate));
	temp = (integer/100)<<4;
	fraction = integer-(100*(temp>>4));
	temp |= ((((fraction*16)+50)/100))&((uint8_t) 0x0F);
	
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	GPIOA->MODER |= GPIO_MODER_MODER2_1;//Pin2 mode AF
	GPIOA->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR2_1;
	GPIOA->AFR[0]|= 0x00000700;//Set the AF to AF7(USART1~3);
	SET_BIT(RCC->AHB1RSTR,RCC_APB1RSTR_USART2RST);
	CLEAR_BIT(RCC->AHB1RSTR,RCC_APB1RSTR_USART2RST);
	USART2->CR1 |= USART_CR1_UE;
	USART2->BRR = temp;
	USART2->CR1 |= USART_CR1_TE;
}


void usart2_send(uint16_t data) {
	USART2->DR=(data&(uint16_t)0x01ff);
	while(!READ_BIT(USART2->SR, USART_SR_TC)){}
}

void delay_ms(int n) {
	for (int i = 0; i < n; i++) 
		for (int j = 0; j < 5*n; j++) {}
}

int main (void) {
	int result;
	
	/* set up pin PA5 for LED */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 						/* enable GPIOA clock */
	GPIOA->MODER &= ~0x00000C00;  /* clear pin mode */
	GPIOA->MODER |= 0x00000400;   /* set pin to output mode */
	
	/* set up pin PA1 for analog input */
	RCC->AHB1ENR |= 1; 						/* enable GPIOA clock */
	GPIOA->MODER |= 0xC; 					/* PA1 analog */
	
	/* setup ADC1 */
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 	/* enable ADC1 clock */
	ADC1->CR2 = 0; 								/* SW trigger */
	ADC1->SQR3 = 1; 							/* conversion sequence starts at ch 1 */
	ADC1->SQR1 = 0; 							/* conversion sequence length 1 */
	ADC1->CR2 |= ADC_CR2_ADON; 							/* enable ADC1 */
	
	usart2_init(16, 9600);
	
	while (1) {
		ADC1->CR2 |= ADC_CR2_SWSTART;    /* start a conversion */
		while(!(ADC1->SR & 2)) {}   /* wait for conv complete */
		result = ADC1->DR; 					/* read conversion result */
		result = result*0.124;
			
		/*send number*/
		int size = 0;
		int num = result;
		while (result > 0) {
			result /= 10;
			size ++;
		}
	
		char number_str[size];
		sprintf(number_str, "%u", num);
	
		for (int i = 0; i < size; i++) {
			usart2_send(number_str[i]);
		}
		
		/*next line*/
		char* string = "\n\r";
		for (int i = 0; i < 4; i++) {
			usart2_send(string[i]);
		}
		
		delay_ms(1000);
	}
}

