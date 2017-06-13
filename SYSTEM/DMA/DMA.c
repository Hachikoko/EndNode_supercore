#include "dma.h"																	   	  
#include "delay.h"		 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//DMA 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 
 extern char frame_queue[1024];
 extern unsigned char com3_buf[1024];
 

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
//void DMA2_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)   //串口1的发送
//{ 
// 
//	DMA_InitTypeDef  DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
////	//配置DMA1发送完成中断
////	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn; // 发送DMA通道的中断配置
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// 优先级设置
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////	NVIC_Init(&NVIC_InitStructure);
//	
//	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
//	{
//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
//		
//	}else 
//	{
//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
//	}
//  DMA_DeInit(DMA_Streamx);
//	
//	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
//	
//  /* 配置 DMA Stream */
//  DMA_InitStructure.DMA_Channel = chx;  //通道选择
//  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
//  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
//  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
//  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream
//		
//  DMA_Cmd(DMA_Streamx, ENABLE);
//	
////  DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TEIF7 | DMA_FLAG_TCIF7); // 清除DMA所有标志
////  DMA_Cmd(DMA2_Stream7, DISABLE); // 关闭DMA
////  DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);// 开启发送DMA通道中断
//} 

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)  //串口2的接收
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_Init(&NVIC_InitStructure);
	
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
  if(DMA_Streamx == DMA1_Stream6){
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//外设到存储器模式
  }else{
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式
  }
  
  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream
  
  DMA_Cmd(DMA_Streamx, ENABLE);
}

//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	  

void DMA1_and_DMA2_Config(void){
	DMA_Config(DMA2_Stream2,DMA_Channel_4,(u32)&USART1->DR,(u32)frame_queue,1024);//串口1 DMA接收配置
	DMA_Config(DMA1_Stream1,DMA_Channel_4,(u32)&USART3->DR,(u32)com3_buf,1024);//串口3 DMA接收配置
	DMA_Config(DMA1_Stream6,DMA_Channel_4,(u32)&USART2->DR,(u32)frame_queue,1024);//串口2 DMA接收配置
	
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TEIF7 | DMA_FLAG_TCIF7); // 清除DMA所有标志
    DMA_Cmd(DMA1_Stream6, DISABLE); // 关闭DMA
    DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);// 开启发送DMA通道中断
 }



void DMA1_Stream6_IRQHandler(void){
	if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//等待DMA2_Steam7传输完成
	{ 
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//清除DMA2_Steam7传输完成标志 
		DMA_Cmd(DMA1_Stream6, DISABLE);                      //关闭DMA传输 
	}
}





















