#include "Timer.h"
#include "NRF24L01.h"
#include "delay.h"
#include "main.h"

extern u8 node_index_for_base_station;

void Timer7_init(void){
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =  TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 30 * node_index_for_base_station;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8399;
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);          //计数器初始化
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );                //中断使能
	
	TIM_Cmd(TIM7,DISABLE); 
	
	return;
}

void TIM7_IRQHandler(void){
//#if (NODE_NUM == 0)
//	u8 buf[27] = {'0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','\r','\n'};
//#elif (NODE_NUM == 1)
//	u8 buf[27] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','\r','\n'};	
//#endif
//	TX_Mode();
	u8 buf[27];
	sprintf((char*)buf,"%02d%02d%02d%02d%02d%02d%02d%02d%02d%02d%02d%02d\r\n",node_index_for_base_station
		,node_index_for_base_station,node_index_for_base_station,node_index_for_base_station
	,node_index_for_base_station,node_index_for_base_station,node_index_for_base_station,node_index_for_base_station
	,node_index_for_base_station,node_index_for_base_station,node_index_for_base_station,node_index_for_base_station);
//	Uart1_SendString(" . in TIM7_IRQHandler ......\r\n");
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_Cmd(TIM7,DISABLE); 
		if(TX_OK==Wireless_Send_Data(buf)){
//			Uart1_SendString(" . 节点射频模块发射成功......\r\n");
			Uart1_SendString(buf);
		//	delay_ms(1000);
		}else {
			Uart1_SendString(" . 节点射频模块发射失败......\r\n");
		//	delay_ms(1000);
		}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //清除中断标志位
//	RX_Mode();
	
	}
}

