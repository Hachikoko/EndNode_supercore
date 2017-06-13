#include "sys.h"
#include "usart.h"	
#include "string.h"
#include "stdlib.h"
#include "link_queue.h"
#include "stdbool.h"
#include "DMA.h"


int com3_absolute_buf_segment_index = -1;
int index_50 = 0;
int index_1024 = 0;
extern Link_Queue* link_queue;
unsigned char com3_buf[1024];
int mod_buf_index = 0;
unsigned short temp_char,pre_char;  //本节点的数据获取
unsigned short temp_frame_char,current_frame_char,pre_frame_char,pre_pre_frame_char;//节点间的数据获取
char frame_queue[1024];
char frame_elements_nums = 0;


bool father_node = false;
bool child_node = false;
bool network_flag = false;

extern u8 node_index_for_end_node;



void Uart1_init(u32 bound){                               //串口1初始化
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); // 开启 串口空闲IDEL 中断
	
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; //响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化 VIC 寄存器、
	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); // 开启串口DMA接收
	USART_Cmd(USART1, ENABLE);  //使能串口2 
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

void Uart2_init(u32 bound){                               //串口1初始化
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟

	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10复用为USART1
	
	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
//	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); // 开启 串口空闲IDEL 中断
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; //响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化 VIC 寄存器、
	
	//配置DMA1_Stream6发送完成中断
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn; // 发送DMA通道的中断配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// 优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); // 开启串口DMA发送
	USART_Cmd(USART2, ENABLE);  //使能串口2 
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

void Uart3_init(u32 bound){

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
//	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//开启中断
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); // 开启 串口空闲IDEL 中断
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2; //响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化 VIC 寄存器、
	
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); // 开启串口DMA接收
	USART_Cmd(USART3, ENABLE);  //使能串口1 
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
}

void Uart1_SendString(u8 *str)
{
  while(*str)
  {
  	 USART_SendData(USART1,*str);											   //将*str从USARTx发送出去
	 while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		       //等待发送完毕
	 str++;	                                                                   //str的地址增加1，到下一个字符
  }
}

void Uart2_SendString(u8 *str)
{
  while(*str)
  {
  	 USART_SendData(USART2,*str);											   //将*str从USARTx发送出去
	 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		       //等待发送完毕
	 str++;	                                                                   //str的地址增加1，到下一个字符
  }
}

void Uart3_SendString(u8 *str)
{
  while(*str)
  {
  	 USART_SendData(USART3,*str);											   //将*str从USARTx发送出去
	 while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);		       //等待发送完毕
	 str++;	                                                                   //str的地址增加1，到下一个字符
  }
}

void USART1_IRQHandler(void)
{
	
	unsigned int read_n = 0;
	if(USART_GetITStatus(USART1,USART_IT_IDLE)==SET)//如果空闲
	{
		read_n = USART1->SR;
		read_n = USART1->DR;;
		DMA_Cmd(DMA2_Stream2,DISABLE);    //关闭DMA
		
		read_n = 1024 - DMA_GetCurrDataCounter(DMA2_Stream2);

		if(1 == read_n && !network_flag && frame_queue[0] == NETWORKING_RECEIVE){
			child_node = true;
		}else if(frame_queue[0] == '#' && frame_queue[2] == '#'){
			int frame_elements_nums = frame_queue[1];
			int index_1024 = frame_elements_nums * 41 + 3;
			frame_queue[1] = frame_queue[1] + 1;
			
			if(com3_buf[0] == 0x5A && (com3_buf[27] == 0xD1)){
				frame_queue[index_1024] = VALID_DATA;
				*(short*)(frame_queue + index_1024 + 7) = *(short*)(com3_buf + 6);
				*(short*)(frame_queue + index_1024 + 9) = *(short*)(com3_buf + 8);
				*(short*)(frame_queue + index_1024 + 11) = *(short*)(com3_buf + 10);
			
				*(short*)(frame_queue + index_1024 + 13) = *(short*)(com3_buf + 13);
				*(short*)(frame_queue + index_1024 + 15) = *(short*)(com3_buf + 15);
				*(short*)(frame_queue + index_1024 + 17) = *(short*)(com3_buf + 17);
			
				*(short*)(frame_queue + index_1024 + 19) = *(short*)(com3_buf + 20);
				*(short*)(frame_queue + index_1024 + 21) = *(short*)(com3_buf + 22);
				*(short*)(frame_queue + index_1024 + 23) = *(short*)(com3_buf + 24);
				
				memcpy((frame_queue + index_1024 + 25),(com3_buf + 27),4);
				memcpy((frame_queue + index_1024 + 29),(com3_buf + 31),4);
				memcpy((frame_queue + index_1024 + 33),(com3_buf + 35),4);
				memcpy((frame_queue + index_1024 + 37),(com3_buf + 39),4);
			}else{
				frame_queue[index_1024] = INVALID_DATA;
			}	
			
			frame_queue[index_1024 + 1] = 'D';
			frame_queue[index_1024 + 2] = node_index_for_end_node;
			
			int i = 0,length = (frame_elements_nums + 1) * 41 + 3;
			DMA_Enable(DMA1_Stream6,length);
//			for(i = 0; i < length;i++) {
//				usart2_send_char(frame_queue[i]);
//			}
			
		}
		
		DMA_ClearFlag(DMA2_Stream2,DMA_FLAG_TCIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_DMEIF2);//清标志
		DMA_SetCurrDataCounter(DMA2_Stream2,1024);
		DMA_Cmd(DMA2_Stream2,ENABLE);
	}
		
    USART_ClearITPendingBit(USART1, USART_IT_IDLE);	
	
//	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//如果寄存器中有数据
//	{
//		uint16_t rec_short;
//		USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
//		rec_short = USART_ReceiveData(USART1);
//		if(*((char*)&rec_short) == NETWORKING)
//		{
//			father_node = true;
//			usart1_send_char(NETWORKING_RECEIVE);
//			usart2_send_char(NETWORKING);
//			
//		}else if(*((char*)&rec_short)== START_FRAME){
//			network_flag = true;
//			if(child_node == true){
//				usart2_send_char(START_FRAME);
//			}
//			Uart3_SendString((u8*)"AT+TRG\r\n");
//			
//		}
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//	}
}

