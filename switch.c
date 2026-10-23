#include "stm32f401rbt6.h"
#include "gps_extract.h"
#include "switch.h"


int ent_sw_flag=0; //ENTER SWITCH COUNTER VARIABLE
int up_sw_flag=0;

void ent_switch(void)
{
	RCC_AHB1ENR |=(0x01<<2);	//set 2nd bit in RCC_AHB1ENR to enable port c clock
	
	GPIOC_PUPDR &=(0xFFCFFFFF); //clear 21 & 20 bits in PUPDR
	GPIOC_PUPDR |=(0x00100000);  //Load 01 bits into 21 - 20 bit fields in PUPDR
}

void ent_sys_conf(void)
{
	RCC_APB2ENR |=(0x01<<14); //set 14 bit in RCC_APB2ENR to enable SYCONFIG clock
	
	SYSCFG_EXTICR3 &=(0xFFFFF0FF); //clear the 11,10,9,8 bits in sys_conf
	SYSCFG_EXTICR3 |=(0x00000200);  //set the 11,10,9,8 bits in sys_conf
	
	EXTI_FTSR |=(0x00000400); //set the 10th bit in pc10 configure IN FSTR
	EXTI_IMR |=(0x000000400); //set the 10th bit in pc10 configure  IN IMR
	
	NVIC_ISER1 |=(0x00000100); //set the 8th bit in pc10 configure (40 PIN 2ND REGISTER 8 PIN)
	
}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI_PR &0X01<<10))
	{
		ent_sw_flag=1;
		EXTI_PR |=0X01<<10;
	}
}


//--------PC08_UPSWITCH------------//

void up_switch(void)
{
	RCC_AHB1ENR |=(0x01<<2);	   //set 2nd bit in RCC_AHB1ENR to enable port c clock
	
	//SWITCH_UP 
	GPIOC_PUPDR &=(0xFFFCFFFF);  //clear 17,16 bits in GPIOC_PORT C
	GPIOC_PUPDR |=(0x00010000);	//Load 01 bits into 17,16 bit fields in GIPO_MODE

}
void up_sys_config(void)
{
		RCC_APB2ENR |=0x01<<14;					//set 14th bit in RCC_APB2ENR to enable port b clock 
		SYSCFG_EXTICR3 &= 0xFFFFFFF0;   //clear 3-0 bits of SYSCFG_EXTICR3   
		SYSCFG_EXTICR3 |= 0x00000002;   //load "0010" into 3-0 bit fields of SYSCFG_EXTICR3 to congigure PC10,PC9,PC8 as external interrupts respectively
		
		EXTI_FTSR |=(0x01<<8);         //set 8 bits of EXTI_FTSR to configure PC10,PC9,PC8 as falling edge interrupts
		EXTI_IMR |=(0X01<<8);         //set 8 bits of EXTI_IMR to enable EXTI15_10 and EXTI9_5 interrupt line
		
		NVIC_ISER0 |=(0x01 << 23);    //set 23th bit in NVIC_ISTR1 to enable NVIC 40th interrupt line
}

void  EXTI9_5_IRQHandler(void)
{
	if(EXTI_PR & (0x01<<8))//CHECKING CONDITION PR FOR 10 BIT
	{
	  
		up_sw_flag=1;
		EXTI_PR |=(0x01<<8); //SET THE 10TH BIT IN PR
		
	}
}






	