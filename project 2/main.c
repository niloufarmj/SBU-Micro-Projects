#include <stm32f4xx.h>

typedef enum {stop, resume, redTurn, greenTurn} state;


#define portA 0  // 7 Segment ports
#define portB 1
#define portC 2
#define portD 3
#define portE 4
#define portF 5
#define portG 6
#define LD 7  
#define RD 8
#define dp 11

#define Green 9 //Colors
#define Red 10



#define MASK(x) (1UL <<(x))  // Shift 1, x time to the left.
#define NMASK(x) (~(1UL <<(x)))  //  ~ mask 

//Static Variables 
 volatile  static state stateOfProgram = redTurn, tmpState = redTurn;  //These variables are used to control the state of the program.
 volatile  static int isOp = 0;  //this variable is used to control the buuton clicks.
 volatile  static int counter = 9, btn1Counter = 0, btn2Counter = 0 ;  // 7 segment numbers

 
 
//Function declerations
void setGPIOConfig(void);
void init(void);
void UpdateSevenSeg(void);
void Delay(volatile int);
void ChangeLEDs(void);
void ChangeLEDsToRed(void);
void ChangeLEDsToGreen(void);
void ResetLED(void);

void setGPIOConfig(void){
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Setting GPIO Configuration
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOA -> MODER = 0x555555;
	GPIOB -> MODER &= 0x0;
	GPIOB -> PUPDR &= NMASK(portA);
	GPIOB -> PUPDR |= MASK(portB);
	GPIOB -> PUPDR &= NMASK(portC);
	GPIOB -> PUPDR |= MASK(portD);
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}


void init(void){
	GPIOA -> ODR |= MASK(Red);  //Initialization the program
	GPIOA -> ODR &= NMASK(Green);
	GPIOA -> ODR &= NMASK(LD);
	GPIOA -> ODR |= MASK(RD);
	GPIOA -> ODR |= MASK(dp);
}



void ResetLED(void){  // Resetting ALL 7 Segment Main LED
		GPIOA -> ODR &= NMASK (portA) & NMASK (portB) &NMASK (portC) &NMASK (portD) &NMASK (portE) &NMASK (portF) &NMASK (portG);
}



void UpdateSevenSeg(void){  //Changing the value of seven segment
	switch(counter){
		case 0 : GPIOA -> ODR |= 0x3F; break;
		case 1 : GPIOA -> ODR |= 0x06; break;
		case 2 : GPIOA -> ODR |= 0x5B; break;
		case 3 : GPIOA -> ODR |= 0x4F; break;
		case 4 : GPIOA -> ODR |= 0x66; break;
		case 5 : GPIOA -> ODR |= 0x6D; break;
		case 6 : GPIOA -> ODR |= 0x7D; break;
		case 7 : GPIOA -> ODR |= 0x07; break;
		case 8 : GPIOA -> ODR |= 0x7F; break;
		case 9 : GPIOA -> ODR |= 0x6F; break;
	}
}

void ChangeLEDsToRed(void){  //Turning on the red LED and turning off the green LED
			GPIOA -> ODR |= MASK(Red);
			GPIOA -> ODR &= NMASK(Green);
			GPIOA -> ODR &= NMASK(LD);
			GPIOA -> ODR |= MASK(RD);
}

void ChangeLEDsToGreen(void){  //Turning on the green LED and turning off the red LED
			GPIOA -> ODR &= NMASK(Red);
			GPIOA -> ODR |= MASK(Green);
			GPIOA -> ODR |= MASK(LD);
			GPIOA -> ODR &= NMASK(RD);
}


void ChangeLEDs(void){ // Changing 7 Segment Main LEDs
	counter = 9;
	if(stateOfProgram == stop){
		if(tmpState == redTurn){
			ChangeLEDsToGreen();
			tmpState = greenTurn;
		}else if (tmpState == greenTurn){
			ChangeLEDsToRed();
			tmpState = redTurn;
		}
	}
	else if(stateOfProgram == redTurn){
		ChangeLEDsToGreen();
		stateOfProgram = greenTurn;
	}
	else if (stateOfProgram == greenTurn ){
		ChangeLEDsToRed();
		stateOfProgram = redTurn;
	}	
}


void Delay(volatile int time){
	for(int i = 0; i< time; i++){
		for ( int j = 0 ; j < time ; j++){ // mask(0) is used for button 1, and mask(1) for button 2
				if(GPIOB -> IDR & MASK(0) ){ 
					btn1Counter++;
					do{ //Nothing to do, just is used for handling the clicks
					}while(GPIOB -> IDR & MASK(0));
					if(btn1Counter == 2){
						isOp = 1; // Changing the color event
						break;
					}			
				}else if (GPIOB -> IDR & MASK(1)){
						btn2Counter++;
						do{ //Nothing to do, just is used for handling the clicks
						}while(GPIOB -> IDR & MASK(1));
						if(btn2Counter == 3){
						tmpState = stateOfProgram;
 						stateOfProgram = stop;
						isOp = 0; // stop event
						}
						if(btn2Counter == 4){
							stateOfProgram = resume;
							isOp = 1;  // Resume event
							break;
						}
				}
		}
	}
}

int main(){
	setGPIOConfig();
	UpdateSevenSeg();
	init();
	while(1){
	Delay(1000);
	if(stateOfProgram == stop){
		if(isOp == 1){
			ResetLED();
			ChangeLEDs();
			UpdateSevenSeg();
			isOp = 0;
		}
		GPIOA -> ODR &= NMASK(dp); // Turnning Off 7 Segment Indicators
	}
	else{
			if(stateOfProgram == resume){
				stateOfProgram = tmpState;
				counter --;
				btn1Counter = btn2Counter = isOp =  0;
			}else{
				if(isOp == 0){
				counter -- ;
			}
				// Push Button Event
			else{
				btn1Counter = btn2Counter = isOp = 0;
				ChangeLEDs();
			}
		}	
		if(counter == 0){
			ChangeLEDs();
		}
		ResetLED();
		UpdateSevenSeg();	
		GPIOA -> ODR |= MASK(dp); // Turnning On 7 Segment Indicators
	}
	}
}