void USART2_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//如果寄存器中有数据
	{
		uint16_t rec_short;
		USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
		rec_short = USART_ReceiveData(USART2);
		if(*((char*)&rec_short) == NETWORKING)
		{
			father_node = true;
			usart2_send_char(NETWORKING_RECEIVE);
			usart1_send_char(NETWORKING);
			
		}else if(*((char*)&rec_short)== START_FRAME){
			network_flag = true;
			if(child_node == true){
				usart1_send_char(START_FRAME);
			}
			Uart3_SendString((u8*)"AT+TRG\r\n");
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	}
//	unsigned int read_n = 0;
//	if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET)//如果空闲
//	{
//		read_n = USART2->SR;
//		read_n = USART2->DR;;
//		DMA_Cmd(DMA1_Stream5,DISABLE);    //关闭DMA
//		
//		read_n = 1024 - DMA_GetCurrDataCounter(DMA1_Stream5);

//		if(1 == read_n && !network_flag && frame_queue[0] == NETWORKING_RECEIVE){
//			child_node = true;
//		}else if(frame_queue[0] == '#' && frame_queue[2] == '#'){
//			int frame_elements_nums = frame_queue[1];
//			int index_1024 = frame_elements_nums * 41 + 3;
//			frame_queue[1] = frame_queue[1] + 1;
//			
//			if(com3_buf[0] == 0xA5 && (com3_buf[26] == 0xD1)){
//				frame_queue[index_1024] = VALID_DATA;
//				*(short*)(frame_queue + index_1024 + 7) = *(short*)(com3_buf + 6);
//				*(short*)(frame_queue + index_1024 + 9) = *(short*)(com3_buf + 8);
//				*(short*)(frame_queue + index_1024 + 11) = *(short*)(com3_buf + 10);
//			
//				*(short*)(frame_queue + index_1024 + 13) = *(short*)(com3_buf + 13);
//				*(short*)(frame_queue + index_1024 + 15) = *(short*)(com3_buf + 15);
//				*(short*)(frame_queue + index_1024 + 17) = *(short*)(com3_buf + 17);
//			
//				*(short*)(frame_queue + index_1024 + 19) = *(short*)(com3_buf + 20);
//				*(short*)(frame_queue + index_1024 + 21) = *(short*)(com3_buf + 22);
//				*(short*)(frame_queue + index_1024 + 23) = *(short*)(com3_buf + 24);
//				
//				memcpy((frame_queue + index_1024 + 25),(com3_buf + 27),4);
//				memcpy((frame_queue + index_1024 + 29),(com3_buf + 31),4);
//				memcpy((frame_queue + index_1024 + 33),(com3_buf + 35),4);
//				memcpy((frame_queue + index_1024 + 37),(com3_buf + 39),4);
//			}else{
//				frame_queue[index_1024] = INVALID_DATA;
//			}	
//			
//			frame_queue[index_1024 + 1] = 'D';
//			frame_queue[index_1024 + 2] = node_index_for_end_node;
//			
//			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 
//			int length = (frame_elements_nums + 1) * 41 + 3;			
//			DMA_Enable(DMA2_Stream7,length);     //开始一次DMA传输！
//		}
//		
//		
//		
//		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5|DMA_FLAG_HTIF5|DMA_FLAG_DMEIF5);//清标志
//		DMA_SetCurrDataCounter(DMA1_Stream5,1024);
//		DMA_Cmd(DMA1_Stream5,ENABLE);
//	}
//		
//    USART_ClearITPendingBit(USART2, USART_IT_IDLE);	

//		int read_n = 0;
//		read_n = USART2->SR;
//		read_n = USART2->DR;
//		DMA_Cmd(DMA1_Stream5, DISABLE); // 关闭DMA ，防止干扰
////		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TEIF7 | DMA_FLAG_TCIF7); // 清除DMA所有标志
//		
//		read_n = 1024 - DMA_GetCurrDataCounter(DMA1_Stream5); 
//		usart1_send_char(frame_queue[0]);
//		if(read_n == 1 && !network_flag && frame_queue[0] == NETWORKING_RECEIVE){
//			child_node = true;
//			
//		}else if(frame_queue[0] == '#' && frame_queue[2] == '#'){
//			int frame_elements_nums = frame_queue[1];
//			int index_1024 = frame_elements_nums * 41 + 3;
//			frame_queue[1] = frame_queue[1] + 1;
//			
//			if(com3_buf[0] == 0xA5 && (com3_buf[26] == 0xD1)){
//				frame_queue[index_1024] = VALID_DATA;
//				*(short*)(frame_queue + index_1024 + 7) = *(short*)(com3_buf + 6);
//				*(short*)(frame_queue + index_1024 + 9) = *(short*)(com3_buf + 8);
//				*(short*)(frame_queue + index_1024 + 11) = *(short*)(com3_buf + 10);
//			
//				*(short*)(frame_queue + index_1024 + 13) = *(short*)(com3_buf + 13);
//				*(short*)(frame_queue + index_1024 + 15) = *(short*)(com3_buf + 15);
//				*(short*)(frame_queue + index_1024 + 17) = *(short*)(com3_buf + 17);
//			
//				*(short*)(frame_queue + index_1024 + 19) = *(short*)(com3_buf + 20);
//				*(short*)(frame_queue + index_1024 + 21) = *(short*)(com3_buf + 22);
//				*(short*)(frame_queue + index_1024 + 23) = *(short*)(com3_buf + 24);
//				
//				memcpy((frame_queue + index_1024 + 25),(com3_buf + 27),4);
//				memcpy((frame_queue + index_1024 + 29),(com3_buf + 31),4);
//				memcpy((frame_queue + index_1024 + 33),(com3_buf + 35),4);
//				memcpy((frame_queue + index_1024 + 37),(com3_buf + 39),4);
//			}else{
//				frame_queue[index_1024] = INVALID_DATA;
//			}	
//			
//			frame_queue[index_1024 + 1] = 'D';
//			frame_queue[index_1024 + 2] = node_index_for_end_node;
//			
//			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 
//			int length = (frame_elements_nums + 1) * 41 + 3;			
//			DMA_Enable(DMA2_Stream7,length);     //开始一次DMA传输！
//		}
////		USART_ClearITPendingBit(USART2,USART_IT_IDLE);
//	}
//	
//	DMA_SetCurrDataCounter(DMA1_Stream5,1024);
//	DMA_Cmd(DMA1_Stream5,ENABLE);
}


