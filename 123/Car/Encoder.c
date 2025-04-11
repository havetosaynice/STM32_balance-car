#include "stm32f10x.h"                  // Device header

/*
 PB6 => M2A =>M2 => ���� TIM4
 PB7 => M2B
 */
 
 /*
 PA6 => M1A => M1 => ���� TIM3
 PA7 => M1B 
 */
void Encoder_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//����TIM3��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//����GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//��PA6��PA7���ų�ʼ��Ϊ��������

	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;               
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);            
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	/*���벶���ʼ��*/
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
	
	/*�������ӿ�����*/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);																
	
	/*TIMʹ��*/
	TIM_Cmd(TIM3, ENABLE);			//ʹ��TIM3����ʱ����ʼ����
	TIM_Cmd(TIM4, ENABLE);
}

/**
  * ��    ������ȡ������������ֵ
  * ��    ������
  * �� �� ֵ�����ϴ˵��ô˺����󣬱�����������ֵ
  */
int16_t EncoderM2_Get(void)
{
	/*ʹ��Temp������Ϊ�м̣�Ŀ���Ƿ���CNT��������*/
	int16_t Temp;
	Temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);
	return Temp;
}

int16_t EncoderM1_Get(void)
{
	/*ʹ��Temp������Ϊ�м̣�Ŀ���Ƿ���CNT��������*/
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp;
}
