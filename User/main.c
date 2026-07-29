#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	Motor_Init();
	Encoder_Init();
    Serial_Init();
	
	Serial_Printf("Motor+Encoder Test\r\n");
	
	int16_t lastCount = 0;
	
	while (1)
	{
		Motor_SetSpeed(30);        // 低速正转，30%占空比
        Delay_ms(1000);
        
        int16_t currentCount = Encoder_GetCount();
        int16_t delta = currentCount - lastCount;
        
        Serial_Printf("Delta: %d\r\n", delta);
        
        lastCount = currentCount;
        
        if(currentCount > 60000 || currentCount < -60000)
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
