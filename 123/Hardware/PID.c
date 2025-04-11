#include "stm32f10x.h"                  // Device heade
#include "PID.h"

extern float Kp,Ki,Kd;   //ֱ��������
float err;               //�˴����
float last_err;          //�ϴ����
float err_sum=0;         //����ۼ�
float err_difference;    //���Ĳ�ֵ

extern float VKp,VKi;    //�ٶȻ�����
float filt_velocity;     //�˲�����ٶ�
float velocity;          //�ٶ����
float last_filt_velocity;//��һ�ε��˲�����ٶ�
float velocity_sum=0;    //�ٶȵ��ۼ�

extern float TKp;
float yaw_err;           //ת�����


//ֱ������
int Vertical_PID_Value(float measure,float calcu)
{

	err = measure - calcu;             //���
	err_sum+=err;                      //�����ۼ�
	err_difference = err - last_err;   //���Ĳ�ֵ
	last_err = err;                    //�˴�����¼Ϊ���ϴ���
	
	return Kp*err + Ki*err_sum + Kd*err_difference;
}


//�ٶȻ���
int Velocity_PID_Value(int velocity_measure,int velocity_calcu)
{
	float a=0.3;   	//�˲�ϵ������ӳ�˲��̶ȣ�
	velocity = velocity_measure - velocity_calcu;
	filt_velocity = a*velocity + (1-a)*last_filt_velocity; //һ���ٶ��˲�
	velocity_sum +=  filt_velocity;                        //�ٶȵ��ۼ�
	I_xianfu(3000);                                        //�ۼ��޷�
	last_filt_velocity = filt_velocity;                    //�˴��ٶȼ�¼Ϊ���ϴ��ٶȡ�

	return VKp*filt_velocity + VKi*velocity_sum;
}

//ת�򻷣�
int Turn_PID_Value(float yaw_measure, float yaw_calcu)
{
	yaw_err = yaw_measure - yaw_calcu;
	
	return  TKp*yaw_err;
}

//I�޷���
void I_xianfu(int max)
{
	if(velocity_sum>max)  velocity_sum=max;
	if(velocity_sum<-max) velocity_sum=-max;
}

