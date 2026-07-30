#include "stm32f10x.h"                  // Device header
#include "PID.h"

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->Target = 0;
    pid->Error = 0;
    pid->LastError = 0;
    pid->Integral = 0;
    pid->Output = 0;
}

float PID_Update(PID_TypeDef *pid, float actual)
{
    pid->Error = pid->Target - actual;
    pid->Integral += pid->Error;
    
    if(pid->Integral > 1000) pid->Integral = 1000;
    if(pid->Integral < -1000) pid->Integral = -1000;
    
    float derivative = pid->Error - pid->LastError;
    
    pid->Output = pid->Kp * pid->Error 
                + pid->Ki * pid->Integral 
                + pid->Kd * derivative;

    if(pid->Output > 100) pid->Output = 100;
    if(pid->Output < -100) pid->Output = -100;
    
    pid->LastError = pid->Error;
    
    return pid->Output;
}
