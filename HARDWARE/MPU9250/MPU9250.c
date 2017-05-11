#include "MPU9250.h"
#include "IIC.h"
#include "delay.h"
#include "usart.h"
#include "main.h"
#include "sys.h"

extern char ret_words[100];

u8 MPU9250_Init(void){
	u8 res_MPU,res_AKM;
	char ID[20];
	IIC_Init();
	MPU_Write_Byte(MPU9250_PWR_MGMT_1,0X80);	//��λMPU9250
	delay_ms(100);
	MPU_Write_Byte(MPU9250_PWR_MGMT_1,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(MPU9250_GYRO_FULL_SCALE_500DPS);			//�����Ǵ�����,��500dps
	MPU_Set_Accel_Fsr(MPU9250_FULL_SCALE_4G);					//���ٶȴ�����,��2g
	MPU_Write_Byte(MPU9250_INT_ENABLE,0X00);	//�ر������ж�
	MPU_Write_Byte(MPU9250_USER_CTRL,0X00);	//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU9250_FIFO_EN,0X00);	//�ر�FIFO
	MPU_Write_Byte(MPU9250_INT_PIN_CFG,0X82);	//INT���ŵ͵�ƽ��Ч ����bypass IIC
	MPU_Write_Byte(MPU9250_PWR_MGMT_1,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
	MPU_Write_Byte(MPU9250_PWR_MGMT_2,0X00);	//���ٶ��������Ƕ�����
	AKM_Write_Byte(MPU9250_MAG_RSV,0x01);
	AKM_Write_Byte(MPU9250_MAG_CNTL,0x12);		//�������̹���
	MPU_Set_Rate(800);						//���ò�����Ϊ50Hz
	res_MPU = MPU_Read_Byte(MPU9250_WHO_AM_I);
	res_AKM = AKM_Read_Byte(MPU9250_MAG_WIA);
#ifdef TEST_MPU
	sprintf(ID,"%x,%x\r\n",res_MPU,res_AKM);
	Uart1_SendString((u8*)ID);
#endif
	if(res_MPU == MPU9250_ID && res_AKM == MPU9250_AKM_ID)//����ID��ȷ
	{
		return 0;
 	}else return 1;
}

//void MPU_Get_9_Axis(MPU9250_DATD * mpu_data){
//	u8 ret = 0;
//	
//	int temp_data;
//	int full_data = 32768;
//	MPU9250_RAW_DATD mpu_raw_data;
//	
//	ret = MPU_Get_Accelerometer(&(mpu_raw_data.ax),&(mpu_raw_data.ay),&(mpu_raw_data.az));
//	if(ret != 0){
//		Uart1_SendString((u8*)"get accelerometer fail!\r\n");
//		return;
//	}
//	
//	ret = MPU_Get_Gyroscope(&(mpu_raw_data.gx),&(mpu_raw_data.gy),&(mpu_raw_data.gz));
//	if(ret != 0){
//		Uart1_SendString((u8*)"get gyroscope fail!\r\n");
//		return;
//	}
//	
//	ret = MPU_Get_Gyroscope(&(mpu_raw_data.mx),&(mpu_raw_data.my),&(mpu_raw_data.mz));
//	if(ret != 0){
//		Uart1_SendString((u8*)"get gyroscope fail!\r\n");
//		return;
//	}
//	
//	temp_data = mpu_raw_data.ax;
//	mpu_data->ax = (((float)temp_data)/((float)full_data)) * 4.0;
//	temp_data = mpu_raw_data.ay;
//	mpu_data->ay = (((float)temp_data)/((float)full_data)) * 4.0;
//	temp_data = mpu_raw_data.az;
//	mpu_data->az = (((float)temp_data)/((float)full_data)) * 4.0;
//	
//	temp_data = mpu_raw_data.gx;
//	mpu_data->gx = (((float)temp_data)/((float)full_data)) * 500.0;
//	temp_data = mpu_raw_data.gy;
//	mpu_data->gy = (((float)temp_data)/((float)full_data)) * 500.0;
//	temp_data = mpu_raw_data.gz;
//	mpu_data->gz = (((float)temp_data)/((float)full_data)) * 500.0;
//	
//	mpu_data->mx = mpu_raw_data.mx * 0.15;
//	mpu_data->my = mpu_raw_data.my * 0.15;
//	mpu_data->mz = mpu_raw_data.mz * 0.15;
//	
//}

u8 MPU_get_ACC_offset(MPU9250_RAW_DATD* acc_offset){
	u8 buf[6],ret;
	ret = MPU_Read_Len(MPU9250_ADDR,MPU9250_ACCEL_XOUT_H,6,buf);
	
}
u8 MPU_get_GYR_offset(MPU9250_RAW_DATD* gry_offset){
	u8 buf[6],ret;
}

void MPU_Get_9_Axis_Raw(MPU9250_RAW_DATD * mpu_raw_data){
	u8 ret = 0;
	
	ret = MPU_Get_Accelerometer(&(mpu_raw_data->ax),&(mpu_raw_data->ay),&(mpu_raw_data->az));
	if(ret != 0){
		Uart1_SendString((u8*)"get accelerometer fail!\r\n");
		return;
	}
	
	ret = MPU_Get_Gyroscope(&(mpu_raw_data->gx),&(mpu_raw_data->gy),&(mpu_raw_data->gz));
	if(ret != 0){
		Uart1_SendString((u8*)"get gyroscope fail!\r\n");
		return;
	}
	
	ret = MPU_Get_Magnetometer(&(mpu_raw_data->mx),&(mpu_raw_data->my),&(mpu_raw_data->mz));
	if(ret != 0){
		Uart1_SendString((u8*)"get magnetometer fail!\r\n");
		return;
	}
	
}

//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU9250_ADDR,MPU9250_ACCEL_XOUT_H,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU9250_ADDR,MPU9250_GYRO_XOUT_H,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}

//�õ���������ֵ(ԭʼֵ)
//mx,my,mz:��������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Magnetometer (short *mx,short *my,short *mz)
{
    u8 buf[6],res;
	u8 x_axis,y_axis,z_axis; 
	
	x_axis = AKM_Read_Byte(MPU9250_MAG_ASAX);
	y_axis = AKM_Read_Byte(MPU9250_MAG_ASAY);
	z_axis = AKM_Read_Byte(MPU9250_MAG_ASAZ);
	
	#ifdef TEST_MPU
	sprintf(ret_words,"x_axis:%d,y_axis:%d,z_axis:%d\r\n",x_axis,y_axis,z_axis);
	Uart1_SendString((u8*)ret_words);
	#endif
	
	if(AKM_Read_Byte(MPU9250_MAG_ST1) == MPU9250_MAG_DATA_IS_READY){
		buf[0]= AKM_Read_Byte(MPU9250_MAG_XOUT_L);
		if(AKM_Read_Byte(MPU9250_MAG_ST2) != MPU9250_MAG_NO_HOFL){
			res =  1;
		}
		
		buf[1]= AKM_Read_Byte(MPU9250_MAG_XOUT_H);
		if(AKM_Read_Byte(MPU9250_MAG_ST2) != MPU9250_MAG_NO_HOFL){
			res =  1;
		}
		
		buf[2]= AKM_Read_Byte(MPU9250_MAG_YOUT_L);
		if(AKM_Read_Byte(MPU9250_MAG_ST2) != MPU9250_MAG_NO_HOFL){
			res =  1;
		}
				
		buf[3]= AKM_Read_Byte(MPU9250_MAG_YOUT_H);
		if(AKM_Read_Byte(MPU9250_MAG_ST2) != MPU9250_MAG_NO_HOFL){
			res =  1;
		}
		
		buf[4]= AKM_Read_Byte(MPU9250_MAG_ZOUT_L);
		if(AKM_Read_Byte(MPU9250_MAG_ST2) != MPU9250_MAG_NO_HOFL){
			res =  1;
		}
		
		buf[5]= AKM_Read_Byte(MPU9250_MAG_ZOUT_H);
		if(AKM_Read_Byte(MPU9250_MAG_ST2) != MPU9250_MAG_NO_HOFL){
			res =  1;
		}
		
		if (1 == res){
			return res;
		}
		
		*mx=((u16)buf[1]<<8)|buf[0];  
		*mx *= (((x_axis - 128) >> 8) + 1);
		*my=((u16)buf[3]<<8)|buf[2];  
		*my *= (((y_axis - 128) >> 8) + 1);
		*mz=((u16)buf[5]<<8)|buf[4];
		*mz *= (((z_axis - 128) >> 8) + 1);
	}


	
	#ifdef TEST_MPU
	sprintf(ret_words,"mx:%d,my:%d,mz:%d\r\n",*mx,*my,*mz);
	Uart1_SendString((u8*)ret_words);
	#endif

    return res;
}

//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU9250_CONFIG,data);//�������ֵ�ͨ�˲���  
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU9250_SMPLRT_DIV,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}


u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU9250_GYRO_CONFIG,fsr<<3);//���������������̷�Χ  
}

