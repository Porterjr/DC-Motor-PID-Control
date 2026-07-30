#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "Key.h"

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
	
	int16_t TargetSpeed = 0;
	int16_t lastCount = 0;
	
	while (1)
	{
		 uint8_t key = Key_GetNum();
        
        if (key == 1)
        {
            TargetSpeed += 20;
            if (TargetSpeed > 100) TargetSpeed = 100;
        }
        else if (key == 2)
        {
            TargetSpeed -= 20;
            if (TargetSpeed < -100) TargetSpeed = -100;
        }
        else if (key == 3)
        {
            TargetSpeed = 0;
        }
        
        Motor_SetSpeed(TargetSpeed);
        Delay_ms(100);
        
        int16_t currentCount = Encoder_GetCount();
        int16_t delta = currentCount - lastCount;
        lastCount = currentCount;
        
        int16_t ActualRPM = delta * 600 / 19240;
        
        OLED_ShowSignedNum(1, 9, TargetSpeed, 3);
        OLED_ShowSignedNum(2, 9, ActualRPM, 3);
        OLED_ShowSignedNum(3, 9, TargetSpeed, 3);
        OLED_ShowSignedNum(4, 9, delta, 4);
        
        Serial_Printf("Tar:%d Act:%d D:%d\r\n", TargetSpeed, ActualRPM, delta);
        
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
