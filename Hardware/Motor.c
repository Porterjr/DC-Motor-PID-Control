#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "PWM.h"

// 接线定义：
// TB6612 AIN1 -> PB12
// TB6612 AIN2 -> PB13
// TB6612 PWMA -> PA0 (TIM2_CH1)
// TB6612 STBY -> 3.3V
// TB6612 GND  -> STM32 GND
// TB6612 VCC  -> STM32 5V

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	PWM_Init();
}

void Motor_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		PWM_SetCompare1((uint16_t)(Speed * 10));
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		PWM_SetCompare1((uint16_t)((-Speed) * 10));
	}
}
