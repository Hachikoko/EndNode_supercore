#include "sys.h"
#include "usart.h"	
#include "string.h"
#include "stdlib.h"
#include "link_queue.h"
#include "stdbool.h"


int com3_absolute_buf_segment_index = -1;
int index_50 = 0;
extern Link_Queue* link_queue;
unsigned char com3_buf[50];
int mod_buf_index = 0;
unsigned short temp_char,pre_char;



void Uart1_init(u32 bound){                               //����1��ʼ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
	

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; //��Ӧ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ�� VIC �Ĵ�����
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
}

void Uart3_init(u32 bound){

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10
	

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//�����ж�
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2; //��Ӧ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ�� VIC �Ĵ�����
	
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
}

void Uart1_SendString(u8 *str)
{
  while(*str)
  {
  	 USART_SendData(USART1,*str);											   //��*str��USARTx���ͳ�ȥ
	 while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		       //�ȴ��������
	 str++;	                                                                   //str�ĵ�ַ����1������һ���ַ�
  }
}

void Uart3_SendString(u8 *str)
{
  while(*str)
  {
  	 USART_SendData(USART3,*str);											   //��*str��USARTx���ͳ�ȥ
	 while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);		       //�ȴ��������
	 str++;	                                                                   //str�ĵ�ַ����1������һ���ַ�
  }
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//����Ĵ�����������
	{
		uint16_t rec_short;
		USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
		rec_short = USART_ReceiveData(USART1);
		if(*((char*)&rec_short) == ADD_SET_NODES)
		{
			
		}else if(*((char*)&rec_short)== ADD_SINGLE_NODES){
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	}
}





void USART3_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET){		//����Ĵ�����������
		USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		//��������
		temp_char = USART_ReceiveData(USART3);
		
		usart1_send_char(temp_char);
		
		if((pre_char == 0x5A) && (temp_char == 0xA5)){
			index_50 = 0;
		}
		pre_char = temp_char;
		if(index_50 < 50){
			com3_buf[index_50++] = (unsigned char)temp_char;
		}
		
		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
		
	}
////	usart1_send_char((u8)'C');
//	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//����Ĵ�����������
////	if(USART_GetITStatus(USART3,USART_IT_RXNE)==RESET)//����Ĵ�����������
//	{
//		uint16_t rec_short;
//		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//		USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
//		//����������жϴ�ϣ��������ʱ��������
//		if(interupt_flag == 1){
////			usart1_send_char((u8)'E');
//			flag_frame = 0;
//			com3_absolute_buf_segment_index = 0;
//			interupt_flag = 0;
//		}
//		
//		com3_buf[com3_absolute_buf_segment_index++] = USART_ReceiveData(USART3);
//		
//		if((com3_absolute_buf_segment_index > 1) && (com3_buf[com3_absolute_buf_segment_index - 1] == 0xA5) && (0x5A == com3_buf[com3_absolute_buf_segment_index - 2])){
//			flag_frame = 1;
//			begin_index = com3_absolute_buf_segment_index;
//		}
//		
//		if(flag_frame && (com3_absolute_buf_segment_index - begin_index) == 56){
//			flag_frame = 0;
//			com3_absolute_buf_segment_index = 0;
//			
////			usart1_send_char((u8)'D');
//			
//			u8* ptr_link_data = (u8*)malloc(32);
//			push_back_node(link_queue,ptr_link_data); //�����߱�������
//			
////			usart1_send_char((u8)'E');
//			
//			float temp;
//			memcpy(&temp,com3_buf+begin_index - 2 + 37,4);
//			*((short*)(ptr_link_data + 24)) = (short)(temp * 10000.0f);  //w
//						
//			memcpy(&temp,com3_buf+begin_index - 2 + 41,4);
//			*((short*)(ptr_link_data + 26)) = (short)(temp * 10000.0f);  //x
//						
//			memcpy(&temp,com3_buf+begin_index - 2 + 45,4);
//			*((short*)(ptr_link_data + 28)) = (short)(temp * 10000.0f);  //y
//			
//			memcpy(&temp,com3_buf+begin_index - 2 + 49,4);
//			*((short*)(ptr_link_data + 30)) = (short)(temp * 10000.0f);  //z
//			usart1_send_char((u8)'F');
//			
//			//���ڵ����
////			int i = 0;
////			for(i = 0; i < 58;i++){
////				usart1_send_char(com3_buf[begin_index - 2 + i]);
////			}
//			
////			send_data_buf[0] = 'D';
////			send_data_buf[1] = node_index_for_end_node;
////			*((unsigned int *)(send_data_buf + 2)) = frame_index++;
////			float temp;
////			memcpy(&temp,com3_buf+begin_index - 2 + 37,4);
////			*((short*)(send_data_buf + 24)) = (short)(temp * 10000.0f);  //w
////						
////			memcpy(&temp,com3_buf+begin_index - 2 + 41,4);
////			*((short*)(send_data_buf + 26)) = (short)(temp * 10000.0f);  //x
////						
////			memcpy(&temp,com3_buf+begin_index - 2 + 45,4);
////			*((short*)(send_data_buf + 28)) = (short)(temp * 10000.0f);  //y
////			
////			memcpy(&temp,com3_buf+begin_index - 2 + 49,4);
////			*((short*)(send_data_buf + 30)) = (short)(temp * 10000.0f);  //z
////			
////			int i;
////			for(i = 0; i < 32;i++){
////				usart1_send_char(send_data_buf[i]);
////			}
////			memset(send_data_buf,0,32);
//		}
//		
//		
//		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
//		
//	}
	
}


