#include "stm32F4xx.h"

#define mask(x) (1U<<x)

void init(void);
void UART2_init(void);
unsigned int UART2_read(void);
void delayMs(int);
int calculate(int, int[]);
void SPI1_init(void);
void MAX_send_data(unsigned int data );
void write_digits(void);


int main (void) {	
	UART2_init();
	SPI1_init();
	
	while(1) {
		write_digits(); // read from UART and sent digits to max7227
	}
}


void UART2_init(void){
	RCC->APB1ENR |= 0x20000;  // Enable UART2 CLOCK
	RCC->AHB1ENR |= 0x01; // Enable GPIOA CLOCK
	
	GPIOA->MODER |= 0x000000A0;  // bits 7-4 = 1010 = 0xA --> Alternate Function for Pin PA2 & PA3
	GPIOA->OSPEEDR |= 0x000000F0;  // bits 7-4 = 1111 = 0xF --> High Speed for PIN PA2 and PA3
	GPIOA->AFR[0] |= 0x07700;  // bits 15-8=01110111=0x77  --> AF7 Alternate function for USART2 at Pin PA2 & PA3
	
	USART2->BRR = 0xD05;   // Baud rate = 4800bps, CLK = 16MHz
	
	USART2->CR1 = 1<<13;  // Enable USART
	USART2->CR1 &= ~(1<<12);  // M = 0; 8 bit word length
	
	USART2->CR1 |= (1<<2); // RE=1.. Enable the Receiver
	USART2->CR1 |= (1<<3);  // TE=1.. Enable Transmitter
	
	USART2->CR2 &= (1<<13);  // bits 13,12 = 10; 2 stop bits
}


unsigned int UART2_read(void) {
	while (!(USART2->SR & 0x0020));  // wait for RXNE bit to set
	return USART2->DR;
}

//-------------------------------------------------------------------------------------------------------------------

void SPI1_init(void){
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // enable SPI1 clock 
	
	// set Alternate Function mode for PA5 (sclk) & PA7 (MOSI:DIN for max7227) 
	GPIOA->MODER &= ~0x00000300U;
	GPIOA->MODER |= 0x00008800; 
	
	// set to AF5(SPI1)
	GPIOA->AFR[0] &= ~0xF0F00000; 
	GPIOA->AFR[0] |= 0x50500000; 
	
	// set PA4 to GPIO output mode
	GPIOA->MODER &= ~0x00000300U; 
	GPIOA->MODER |= 0x00000100; 
	
	SPI1->CR1 = 0x0; 
	SPI1->CR1 &= ~(SPI_CR1_CPHA | SPI_CR1_CPOL) ; // reset CPHA and CPOL 
	SPI1->CR1 |= SPI_CR1_MSTR ; // set master selection 
	SPI1->CR1 |= (SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2) ; // set baud rate to 111 : fpclk(16MHz)/256
	SPI1->CR1 |= SPI_CR1_SSI | SPI_CR1_SSM ;// software slave manage mode
	SPI1->CR1 |= SPI_CR1_SPE; // enable SPI_1 

	MAX_send_data(0xC01); // turn on 
	MAX_send_data(0xA1A); // set light intensity 
	MAX_send_data(0xB05); // scan limit 
	MAX_send_data(0x9ff); // decoding mode	
}


//-------------------------------------------------------------------------------------------------------------------

void MAX_send_data(unsigned int data ) {	
	while (!(SPI1->SR & SPI_SR_TXE)); // wait til TE empty 
	GPIOA->BSRR = 0x00100000; //slave clk to low 
	SPI1->DR = data >> 8 ;  // write command 
	while (!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = data & 0xff;  // write 8 bit data 
	while (SPI1->SR & SPI_SR_BSY); // wait for data send 
	GPIOA->BSRR = 0x0000010; //slave clk to high 
}

// write 2 digits to MAX7227
void write_digits(void){   
		unsigned int digit_1 = UART2_read() - '0'; 
		unsigned int digit_0 = UART2_read() - '0';

		MAX_send_data(mask(8) + digit_0); 
		MAX_send_data(mask(9) + digit_1);
}

//-------------------------------------------------------------------------------------------------------------------

void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 7000; i++) ;
}