#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "NRF24L01.h"
#include "Timer.h"
#include "main.h"

u8 node_index_for_base_station = 0;
u8 node_index_for_end_node = 10;
u8 extra_node_flag = 0;
u8 current_frequency = 0;


int main(void)
{
	char ret_words[200];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组2
	Uart1_init(115200);
	delay_init(168);
	NRF24L01_Init();
	while(NRF24L01_Check()){
		delay_ms(100);
	}
	if(extra_node_flag == 1){
		current_frequency = ADD_EXTRA_FREQUENCY;
	}else{
		current_frequency = node_index_for_end_node * 5;
	}
	RX_Mode();
	while(1){
//		if(TX_OK==Wireless_Send_Data(buf)){
		sprintf(ret_words,"I am node %02d,current_frequency:%d\r\n",node_index_for_base_station,current_frequency);
		Uart1_SendString((u8*)ret_words);
		delay_ms(1000);
//		}else {
//			Uart1_SendString(" . 射频模块发射失败......\r\n");
//			delay_ms(1000);
//		}
	}

}
