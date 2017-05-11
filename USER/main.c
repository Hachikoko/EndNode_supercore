#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "Timer.h"
#include "main.h"
#include "NRF24L01.h"
#include "link_queue.h"

u8 node_index_for_base_station = 0;
u8 node_index_for_end_node = 10;
u8 extra_node_flag = 0;
u8 current_frequency = 0;

Link_Queue* link_queue = NULL;

char ret_words[100];

int main(void)
{

	u8 ret = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组2
	delay_init(168);
	Uart1_init(115200);
	Uart3_init(115200);
	
//	NRF24L01_Init();
//	while(NRF24L01_Check()){
//		delay_ms(100);
//	}
	
	link_queue = creat_link_queue();
	
//	if(extra_node_flag == 1){
//		current_frequency = ADD_EXTRA_FREQUENCY;
//	}else{
//		current_frequency = node_index_for_end_node * 5;
//	}
//	RX_Mode();
	while(1){
//		sprintf(ret_words,"I am node %02d,current_frequency:%d\r\n",node_index_for_base_station,current_frequency);
//		Uart1_SendString((u8*)ret_words);
//		MPU_Get_9_Axis(&mpu_data);
//		sprintf(ret_words,"ax:%f\tay:%f\taz:%f\tgx:%f\tgy:%f\tgz:%f\tmx:%f\tmy:%f\tmz:%f\r\n"
//		,mpu_data.ax,mpu_data.ay,mpu_data.az,mpu_data.gx,mpu_data.gy,mpu_data.gz,mpu_data.mx,mpu_data.my,mpu_data.mz);
//		Uart1_SendString((u8*)ret_words);
//		MPU9250_send_data(mpu_raw_data.ax,mpu_raw_data.ay,mpu_raw_data.az,mpu_raw_data.gx,mpu_raw_data.gy,mpu_raw_data.gz,mpu_raw_data.mx,mpu_raw_data.my,mpu_raw_data.mz);
		delay_ms(10);
//		usart1_send_char((u8)'A');
	}

}
