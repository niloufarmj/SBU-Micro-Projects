#include "adc.h"
#include "gpio.h"
#include "math.h"
#include "main.h"
unsigned char seg[]={0x40,
										0x79,
										0x24,
										0x30,
										0x19,
										0x12,
										0x02,
										0x78,
										0x00,
										0x10,
										0XFF
								};
unsigned int i;
unsigned int adc[1410];
float buffer;
void delay_us(unsigned long int n){
	unsigned long int x=0;
		unsigned long fCPU=3200000;
		n=n*(fCPU / 50);
		while (n > 0){
		n= n-1;
	}
}
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void showOn7Segment (unsigned int data_out){	                          
	 (data_out & 1 )== 0?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	(data_out & 2)== 0? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET) : HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
	(data_out & 4)== 0? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET) :HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
	 (data_out & 8)== 0?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET) :HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
	(data_out & 16)== 0? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
	(data_out & 32)== 0 ? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
	 (data_out & 64)== 0 ? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);
	(data_out & 128)== 0 ? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET) : HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
	 (data_out & 256)== 0 ? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
	(data_out & 512)== 0? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET):	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
	(data_out & 1024)== 0 ? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
	(data_out & 2048)== 0 ? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
	 (data_out & 4096)== 0?	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
	(data_out & 8192)==0? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
	(data_out & 16384)==0 ? HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
 };
float goertzel_mag(int numberOfSamples,int feq,int sampleRate){
	int     k,i;
	float  floatnumberOfSamples,
	omega,
	sine,
	cosine,
	coeff,
	q0,q1,q2,
	magnitude,
	real,
	imag,
	scalingFactor = numberOfSamples / 2.0;
	floatnumberOfSamples = (float) numberOfSamples;
	
	
	k = (int) (0.5 + ((floatnumberOfSamples * feq) / sampleRate));
	omega = (2.0 * 3.1415* k) / floatnumberOfSamples;
	
	
	sine = sin(omega);
	cosine = cos(omega);
	coeff = 2.0 * cosine;
	q0= q1 = q2 = 0;
	
	
	int index = 0 ; 
	while( index <numberOfSamples){
			q2 = q1;
			q1 = q0;
			q0 = coeff * q1 - q2 + adc[index];
			index++;
	}
	real = (q0 - q1 * cosine) / scalingFactor;
	imag = (-q1 * sine) / scalingFactor;

	magnitude = sqrtf(real*real + imag*imag);
	return magnitude;
};
int main(void){
		HAL_Init();
		SystemClock_Config(); /* Configure the system clock */
		MX_GPIO_Init();
		MX_ADC1_Init();
		showOn7Segment(0XFFFF);
		hadc1.Init.NbrOfConversion=1;
		HAL_ADC_Init(&hadc1);						
int numberOfShift = 7;	
	float mag = 80.0 ; 
		while (1){
		 for(i=0;i<1400;i++){
					HAL_ADC_Start(&hadc1);
					if(HAL_ADC_PollForConversion(&hadc1,1)==HAL_OK)
										adc[i]=HAL_ADC_GetValue(&hadc1);                                         																			 																							 
					HAL_ADC_Stop(&hadc1); 
					};	
						i=0;
						if((goertzel_mag(1400,697,25000))> mag){
							 if((goertzel_mag(1400,1209,25000))> mag)
														showOn7Segment((seg[1/10])+((seg[1])<< numberOfShift));
							 if((goertzel_mag(1400,1336,25000))> mag)
														showOn7Segment((seg[2/10])+((seg[2])<< numberOfShift));
							 if((goertzel_mag(1400,1477,25000))> mag)
													showOn7Segment((seg[3/10])+((seg[3])<< numberOfShift));
							 if((goertzel_mag(1400,1633,25000))> mag)
															showOn7Segment((seg[4/10])+((seg[4])<< numberOfShift));
							 };
						i=0;
						if((goertzel_mag(1400,770,25000))> mag){
							 if((goertzel_mag(1400,1209,25000))> mag)
													showOn7Segment((seg[5/10])+((seg[5])<< numberOfShift));
							 if((goertzel_mag(1400,1336,25000))> mag)
													showOn7Segment((seg[6/10])+((seg[6])<< numberOfShift));
							 if((goertzel_mag(1400,1477,25000))> mag)
													showOn7Segment((seg[7/10])+((seg[7])<< numberOfShift));
							 if((goertzel_mag(1400,1633,25000))> mag)
												showOn7Segment((seg[8/10])+((seg[8])<< numberOfShift));
							 };
						i=0;
						if((goertzel_mag(1400,852,25000))> mag){
								 if((goertzel_mag(1400,1209,25000))> mag)
															showOn7Segment((seg[9/10])+((seg[9])<< numberOfShift));
								 if((goertzel_mag(1400,1336,25000))> mag)
															showOn7Segment((seg[10/10])+((seg[0])<< numberOfShift));
								 if((goertzel_mag(1400,1477,25000))> mag)
													   	showOn7Segment((seg[11/10])+((seg[1])<< numberOfShift));
								 if((goertzel_mag(1400,1633,25000))> mag)
															showOn7Segment((seg[12/10])+((seg[2])<< numberOfShift));
								 };
						i=0;
						if((goertzel_mag(1400,941,25000))> mag){
								 if((goertzel_mag(1400,1209,25000))> mag)
															showOn7Segment((seg[13/10])+((seg[3])<< numberOfShift));
								 if((goertzel_mag(1400,1336,25000))> mag)
														showOn7Segment((seg[14/10])+((seg[4])<< numberOfShift));
								 if((goertzel_mag(1400,1477,25000))> mag)
														showOn7Segment((seg[15/10])+((seg[5])<< numberOfShift));
								 if((goertzel_mag(1400,1633,25000))> mag)
														showOn7Segment((seg[16/10])+((seg[6])<< numberOfShift));
								 };									
						};
		}
void SystemClock_Config(void){
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}
void Error_Handler(void){
                        }
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line){ 
                                                }
#endif