//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU9250_ACCEL_CONFIG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}

//IIC������һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 AKM_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	IIC_Send_Byte((MPU9250_MAG_ADDRESS<<1)|0);//����������ַ+д����	
	IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((MPU9250_MAG_ADDRESS<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=IIC_Read_Byte(0);//��ȡ����,����nACK 
    IIC_Stop();			//����һ��ֹͣ���� 
	return res;		
}

//IICд����һ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
u8 AKM_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((MPU9250_MAG_ADDRESS<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	IIC_Send_Byte(data);//��������
	if(IIC_Wait_Ack())	//�ȴ�ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}


//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	IIC_Send_Byte((MPU9250_ADDR<<1)|0);//����������ַ+д����	
	IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((MPU9250_ADDR<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=IIC_Read_Byte(0);//��ȡ����,����nACK 
    IIC_Stop();			//����һ��ֹͣ���� 
	return res;		
}



//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((addr<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//������,����nACK 
		else *buf=IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//����һ��ֹͣ���� 
	return 0;	
}


//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((MPU9250_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	IIC_Send_Byte(data);//��������
	if(IIC_Wait_Ack())	//�ȴ�ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}


//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);	//��������
		if(IIC_Wait_Ack())		//�ȴ�ACK
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 

//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{

	USART_SendData(USART1,c); 
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
    

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
