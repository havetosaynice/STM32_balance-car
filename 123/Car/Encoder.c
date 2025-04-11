#include "stm32f10x.h"                  // Device header

/*
 PB6 => M2A =>M2 => 右轮 TIM4
 PB7 => M2B
 */
 
 /*
 PA6 => M1A => M1 => 左轮 TIM3
 PA7 => M1B 
 */
void Encoder_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM3的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PA6和PA7引脚初始化为上拉输入

	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;               
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);            
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	/*输入捕获初始化*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							
	TIM_ICStructInit(&TIM_ICInitStructure);					
													
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				
	TIM_ICInitStructure.TIM_ICFilter = 0xF;						
	TIM_ICInit(TIM3, &TIM_ICInitStructure);		
	TIM_ICInit(TIM4, &TIM_ICInitStructure);	
	
	/*编码器接口配置*/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);																
	
	/*TIM使能*/
	TIM_Cmd(TIM3, ENABLE);			//使能TIM3，定时器开始运行
	TIM_Cmd(TIM4, ENABLE);
}

/**
  * 函    数：获取编码器的增量值
  * 参    数：无
  * 返 回 值：自上此调用此函数后，编码器的增量值
  */
int16_t EncoderM2_Get(void)
{
	/*使用Temp变量作为中继，目的是返回CNT后将其清零*/
	int16_t Temp;
	Temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);
	return Temp;
}

int16_t EncoderM1_Get(void)
{
	/*使用Temp变量作为中继，目的是返回CNT后将其清零*/
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp;
}
