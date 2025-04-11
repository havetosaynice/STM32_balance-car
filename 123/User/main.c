#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "Motor.h"
#include "Timer.h"
#include "Encoder.h"
#include "PWM.h"
//TAW 向左是+，向右是-
extern float Pitch,Roll,Yaw;//俯仰角默认跟中值一样，翻滚角，偏航角
float zhongzhi=-7; //直立环中值

extern int16_t Velocity_measure; //速度 脉冲
extern int16_t Velocity_Fact; //实际速度
extern float velocity_sum; 


int velocity_calcu = 0 ; //速度理论值
float yaw_calcu = 0;  //yaw角理论值

int  Motor_Flag;//电机使能标志（自动）：1使能 0失能
int  Motor=1;  //电机使能标志（手动）：1使能 0失能


//直立环PD参数: 先调节Kp，再调节Kd
float Kp=-420,Ki=0,Kd=-2000;  //调完速度环后精调
//-420   0   -2000

//速度环PI参数: 调节对于平衡车来说需要是为正反馈，及当速度理论值为0时，拨动轮子会增大速度

//Ki*200==KP
float VKp=-190,VKi=-0.94; 
//+190    0.95

//直立环KP参数：
float TKp = 80;



int main(void)
{
	OLED_Init();		//OLED初始化
	Motor_Init();
//	PWM_SetCompareM1(1000);
//	PWM_SetCompareM2(1000);
	Encoder_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	Timer_Init();
	
	while(1)
	{
		//【1】安全装置
			if(Roll<-30 || Roll>30)//已经倒了
	{
	  Motor_Flag = 0; 
		velocity_sum=0;
	  TIM_SetCounter(TIM4,0); //编码器计数值清零
		TIM_SetCounter(TIM3,0);
	}
    else Motor_Flag = 1;	
	
	
		OLED_ShowSignedNum(1,1,Velocity_measure,5);	
		OLED_ShowSignedNum(2,1,Velocity_Fact,5);
    OLED_ShowSignedNum(3,1,Roll,3);
		OLED_ShowSignedNum(4,1,Yaw ,3);
//			OLED_ShowNum(1,1,TIM_GetCounter(TIM3) ,5);//M1 计数从65535自减法的
//			OLED_ShowNum(2,1,TIM_GetCounter(TIM4) ,5);//M2 计数是从1自增的

	}
	
}


//		OLED_ShowNum(1,1,Temp,5);
//		MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);				//读取姿态信息(其中偏航角有飘移是正常现象)
//		MPU_Get_Gyroscope(&gx,&gy,&gz);
//		MPU_Get_Accelerometer(&ax,&ay,&az);


//		OLED_ShowSignedNum(2, 1, Pitch, 5);
//		OLED_ShowSignedNum(3, 1, Roll, 5);
//		OLED_ShowSignedNum(4, 1, Yaw, 5);
//		OLED_ShowSignedNum(2, 8, gx, 5);
//		OLED_ShowSignedNum(3, 8, gy, 5);
//		OLED_ShowSignedNum(4, 8, gz, 5);
