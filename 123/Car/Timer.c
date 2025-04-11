#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "inv_mpu.h"
#include "Delay.h"
#include "Motor.h"
#include "PID.h"
#include "OLED.h"

float Pitch,Roll,Yaw;

float measure,calcu;   //测量值和理论值  直立环
extern float zhongzhi; //roll理论值（小车平衡时的角度）

int16_t Velocity_measure;          //速度测量值（编码器脉冲数，非真实速度）
int16_t Velocity_Fact;
extern int velocity_calcu; //速度理论值   速度环
int PWM0,PWM1,PWM2,PWM_Yaw;               //PWM值

float yaw_measure; //转向测量值   转向环
extern float yaw_calcu;  ///yaw角理论值

extern int Motor_Flag; //电机使能标志（自动）
extern int Motor;//电机使能标志（手动）

void Timer_Init(void)
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // 开启TIM1的时钟

    /* 配置时钟源 */
    TIM_InternalClockConfig(TIM1); // 选择TIM1为内部时钟，若不调用此函数，TIM默认也为内部时钟

    /* 时基单元初始化 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; // 定义结构体变量
		TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      // 时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;               // 计数周期，即ARR的值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 360 - 1;             // 预分频器，即PSC的值
//    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;           // 重复计数器，高级定时器才会用到
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);             // 将结构体变量交给TIM_TimeBaseInit，配置TIM1的时基单元

    /* 中断输出配置 */
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update); // 自动清除中断

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); // 开启TIM1的更新中断

    /* NVIC中断分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置NVIC为分组2

    /* NVIC配置 */
    NVIC_InitTypeDef NVIC_InitStructure;                    // 定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;      // 选择配置NVIC的TIM1更新线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          // 指定NVIC线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 指定NVIC线路的抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;       // 指定NVIC线路的响应优先级为1
    NVIC_Init(&NVIC_InitStructure);                          // 将结构体变量交给NVIC_Init，配置NVIC外设

    /* TIM使能 */
    TIM_Cmd(TIM1, ENABLE); // 使能TIM1，定时器开始运行
}


void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET) // 判断是否是TIM1的更新事件触发的中断
    {
			
							if(MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw)==0)
				{
					//【1】直立参数
				measure = Roll;
				calcu = zhongzhi;  
					//【2】速度参数
				Velocity_measure = (EncoderM2_Get()+EncoderM1_Get())/2;
					//真实速度
				Velocity_Fact = Velocity_measure*200/1320*67*3.1416;  //毫米每秒
					//【3】转向参数
				yaw_measure=Pitch;
					//只有直立的话 需要将PWM0置为相反数
				PWM0 = -(Vertical_PID_Value(measure, calcu) + Velocity_PID_Value(Velocity_measure,velocity_calcu)); 
				//PID计算：直立+速度+转向
				PWM_Yaw =Turn_PID_Value(yaw_measure,yaw_calcu);
				PWM1 = PWM0 + PWM_Yaw;
				PWM2 = PWM0 - PWM_Yaw;
				
				PWM_Limition(7000,&PWM1,&PWM2);//限幅

				if(Motor_Flag && Motor) Judge_Move(PWM1,PWM2); //给电机PWM
					else
					Judge_Move(0,0);   //关闭电机	
				}	
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update); // 清除中断标志位
    }
}

