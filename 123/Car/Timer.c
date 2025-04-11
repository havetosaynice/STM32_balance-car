#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "inv_mpu.h"
#include "Delay.h"
#include "Motor.h"
#include "PID.h"
#include "OLED.h"

float Pitch,Roll,Yaw;

float measure,calcu;   //����ֵ������ֵ  ֱ����
extern float zhongzhi; //roll����ֵ��С��ƽ��ʱ�ĽǶȣ�

int16_t Velocity_measure;          //�ٶȲ���ֵ��������������������ʵ�ٶȣ�
int16_t Velocity_Fact;
extern int velocity_calcu; //�ٶ�����ֵ   �ٶȻ�
int PWM0,PWM1,PWM2,PWM_Yaw;               //PWMֵ

float yaw_measure; //ת�����ֵ   ת��
extern float yaw_calcu;  ///yaw������ֵ

extern int Motor_Flag; //���ʹ�ܱ�־���Զ���
extern int Motor;//���ʹ�ܱ�־���ֶ���

void Timer_Init(void)
{
    /* ����ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // ����TIM1��ʱ��

    /* ����ʱ��Դ */
    TIM_InternalClockConfig(TIM1); // ѡ��TIM1Ϊ�ڲ�ʱ�ӣ��������ô˺�����TIMĬ��ҲΪ�ڲ�ʱ��

    /* ʱ����Ԫ��ʼ�� */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; // ����ṹ�����
		TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      // ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // ������ģʽ��ѡ�����ϼ���
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;               // �������ڣ���ARR��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler = 360 - 1;             // Ԥ��Ƶ������PSC��ֵ
//    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;           // �ظ����������߼���ʱ���Ż��õ�
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);             // ���ṹ���������TIM_TimeBaseInit������TIM1��ʱ����Ԫ

    /* �ж�������� */
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update); // �Զ�����ж�

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); // ����TIM1�ĸ����ж�

    /* NVIC�жϷ��� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����NVICΪ����2

    /* NVIC���� */
    NVIC_InitTypeDef NVIC_InitStructure;                    // ����ṹ�����
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;      // ѡ������NVIC��TIM1������
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          // ָ��NVIC��·ʹ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // ָ��NVIC��·����ռ���ȼ�Ϊ2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;       // ָ��NVIC��·����Ӧ���ȼ�Ϊ1
    NVIC_Init(&NVIC_InitStructure);                          // ���ṹ���������NVIC_Init������NVIC����

    /* TIMʹ�� */
    TIM_Cmd(TIM1, ENABLE); // ʹ��TIM1����ʱ����ʼ����
}


void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET) // �ж��Ƿ���TIM1�ĸ����¼��������ж�
    {
			
							if(MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw)==0)
				{
					//��1��ֱ������
				measure = Roll;
				calcu = zhongzhi;  
					//��2���ٶȲ���
				Velocity_measure = (EncoderM2_Get()+EncoderM1_Get())/2;
					//��ʵ�ٶ�
				Velocity_Fact = Velocity_measure*200/1320*67*3.1416;  //����ÿ��
					//��3��ת�����
				yaw_measure=Pitch;
					//ֻ��ֱ���Ļ� ��Ҫ��PWM0��Ϊ�෴��
				PWM0 = -(Vertical_PID_Value(measure, calcu) + Velocity_PID_Value(Velocity_measure,velocity_calcu)); 
				//PID���㣺ֱ��+�ٶ�+ת��
				PWM_Yaw =Turn_PID_Value(yaw_measure,yaw_calcu);
				PWM1 = PWM0 + PWM_Yaw;
				PWM2 = PWM0 - PWM_Yaw;
				
				PWM_Limition(7000,&PWM1,&PWM2);//�޷�

				if(Motor_Flag && Motor) Judge_Move(PWM1,PWM2); //�����PWM
					else
					Judge_Move(0,0);   //�رյ��	
				}	
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update); // ����жϱ�־λ
    }
}

