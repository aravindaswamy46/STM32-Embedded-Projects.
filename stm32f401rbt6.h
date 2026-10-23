/*****************************************************
* File:STM32F401RBT6
* Description:This file contain of base address and physcial address of stm32 peripherail and macros used

*******************************************************/

/*********************BASE ADDRES OF STM2*************/

//AHB1
#define AHB1_BASEADDR 0x40020000 //Base Address of AHB1 bus 

#define DMA2_BASEADDR 0x40026400


//APB1
#define APB1_BASEADDR 0x40000000 //Base Address of APB1 bus

//APB2
#define APB2_BASEADDR 0x40010000 //Base Address of APB2 bus

//RCC
#define RCC_BASEADDR 0x40023800 //Base Address of RCC


//GPIO
#define GPIOA_BASEADDR 0X40020000 //Base Address of GPIOA
#define GPIOB_BASEADDR 0X40020400 //Base Address of GPIOB
#define GPIOC_BASEADDR 0X40020800 //Base Address of GPIOC

//SYSCFG
#define SYSCFG_BASEADDR 0X40013800 //Base Address of SYSCFG

//EXTI
#define EXTI_BASEADDR 0X40013C00 //Base Address OF EXTI


//ADC
#define ADC1_BASEADDR 0x40012000    //BASE address of ADC1


/*************************************BASE ADDRESS OF CORTEX M4**************/
//NVIC 
#define NVIC_BASEADDR 0XE000E100 //Base Address of NVIC


#define STK_BASEADDR 0x0E000E010 //systick


/***************UART**********************************/

#define USART1_BASEADDR 0x40011000
#define USART2_BASEADDR 0x40004400 //base
#define USART6_BASEADDR 0x40011400

/***********************************************************************
*                         PHYSCIAL ADDRESS 
**************************************************************************/

//RCC_AHB1ENR
#define RCC_AHB1ENR 	*((int *)(RCC_BASEADDR + 0x30))
#define RCC_AHB2ENR 	*((int *)(RCC_BASEADDR + 0x34))
#define RCC_APB1ENR 	*((int *)(RCC_BASEADDR + 0x40))
#define RCC_APB2ENR 	*((int *)(RCC_BASEADDR + 0x44))

//GPIOA
#define GPIOA_MODER 	*((int *)(GPIOA_BASEADDR + 0X00))
#define GPIOA_ODR 		*((int *)(GPIOA_BASEADDR + 0X14))
#define GPIOA_PUPDR 	*((int *)(GPIOA_BASEADDR + 0X0C))
#define GPIOA_IDR 		*((int *)(GPIOA_BASEADDR + 0X10))
	
#define GPIOA_AFRH    *((int *)(GPIOA_BASEADDR + 0X24))

#define GPIOA_AFRL    *((int *)(GPIOA_BASEADDR + 0X20))
	
#define GPIOA_AFRL    *((int *)(GPIOA_BASEADDR + 0X20))

#define GPIOA_AFRH    *((int *)(GPIOA_BASEADDR + 0X24))


//GPIOB

#define GPIOB_MODER 	*((int *)(GPIOB_BASEADDR + 0X00))
#define GPIOB_ODR 		*((int *)(GPIOB_BASEADDR + 0X14))
#define GPIOB_PUPDR		*((int *)(GPIOB_BASEADDR + 0X0C))
#define GPIOB_IDR 		*((int *)(GPIOB_BASEADDR + 0X10))

//GPIOC
#define GPIOC_MODER 	*((int *)(GPIOC_BASEADDR + 0X00))
#define GPIOC_ODR 		*((int *)(GPIOC_BASEADDR + 0X14))
#define GPIOC_PUPDR 	*((int *)(GPIOC_BASEADDR + 0X0C))
#define GPIOC_IDR 		*((int *)(GPIOC_BASEADDR + 0X10))

//SYSCFG
#define SYSCFG_EXTICR3 *((int *)(SYSCFG_BASEADDR + 0X10))
	


