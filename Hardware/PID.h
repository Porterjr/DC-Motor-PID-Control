#ifndef __PID_H
#define __PID_H

typedef struct {
    float Kp, Ki, Kd;
    float Target;
    float Error, LastError;
    float Integral;
    float Output;
} PID_TypeDef;

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd);
float PID_Update(PID_TypeDef *pid, float actual);

#endif
