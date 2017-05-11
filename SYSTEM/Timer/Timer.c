#include "Timer.h"
#include "NRF24L01.h"
#include "delay.h"
#include "main.h"
#include "MPU9250.h"
#include "string.h"
#include "fliter.h"

extern u8 node_index_for_base_station;
extern char ret_words[100];
extern unsigned int absolute_frame_num;

void Timer7_init(void){
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =  TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 60 * node_index_for_base_station;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8399;
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);          //计数器初始化
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //清除中断标示位
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );                //中断使能
	
	TIM_Cmd(TIM7,DISABLE); 
	
	return;
}

void TIM7_IRQHandler(void){
	u8 buf[30];
	u8* temp = buf;
	Fliter_Result flited_result;
	memset(buf,0,30);
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_Cmd(TIM7,DISABLE); 
		sprintf((char*)buf,"DT%02d",node_index_for_base_station);
		temp += 4;
		*((unsigned int *)temp) = absolute_frame_num;
		temp += 4;
		*((short *)temp) = (short)flited_result.ax;
		temp += 2;
		*((short *)temp) = (short)flited_result.ay;
		temp += 2;
		*((short *)temp) = (short)flited_result.az;
		temp += 2;
		*((short *)temp) = (short)flited_result.gx;
		temp += 2;
		*((short *)temp) = (short)flited_result.gy;
		temp += 2;
		*((short *)temp) = (short)flited_result.gz;
		temp += 2;
		*((short *)temp) = (short)flited_result.mx;
		temp += 2;
		*((short *)temp) = (short)flited_result.my;
		temp += 2;
		*((short *)temp) = (short)flited_result.mz;
		temp += 2;
		*temp = '@';
		temp += 1;
		*temp = 0;
		if(TX_OK==Wireless_Send_Data(buf)){
		}else {
			Uart1_SendString((u8*)" . 节点射频模块发射失败......\r\n");
		}
		#ifdef TEST_MPU
//		sprintf(ret_words,"mag_x:%d,mag_y:%d,mag_z:%d,",flited_result.mx,flited_result.my,flited_result.mz);
//		Uart1_SendString((u8*)ret_words);
		#endif
		
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //清除中断标志位
	}
}

