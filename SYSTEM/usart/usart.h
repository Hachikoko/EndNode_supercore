#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h"
#include "stdbool.h"

#define  NETWORKING 'N'			//有线组网发起信号
#define  NETWORKING_RECEIVE 'n'			//有线组网
#define  START_FRAME 'S'		//有线帧起始信号
#define  VALID_DATA 1
#define  INVALID_DATA 0




void Uart1_init(u32 bound);
void Uart1_SendString(u8 *str);
void usart1_send_char(u8 c);

void Uart2_init(u32 bound);
void Uart2_SendString(u8 *str);
void usart2_send_char(u8 c);

void Uart3_init(u32 bound);
void Uart3_SendString(u8 *str);
void usart3_send_char(u8 c);

#endif

