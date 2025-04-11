#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Motor.h"


/* PB14 ==AIN1 =>M2 =>����
	PB15==AIN2
	
	PB12==BIN2 =>M1 =>����
	PB13==BIN1 
	*/
void Motor_Init()
{	
	PWM_Init(7200,1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��PA1��PA2���ų�ʼ��Ϊ�������
	
	/*����GPIO��ʼ�����Ĭ�ϵ�ƽ*/
	GPIO_SetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_13);				//����AIN1��BIN1����Ϊ�ߵ�ƽ
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_15);				//����BIN2��AIN2����λ�͵�ƽ

}
//PWM1 = M1 ,PWM2 = M2
void Judge_Move(int PWM1,int PWM2)
{
	if(PWM1>0)//��ǰ
	{
//		Lwheel_straight(); 
		Rwheel_straight();
		PWM_SetCompareM1(PWM1);
//		PWM_SetCompareM2(PWM2);
//		if(PWM-170<0)
//		{
//			PWM_SetCompareM2(-(PWM-170));
//		}
//		else
//		{
//		PWM_SetCompareM2(PWM-170);
//		}
		
	}else
	{
//		Lwheel_Back();
		Rwheel_Back();
//		if(PWM+170>0)
//		{
//			PWM_SetCompareM1(PWM+170);
//		}
//		else
//		{
//		 PWM_SetCompareM1(-(PWM+170));
//		}
		PWM_SetCompareM1(-PWM1);
//		PWM_SetCompareM2(-PWM2);
	}
	if(PWM2>0)//��ǰ
	{
		Lwheel_straight(); 
//		Rwheel_straight();
//		PWM_SetCompareM1(PWM1);
		PWM_SetCompareM2(PWM2);
//		if(PWM-170<0)
//		{
//			PWM_SetCompareM2(-(PWM-170));
//		}
//		else
//		{
//		PWM_SetCompareM2(PWM-170);
//		}
		
	}else
	{
		Lwheel_Back();
//		Rwheel_Back();
//		if(PWM+170>0)
//		{
//			PWM_SetCompareM1(PWM+170);
//		}
//		else
//		{
//		 PWM_SetCompareM1(-(PWM+170));
//		}
//		PWM_SetCompareM1(-PWM1);
		PWM_SetCompareM2(-PWM2);
	}
}

void PWM_Limition(int Max,int *PWM1,int *PWM2)
{
	if(*PWM1>Max)  *PWM1 = Max;
	if(*PWM1<-Max) *PWM1 =-Max;
	if(*PWM2>Max)  *PWM2 = Max;
	if(*PWM2<-Max) *PWM2 =-Max;
}


void Lwheel_straight() //M1
{
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}

void Lwheel_Back()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}
void Rwheel_straight() //M2
{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}

void Rwheel_Back()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}
