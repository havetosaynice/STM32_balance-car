#ifndef _PID_H_
#define _PID_H_

void I_xianfu(int max);
int Vertical_PID_Value(float measure,float calcu);
int Velocity_PID_Value(int velocity_measure,int velocity_calcu);
int Turn_PID_Value(float yaw_measure, float yaw_calcu);
#endif
