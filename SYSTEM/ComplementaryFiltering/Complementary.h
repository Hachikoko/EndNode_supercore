#ifndef _COMPLEMENTARY_H_
#define _COMPLEMENTARY_H_
//#include "MPU9250.h"
#include "main.h"
#include "stdbool.h"
#include "sys.h"

#define so3_comp_params_Kp 1.0f
#define so3_comp_params_Ki  0.05f



/* output euler angles */

typedef struct fliter_data{
	
	bool ready;
	float gyroOffset[3];
}Fliter_Data;


typedef struct result_data{
	
	float euler[3];       	//ŷ����
	float q[4]; 			//��Ԫ�� 
}Fliter_Result_Data;

typedef struct mpu9250_raw_data{
	short ax;			//���ٶ�
	short ay;
	short az;
	short gx;			//������
	short gy;
	short gz;
	short mx;			//������
	short my;
	short mz;
}MPU9250_RAW_DATD;

typedef struct mpu9250_data{
	float ax;			//���ٶ�
	float ay;
	float az;
	float gx;			//������
	float gy;
	float gz;
	float mx;			//������
	float my;
	float mz;
}MPU9250_DATD;

static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;        /** quaternion of sensor frame relative to auxiliary frame */
static float dq0 = 0.0f, dq1 = 0.0f, dq2 = 0.0f, dq3 = 0.0f;        /** quaternion of sensor frame relative to auxiliary frame */
static float gyro_bias[3] = {0.0f, 0.0f, 0.0f}; /** bias estimation */
static float q0q0, q0q1, q0q2, q0q3;
static float q1q1, q1q2, q1q3;
static float q2q2, q2q3;
static float q3q3;
static bool bFilterInit = 0;


float invSqrt(float number);
void complementary_fliter(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void complementary_init(float ax, float ay, float az, float mx, float my, float mz);
void complementary_task(Fliter_Data*,Fliter_Result_Data*,MPU9250_DATD*);
void complementary_data_init(Fliter_Data*);


#endif //complementary.h

