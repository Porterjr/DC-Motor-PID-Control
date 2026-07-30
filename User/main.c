#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "Key.h"
#include "PID.h"

int main(void)
{
	OLED_Init();
	Motor_Init();
	Encoder_Init();
    Serial_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "Target:");
    OLED_ShowString(2, 1, "Actual:");
    OLED_ShowString(3, 1, "PWM:");
    OLED_ShowString(4, 1, "Delta:");
	
	PID_TypeDef MotorPID;
	PID_Init(&MotorPID, 3.0f, 0.3f, 0.0f);
	MotorPID.Target = 0;
	
	int16_t lastCount = 0;
	int16_t TargetRPM = 20;
	
	while (1)
	{
		uint8_t key = Key_GetNum();
        
        if (key == 1)
        {
            TargetRPM += 10;
            if (TargetRPM > 100) TargetRPM = 100;
        }
        else if (key == 2)
        {
            TargetRPM -= 10;
            if (TargetRPM < -100) TargetRPM = -100;
        }
        else if (key == 3)
        {
            TargetRPM = 0;
        }
        
        MotorPID.Target = (float)TargetRPM;
        Delay_ms(100);
        
        int16_t currentCount = Encoder_GetCount();
        int16_t delta = currentCount - lastCount;
        lastCount = currentCount;
        
        int16_t ActualRPM = delta * 600 / 19240;
        
        float pwmOutput = PID_Update(&MotorPID, (float)ActualRPM);
        Motor_SetSpeed((int16_t)pwmOutput);
        
        OLED_ShowSignedNum(1, 9, TargetRPM, 3);
        OLED_ShowSignedNum(2, 9, ActualRPM, 3);
        OLED_ShowSignedNum(3, 9, (int16_t)pwmOutput, 3);
        OLED_ShowSignedNum(4, 9, (int16_t)(MotorPID.Target - ActualRPM), 3);
        
        Serial_Printf("Tar:%d Act:%d PWM:%d Err:%.1f\r\n", 
                      TargetRPM, ActualRPM, (int16_t)pwmOutput, MotorPID.Error);
        
        if(currentCount > 30000 || currentCount < -30000)
        {
            TIM_SetCounter(TIM3, 0);
            lastCount = 0;
        }
    }
}

//		Motor_SetSpeed(50);
//        Delay_ms(3000);
//        
//        Motor_SetSpeed(0);
//        Delay_ms(1000);
//        
//        Motor_SetSpeed(-50);
//        Delay_ms(3000);
//        
//        Motor_SetSpeed(0);
//        Delay_ms(1000);
		
//		Motor_SetSpeed(20);    // 20% 占空比
//        Delay_ms(3000);
//        
//        Motor_SetSpeed(50);    // 50% 占空比
//        Delay_ms(3000);
//        
//        Motor_SetSpeed(80);    // 80% 占空比
//        Delay_ms(3000);
//        
//        Motor_SetSpeed(100);   // 100% 占空比
//        Delay_ms(3000);
//        
//        Motor_SetSpeed(0);     // 停止
//        Delay_ms(2000);

//		Motor_SetSpeed(30);        // 低速正转，30%占空比
//        Delay_ms(1000);
//        
//        int16_t currentCount = Encoder_GetCount();
//        int16_t delta = currentCount - lastCount;
//        
//        Serial_Printf("Delta: %d\r\n", delta);
//        
//        lastCount = currentCount;
//        
//        if(currentCount > 60000 || currentCount < -60000)
//        {
//            TIM_SetCounter(TIM3, 0);
//            lastCount = 0;
//		}