void USART3_IRQHandler(void)
{
	unsigned int read_n = 0;
	if(USART_GetITStatus(USART3,USART_IT_IDLE)==SET){
		read_n = USART3->SR;
		read_n = USART3->DR;
		DMA_Cmd(DMA1_Stream1,DISABLE);    //关闭DMA
		
		read_n = 1024 - DMA_GetCurrDataCounter(DMA1_Stream1);
		
//		int i = 0;
//		for(i = 0; i < read_n; i++){
//			usart2_send_char(com3_buf[i]);
//		}
		
		if((child_node == false) && (father_node == true) && 44 == read_n){
			frame_queue[0] = '#';
			frame_queue[1] = 1;
			frame_queue[2] = '#';
			if(com3_buf[0] == 0x5A && (com3_buf[27] == 0xD1)){
				frame_queue[3] = VALID_DATA;
				*(short*)(frame_queue + 10) = *(short*)(com3_buf + 6);
				*(short*)(frame_queue + 12) = *(short*)(com3_buf + 8);
				*(short*)(frame_queue + 14) = *(short*)(com3_buf + 10);
			
				*(short*)(frame_queue + 16) = *(short*)(com3_buf + 13);
				*(short*)(frame_queue + 18) = *(short*)(com3_buf + 15);
				*(short*)(frame_queue + 20) = *(short*)(com3_buf + 17);
			
				*(short*)(frame_queue + 22) = *(short*)(com3_buf + 20);
				*(short*)(frame_queue + 24) = *(short*)(com3_buf + 22);
				*(short*)(frame_queue + 26) = *(short*)(com3_buf + 24);

				memcpy((frame_queue + 28),(com3_buf + 27),4);
				memcpy((frame_queue + 32),(com3_buf + 31),4);
				memcpy((frame_queue + 36),(com3_buf + 35),4);
				memcpy((frame_queue + 40),(com3_buf + 39),4);

			}else{
				frame_queue[3] = INVALID_DATA;
			}	
			frame_queue[4] = 'D';
			frame_queue[5] = node_index_for_end_node;
			
			DMA_Enable(DMA1_Stream6,44);
			
//			int i = 0;
//			for(i = 0; i < 44;i++){
//				usart2_send_char(frame_queue[i]);
//			}
		}
		
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_DMEIF1);//清标志
		DMA_SetCurrDataCounter(DMA1_Stream1,1024);
		DMA_Cmd(DMA1_Stream1,ENABLE);
		
		
	}
	USART_ClearITPendingBit(USART3, USART_IT_IDLE);	
	
