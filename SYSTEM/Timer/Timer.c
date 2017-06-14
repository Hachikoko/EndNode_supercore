#include "Timer.h"
#include "NRF24L01.h"
#include "delay.h"
#include "main.h"
#include "string.h"
#include "fliter.h"

extern u8 node_index_for_base_station;
extern u8 node_index_for_end_node;
extern char ret_words[100];
extern unsigned int absolute_frame_num;
extern unsigned char com3_buf[1024];
float temp_quat = 0.0f;

u8 temp_ptr[32];

int i = 0;

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
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_Cmd(TIM7,DISABLE); 	
		
		temp_ptr[0] = 'D';
		temp_ptr[1] = node_index_for_end_node;
		*(unsigned int*)(temp_ptr+2) = absolute_frame_num;

		if((com3_buf[0] == 0x5A) && (com3_buf[27] == 0xD1)){
			*(short*)(temp_ptr + 6) = *(short*)(com3_buf + 7);
			*(short*)(temp_ptr + 8) = *(short*)(com3_buf + 9);
			*(short*)(temp_ptr + 10) = *(short*)(com3_buf + 11);
			
			*(short*)(temp_ptr + 12) = *(short*)(com3_buf + 14);
			*(short*)(temp_ptr + 14) = *(short*)(com3_buf + 16);
			*(short*)(temp_ptr + 16) = *(short*)(com3_buf + 18);
			
			*(short*)(temp_ptr + 18) = *(short*)(com3_buf + 21);
			*(short*)(temp_ptr + 20) = *(short*)(com3_buf + 23);
			*(short*)(temp_ptr + 22) = *(short*)(com3_buf + 25);
			
			memcpy(&temp_quat,(com3_buf + 28),4);
			*(short*)(temp_ptr + 24) = (short)(temp_quat * 10000.0f);
			memcpy(&temp_quat,(com3_buf + 32),4);
			*(short*)(temp_ptr + 26) = (short)(temp_quat * 10000.0f);
			memcpy(&temp_quat,(com3_buf + 36),4);
			*(short*)(temp_ptr + 28) = (short)(temp_quat * 10000.0f);
			memcpy(&temp_quat,(com3_buf + 40),4);
			*(short*)(temp_ptr + 30) = (short)(temp_quat * 10000.0f);
			
			
			
		}else{
			TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //清除中断标志位
			return;
		}
		
		
		if(TX_OK==Wireless_Send_Data(temp_ptr)){
			
		}else {
		//	Uart2_SendString((u8*)" . 节点射频模块发射失败......\r\n");
		}

		
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //清除中断标志位
	}
}

