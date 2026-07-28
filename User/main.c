#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"

int main(void)
{
	OLED_Init();
	Motor_Init();
	
	while (1)
	{
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
		
		Motor_SetSpeed(20);    // 20% 占空比
        Delay_ms(3000);
        
        Motor_SetSpeed(50);    // 50% 占空比
        Delay_ms(3000);
        
        Motor_SetSpeed(80);    // 80% 占空比
        Delay_ms(3000);
        
        Motor_SetSpeed(100);   // 100% 占空比
        Delay_ms(3000);
        
        Motor_SetSpeed(0);     // 停止
        Delay_ms(2000);
	}
	
}