//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{
	USART_SendData(USART1,c); 
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
} 

void usart3_send_char(u8 c)
{
	USART_SendData(USART3,c); 
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);	
} 

//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];	//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}

//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void MPU9250_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short meox,short meoy,short meoz)
{
	u8 tbuf[18]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	tbuf[12]=(meox>>8)&0XFF;
	tbuf[13]=meox&0XFF;
	tbuf[14]=(meoy>>8)&0XFF;
	tbuf[15]=meoy&0XFF;
	tbuf[16]=(meoz>>8)&0XFF;
	tbuf[17]=meoz&0XFF;
	usart1_niming_report(0XA1,tbuf,18);//�Զ���֡,0XA1
}

//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
//void MPU9250_report_imu(MPU9250_RAW_DATD*mup_raw_data,Fliter_Result_Data*result_data)
//{
//	u8 tbuf[24]; 
//	u8 i;
//	short roll,pitch,yaw;
//	roll = (short)(result_data->euler[0] * 100);
//	pitch = (short)(result_data->euler[1] * 100);
//	yaw = (short)result_data->euler[2] * 10;
//	for(i=0;i<28;i++)tbuf[i]=0;//��0
//	tbuf[0]=((mup_raw_data->ax)>>8)&0XFF;
//	tbuf[1]=(mup_raw_data->ax)&0XFF;
//	tbuf[2]=((mup_raw_data->ay)>>8)&0XFF;
//	tbuf[3]=(mup_raw_data->ay)&0XFF;
//	tbuf[4]=((mup_raw_data->az)>>8)&0XFF;
//	tbuf[5]=(mup_raw_data->az)&0XFF; 
//	tbuf[6]=((mup_raw_data->gx)>>8)&0XFF;
//	tbuf[7]=(mup_raw_data->gx)&0XFF;
//	tbuf[8]=((mup_raw_data->gy)>>8)&0XFF;
//	tbuf[9]=(mup_raw_data->gy)&0XFF;
//	tbuf[10]=((mup_raw_data->gz)>>8)&0XFF;
//	tbuf[11]=(mup_raw_data->gz)&0XFF;	
//	tbuf[12]=((mup_raw_data->mx)>>8)&0XFF;
//	tbuf[13]=(mup_raw_data->mx)&0XFF;
//	tbuf[14]=((mup_raw_data->my)>>8)&0XFF;
//	tbuf[15]=(mup_raw_data->my)&0XFF;
//	tbuf[16]=((mup_raw_data->mz)>>8)&0XFF;
//	tbuf[17]=(mup_raw_data->mz)&0XFF;
//	tbuf[18]=(roll>>8)&0XFF;
//	tbuf[19]=roll&0XFF;
//	tbuf[20]=(pitch>>8)&0XFF;
//	tbuf[21]=pitch&0XFF;
//	tbuf[22]=(yaw>>8)&0XFF;
//	tbuf[23]=yaw&0XFF;
//	usart1_niming_report(0XAF,tbuf,24);//�ɿ���ʾ֡,0XAF
//} 






































//////////////////////////////////////////////////////////////////////////////////// 	 
////���ʹ��ucos,����������ͷ�ļ�����.
//#if SYSTEM_SUPPORT_OS
//#include "includes.h"					//ucos ʹ��	  
//#endif
////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEK STM32F4̽���߿�����
////����1��ʼ��		   
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2014/6/10
////�汾��V1.5
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
////All rights reserved
////********************************************************************************
////V1.3�޸�˵�� 
////֧����Ӧ��ͬƵ���µĴ��ڲ���������.
////�����˶�printf��֧��
////�����˴��ڽ��������.
////������printf��һ���ַ���ʧ��bug
////V1.4�޸�˵��
////1,�޸Ĵ��ڳ�ʼ��IO��bug
////2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
////3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
////4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////V1.5�޸�˵��
////1,�����˶�UCOSII��֧��
//////////////////////////////////////////////////////////////////////////////////// 	  
// 

////////////////////////////////////////////////////////////////////
////�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 
//}; 

//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{ 	
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART1->DR = (u8) ch;      
//	return ch;
//}
//#endif
// 
//#if EN_USART1_RX   //���ʹ���˽���
////����1�жϷ������
////ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
//u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
////����״̬
////bit15��	������ɱ�־
////bit14��	���յ�0x0d
////bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_RX_STA=0;       //����״̬���	

////��ʼ��IO ����1 
////bound:������
//void uart_init(u32 bound){
//   //GPIO�˿�����
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
// 
//	//����1��Ӧ���Ÿ���ӳ��
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
//	
//	//USART1�˿�����
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
//	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

//   //USART1 ��ʼ������
//	USART_InitStructure.USART_BaudRate = bound;//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
//	
//  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
//	
//	//USART_ClearFlag(USART1, USART_FLAG_TC);
//	
//#if EN_USART1_RX	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

//	//Usart1 NVIC ����
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

//#endif
//	
//}


//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x8000;	//��������� 
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}   		 
//  } 
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntExit();  											 
//#endif
//} 
//#endif	