//EXTI
#define EXTI_FTSR 		*((int *)(EXTI_BASEADDR  + 0X0C))
#define EXTI_IMR 			*((int *) (EXTI_BASEADDR + 0X00))
#define EXTI_PR 			*((int *) (EXTI_BASEADDR + 0X14))
	


	
	
/**************************************************************************
*                         PHYSCIAL ADDRESS OF CORTEX M4
**************************************************************************/
	
//NVIC
#define NVIC_ISER0 		*((int *) (NVIC_BASEADDR + 0X00))
#define NVIC_ISER1 		*((int *) (NVIC_BASEADDR + 0X04))
	
/*****************************sys***********************************/

//systick
#define STK_CTRL  *((int *)(STK_BASEADDR + 0X00))
#define STK_LOAD  *((int *)(STK_BASEADDR + 0X04))
#define STK_VAL   *((int *)(STK_BASEADDR + 0X08))
	
/**********************ADCA***********************/


#define ADC_SR 				*((int *)(ADC1_BASEADDR + 0X00))
#define ADC_CR1 			*((int *)(ADC1_BASEADDR + 0x04))
#define ADC_CR2 			*((int *)(ADC1_BASEADDR + 0x08))

#define ADC_SQR3		  *((int *)(ADC1_BASEADDR + 0x34))	
#define ADC_DR 				*((int *)(ADC1_BASEADDR + 0x4c))
	

/****************UART1***********************/
#define USART1_CR1     *((int *) (USART1_BASEADDR + 0X0C))
#define USART1_SR      *((int *) (USART1_BASEADDR + 0X00))   //SR
#define USART1_DR   	 *((int *) (USART1_BASEADDR + 0X04))   //DR
#define USART1_CR2     *((int *) (USART1_BASEADDR + 0X10))	
#define USART1_CR3     *((int *) (USART1_BASEADDR + 0X14))



/****************UART2***********************/
#define USART2_CR1    *((int *) (USART2_BASEADDR + 0X0C))
#define USART2_SR     *((int *) (USART2_BASEADDR + 0X00))   //SR
#define USART2_DR     *((int *) (USART2_BASEADDR + 0X04))   //DR
#define USART2_CR2    *((int *) (USART2_BASEADDR + 0X10))
#define USART2_CR3    *((int *) (USART2_BASEADDR + 0X14))
	

#define USART2_BRR  *((int *) (USART2_BASEADDR + 0X08))
#define USART1_BRR  *((int *) (USART1_BASEADDR + 0X08))

#define USART6_BRR  *((int *) (USART6_BASEADDR + 0X08))
/****************UART6***********************/
#define USART6_SR   *((int *) (USART6_BASEADDR + 0X00))   //SR
#define USART6_DR   *((int *) (USART6_BASEADDR + 0X04))   //DR
#define USART6_CR2  *((int *) (USART6_BASEADDR + 0X10))
#define USART6_CR3  *((int *) (USART6_BASEADDR + 0X14))

#define USART6_CR1  *((int *) (USART6_BASEADDR + 0X0C))	


/*****DMA******/

#define DMA2_S5CR     *((int *)    (DMA2_BASEADDR + 0x10+0x18*5))
#define DMA2_S5PAR    *((int *)    (DMA2_BASEADDR + 0x18+0x18*5))
#define DMA2_S5M0AR   *((int *)    (DMA2_BASEADDR + 0x1C+0x18*5))	

#define DMA2_S5NDTR   *((int *)    (DMA2_BASEADDR + 0x14+0x18*5))	
#define DMA2_LIFCR    *((int *)    (DMA2_BASEADDR + 0x08))	



//#define DMA2_S5CR *((int *) (DMA2_BASEADDR + 0x88))
//#define DMA2_S5PAR *((int *) (DMA2_BASEADDR + 0x90))
//#define DMA2_S5M0AR *((int *) (DMA2_BASEADDR + 0x94))	

//#define DMA2_S5NDTR *((int *) (DMA2_BASEADDR + 0x8C))	


	





