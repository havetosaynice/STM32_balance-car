#ifndef _MOTOR_H_
#define _MOTOR_H_

void Motor_Init(void);
void Judge_Move(int PWM1,int PWM2);
void PWM_Limition(int Max,int *PWM1,int *PWM2);
void Lwheel_straight(void);
void Lwheel_Back(void);
void Rwheel_straight(void);
void Rwheel_Back(void);

#endif
