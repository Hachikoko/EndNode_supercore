#include "sys.h"
#include "NRF24L01.h"
#include "SPI.h"
#include "delay.h"
#include "main.h"
#include "Timer.h"

//������ر���
//u8 end_node_add_flag = 0;

extern u8 node_index_for_base_station;
extern u8 node_index_for_end_node;
extern u8 extra_node_flag;
extern u8 current_frequency;
//��ַ
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x00}; //���յ�ַ	
u8 words_buf[100];

unsigned int absolute_frame_num = 0;

 void EXTI1_IRQHandler(void){
	
	 u8 rev_buf[5] = {0,0,0,0,0};
	 u8 RX_Status;
	 
	 if(EXTI_GetITStatus(EXTI_Line1) != RESET){
		Clr_NRF24L01_CE;
		RX_Status=NRF24L01_RxPacket(rev_buf);
		if(RX_Status == 0){
			Uart1_SendString(rev_buf);
			Uart1_SendString("\r\n");
		 }
		 Set_NRF24L01_CE; 
		 //����ʱ϶����ʼѭ��
		 if(rev_buf[0] == 'S' && rev_buf[1] == 'T'){
//#ifdef TEST_VERSION
//			 Uart1_SendString((u8*)"��ʱ϶������...\r\n");
//#endif
			 absolute_frame_num = (*((unsigned int *)(rev_buf+2)));
			 current_frequency = WORK_FREQUENCY;
			 TIM_Cmd(TIM7,ENABLE); 
		 }else if(rev_buf[0] == 'A' && rev_buf[1] == 'N'){
			node_index_for_base_station = rev_buf[2] - '0' + rev_buf[3] - '0';
			Timer7_init();
			sprintf((char *)words_buf,"AN%02d",node_index_for_base_station);
			Wireless_Send_Data(words_buf);
//			end_node_add_flag = 1;
			current_frequency = WORK_FREQUENCY;
			RX_Mode();
		 }
		 EXTI_ClearITPendingBit(EXTI_Line1);
	 }
 }

void NRF24L01_Init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);           //ʹ��GPIO��ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;            //NRF24L01 CE��CS��  PC4��CS   PC5��CE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	Set_NRF24L01_CE;                                          //��ʼ��ʱ������
  Set_NRF24L01_CSN;                                   			//��ʼ��ʱ������
	
	//����NRF2401��IRQ
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;            //��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);             //�ⲿ�ж��߱��뿪��SYSCFG ʱ��
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);      //ӳ��IO�����ж���
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//���ȼ����
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	
	SPI1_init();                                      //��ʼ��SPI
	Clr_NRF24L01_CE; 	                               //ʧ��24L01
	Set_NRF24L01_CSN;                                  //SPIƬѡȡ��
	
	
	//�ر�ͬһ��SPI�ܽŵ�����SPI�豸
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);          
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;              
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_SetBits(GPIOG,GPIO_Pin_15);
	
//	//��ؼĴ�����ʼ��
//	Clr_NRF24L01_CE;	  
//  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
//	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
//	  
// 	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
//  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  
//	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_AW,0x03);	     //���õ�ַ���
//	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x00);	     //�ر��Զ��ط�	
// // 	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,100);	     //����RFͨ��Ƶ��
//  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪�� 	
//  	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    	
////  NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG1, 0x3f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
//  	Set_NRF24L01_CE;

  Clr_NRF24L01_CE;	  
  NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
	  
 	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	
  NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,node_index_for_end_node * 5);	     //����RFͨ��Ƶ��		  			
	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x00);//(���Ҹĳ��ط���ֹ�ˣ������Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��  
  NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x3f); //����TX�������,0db����,2Mbps,���������濪��   
	 
  Set_NRF24L01_CE; 
	
	return;
}


u8  Wireless_Send_Data(u8 *txbuf){
	u8 ret = 0;
	TX_Mode();
	delay_us(100);
  ret = NRF24L01_TxPacket(txbuf);
	delay_us(200);
	RX_Mode();
  delay_us(100);
	return ret;
}


void RX_Mode(void)
{
	Clr_NRF24L01_CE;
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x00);    //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ 
//	if(end_node_add_flag == 0 && extra_node_flag != 1){
//		NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,node_index_for_end_node * 5);	     //����RFͨ��Ƶ��		  	
//	}else if(end_node_add_flag == 1){
//		NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,WORK_FREQUENCY);	     //����RFͨ��Ƶ��		  	
//	}else if(extra_node_flag == 1 && end_node_add_flag == 0){
//		NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,ADD_EXTRA_FREQUENCY);	
//	}
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,current_frequency);
	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG1, 0x3f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	Set_NRF24L01_CE;
	