//	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET){		//如果寄存器中有数据
//		USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
//		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//		//缓存数据
//		temp_char = USART_ReceiveData(USART3);
//		
//		if((pre_char == 0x5A) && (temp_char == 0xA5)){
//			index_50 = 0;
//		}
//		pre_char = temp_char;
//		if(index_50 < 50){
//			com3_buf[index_50++] = (unsigned char)temp_char;
//		}
//		
//		//有线下末端节点主动发送数据

//		if((index_50 == 43) && (child_node == false) && (father_node == true)){
//			frame_queue[0] = '#';
//			frame_queue[1] = 1;
//			frame_queue[2] = '#';
//			if(com3_buf[0] == 0xA5 && (com3_buf[26] == 0xD1)){

//				frame_queue[3] = VALID_DATA;
//				*(short*)(frame_queue + 10) = *(short*)(com3_buf + 6);
//				*(short*)(frame_queue + 12) = *(short*)(com3_buf + 8);
//				*(short*)(frame_queue + 14) = *(short*)(com3_buf + 10);
//			
//				*(short*)(frame_queue + 16) = *(short*)(com3_buf + 13);
//				*(short*)(frame_queue + 18) = *(short*)(com3_buf + 15);
//				*(short*)(frame_queue + 20) = *(short*)(com3_buf + 17);
//			
//				*(short*)(frame_queue + 22) = *(short*)(com3_buf + 20);
//				*(short*)(frame_queue + 24) = *(short*)(com3_buf + 22);
//				*(short*)(frame_queue + 26) = *(short*)(com3_buf + 24);

//				memcpy((frame_queue + 28),(com3_buf + 27),4);
//				memcpy((frame_queue + 32),(com3_buf + 31),4);
//				memcpy((frame_queue + 36),(com3_buf + 35),4);
//				memcpy((frame_queue + 40),(com3_buf + 39),4);

//			}else{
//				frame_queue[3] = INVALID_DATA;;
//			}	
//			frame_queue[4] = 'D';
//			frame_queue[5] = node_index_for_end_node;
//			
//			//保留4字节给序列号
//			
//			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 			
//			DMA_Enable(DMA2_Stream7,44);     //开始一次DMA传输！
//			

//		}
//		
//		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
//		
//	}
	
}


//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
	USART_SendData(USART1,c); 
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
} 

void usart2_send_char(u8 c)
{
	USART_SendData(USART2,c); 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);	
} 

void usart3_send_char(u8 c)
{
	USART_SendData(USART3,c); 
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);	
} 




