#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "Timer.h"
#include "main.h"
#include "NRF24L01.h"
#include "link_queue.h"
#include "string.h"
#include "stdlib.h"
#include "DMA.h"

u8 node_index_for_base_station = 0;
u8 node_index_for_end_node = 6;
u8 extra_node_flag = 0;
u8 current_frequency = 0;
Link_Queue* link_queue = NULL;
char ret_words[100];


void supercore_init(void){
	
	//	Uart3_SendString((u8*)"AT+RST\r\n");  //����ģ������Ϊ0��
	//	delay_ms(10);
	
	Uart3_SendString((u8*)"AT+ODR=0\r\n");  //����ģ������Ϊ0��
	delay_ms(10);
	Uart3_SendString((u8*)"AT+MODE=0\r\n");  //���� 1��9��ģʽ��0��6��ģʽ
	delay_ms(10);
	Uart3_SendString((u8*)"AT+DIR=0\r\n");  //���� 0��ˮƽģʽ��1����ֱģʽ��
	delay_ms(10);
	Uart3_SendString((u8*)"AT+SETPTL=A0,B0,C0,D1\r\n");  //�������ٶȡ����ٶȡ������ơ���Ԫ����
	delay_ms(10);
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�ж����ȼ�����2
	delay_init(168);
	Uart1_init(115200);
	Uart2_init(115200);
	Uart3_init(115200);
	supercore_init();
	
	//����1�ʹ���2��DMA���ͳ�ʼ��
	DMA1_and_DMA2_Config();
	
	
	//���ߴ����ʼ��
	NRF24L01_Init();
	while(NRF24L01_Check()){
		delay_ms(100);
	}
	current_frequency = node_index_for_end_node * 5;
	RX_Mode();
	
	while(1){
		delay_ms(100);
//		usart2_send_char('A');
	}

}