//	Clr_NRF24L01_CE;	  
//  NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
//	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
//	  
// 	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x00);    //ʹ��ͨ��0���Զ�Ӧ��    
//  NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
//  NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,100);	     //����RFͨ��Ƶ��		  
//  NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
//  NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��   
//  NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG1, 0x3f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
//  Set_NRF24L01_CE;
}	

void TX_Mode(void)
{
	Clr_NRF24L01_CE;
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x00);     //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x00); //ʹ��ͨ��0�Ľ��յ�ַ  
 	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x00);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
//	if(end_node_add_flag == 0 && extra_node_flag != 1){
//		NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,node_index_for_end_node * 5);	     //����RFͨ��Ƶ��		  	
//	}else if(end_node_add_flag == 1){
//		NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,WORK_FREQUENCY);	     //����RFͨ��Ƶ��		  	
//	}else if(extra_node_flag == 1 && end_node_add_flag == 0){
//		NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,ADD_EXTRA_FREQUENCY);	
//	}  
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,current_frequency);
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��  
	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG1,0x3e);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	Set_NRF24L01_CE;
	
//	Clr_NRF24L01_CE;	    
//  NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
//  NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
//  NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x00);     //ʹ��ͨ��0���Զ�Ӧ��    
//  NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x00); //ʹ��ͨ��0�Ľ��յ�ַ  
// 	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x00);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
//  NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,100);       //����RFͨ��Ϊ40,ADת�����ԣ���ʱ����Ϊ40
//  NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
//  NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG1,0x3e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
//	Set_NRF24L01_CE; 
}


//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{

	u8 state;   
    u32 timeout = 0; 
	Clr_NRF24L01_CE;
	NRF24L01_Write_Reg(FLUSH_TX,NOP);   //ˢ�·��ͻ�����ǧ��Ҫȥ��������������ʱ�򲻸���
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	Set_NRF24L01_CE;                                     //��������	   
	while(timeout <2000)
	{
	  timeout++;
	}
	state=NRF24L01_Read_Reg(STATUS);                     //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,0x7E);      //���TX_DS��MAX_RT�жϱ�־
	if(state&TX_OK)                                      //�������
	{
		return TX_OK;
	}
	return 0xff;                                         //����ԭ����ʧ��
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 state;		    							      
	state=NRF24L01_Read_Reg(STATUS);                //��ȡ״̬�Ĵ�����ֵ    	 
        //printf("\n\r Rx state %X", state);
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,0x7E); //���TX_DS��MAX_RT�жϱ�־,д1����Ϊ���
	if(state&RX_OK)                                 //���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;                                      //û�յ��κ�����
}


//ͨ��SPIд�Ĵ���
u8 NRF24L01_Write_Reg(u8 regaddr,u8 data)
{
	u8 status;	
    Clr_NRF24L01_CSN;                    //ʹ��SPI����
  	status =SPI1_ReadWriteByte(regaddr); //���ͼĴ����� 
  	SPI1_ReadWriteByte(data);            //д��Ĵ�����ֵ
  	Set_NRF24L01_CSN;                    //��ֹSPI����	   
  	return(status);       		         //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 regaddr)
{
	u8 reg_val;	    
 	Clr_NRF24L01_CSN;                //ʹ��SPI����		
  	SPI1_ReadWriteByte(regaddr);     //���ͼĴ�����
  	reg_val=SPI1_ReadWriteByte(NOP);//��ȡ�Ĵ�������
  	Set_NRF24L01_CSN;                //��ֹSPI����		    
  	return(reg_val);                 //����״ֵ̬
}	


u8  NRF24L01_Write_Buf(u8 regaddr, u8 *pBuf, u8 datalen)
{
	u8 status,u8_ctr;	    
 	Clr_NRF24L01_CSN;                                    //ʹ��SPI����
  	status = SPI1_ReadWriteByte(regaddr);                //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //д������	 
  	Set_NRF24L01_CSN;                                    //�ر�SPI����
  	return status;                                       //���ض�����״ֵ̬
}


u8 NRF24L01_Read_Buf(u8 regaddr,u8 *pBuf,u8 datalen)
{
	u8 status,u8_ctr;	       
  	Clr_NRF24L01_CSN;                     //ʹ��SPI����
  	status=SPI1_ReadWriteByte(regaddr);   //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(NOP);//��������
  	Set_NRF24L01_CSN;                     //�ر�SPI����
  	return status;                        //���ض�����״ֵ̬
}

u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i = 0;   	 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5);//����д��ĵ�ַ  	
	for(i=0;i<5;i++)
	{
	    if(buf1[i]!=0XA5)
		{	//printf("%X ",buf1[i]);
		    break;					   
		}
	}
	if(i!=5)
	{
	
		 Uart1_SendString(" . ��Ƶģ����ʧ��......\r\n");
	return 1;                               //NRF24L01����λ
	}
		
		 Uart1_SendString(" . ��Ƶģ����ͨ��......\r\n");
	return 0;		                                //NRF24L01��λ
}





