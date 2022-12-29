#include "app.h"
#include "main.h"

#define mpu6050_de 2

FLAG flag = {0, 1};									// timer2 : false, angle_offset : true
unsigned long preTimeMPU;
uint8_t pData[1] = {0x01};
double gotXangle;
double gotYangle;
double angleX, angleY;
double accXangle, accYangle;
double gyroXrate, gyroYrate;
int16_t MPU6050_temper;
uint8_t MPU6050_Rec_Data[2];
double MPU6050_Temperature;
char MPU6050_str[30];
double A;



int16_t accX, accY, accZ;
//
//FLAG flag_1 = {0, 1};									// timer2 : false, angle_offset : true
//unsigned long preTimeMPU_1;
//uint8_t pData_1[1] = {0x01};
//double gotXangle_1;
//double gotYangle_1;
//double angleX_1, angleY_1;
//double accXangle_1, accYangle_1;
//double gyroXrate_1, gyroYrate_1;
//int16_t MPU6050_temper_1;
//uint8_t MPU6050_Rec_Data_1[2];
//double MPU6050_Temperature_1;
//char MPU6050_str_1[30];
//double A_1;


TIM_HandleTypeDef htim2;
I2C_HandleTypeDef hi2c1;
//I2C_HandleTypeDef hi2c2;

void MPU6050_printAngle(unsigned long milliseconds)
{
	MPU6050_load_Angle(milliseconds);
#if mpu6050_de == 1
	sprintf(MPU6050_str,"X:%.1f Y:%.1f A:%.2f", angleX, angleY,A);
	lcdSetCursor(0, 60);
	lcdPrintf(MPU6050_str);
#elif mpu6050_de == 2
	sprintf(MPU6050_str,"Y:%05.1f A:%05.2f", angleY,A);
	lcdSetCursor(0, 60);
	lcdPrintf(MPU6050_str);

	sprintf(MPU6050_str,"acc:%05.1f, gyro:%05.1f",accYangle,gyroYrate );
	lcdSetCursor(0, 100);
	lcdPrintf(MPU6050_str);

	sprintf(MPU6050_str,"accx:%06d  ",accX);
	lcdSetCursor(0, 140);
	lcdPrintf(MPU6050_str);

	sprintf(MPU6050_str,"accz:%06d  ",accZ );
	lcdSetCursor(0, 160);
	lcdPrintf(MPU6050_str);
	#endif
}

void MPU6050_reset(void)
{
	flag.angle_offset=1;
	preTimeMPU=0;
}


void MPU6050_load_Temperature(void){
	HAL_I2C_Mem_Read(&hi2c1, 0xD0, 0x41, 1, MPU6050_Rec_Data, 2, 100);
	MPU6050_temper = (int16_t)(MPU6050_Rec_Data[0] << 8 | MPU6050_Rec_Data[1]);
	MPU6050_Temperature = (float)((int16_t)MPU6050_temper / (float)340.0 + (float)36.53);
}
void MPU6050_printTemper(void){
	MPU6050_load_Temperature();
	sprintf(MPU6050_str,"%.2f C",MPU6050_Temperature);
}


void MPU6050_load_Angle(unsigned long milliseconds)
{
	double timePass;
	unsigned long present;
	static const double k = 3.0;
	HAL_I2C_Mem_Write(&hi2c1, I2C_DEVICE_ADDRESS, 0x6B, I2C_MEMADD_SIZE_8BIT, pData, 1, 500);								// Set Mode
	MPU6050_getAccel(&accXangle, &accYangle);						// get accel value
	MPU6050_getGyro(&gyroXrate, &gyroYrate);						// get gyro value

	present = milliseconds;
	timePass = (present - preTimeMPU)/1000.0;
	preTimeMPU = present;
	A = k/(k+timePass);

	if(flag.angle_offset){											// initial state
		flag.angle_offset = 0;
		//		gotXangle = accXangle;
		gotYangle = accYangle;
	}
	else{
		//		gotXangle = A*(gotXangle + gyroXrate*timePass) + (1.0 - A)*accXangle;
		gotYangle = A*(gotYangle + gyroYrate*timePass) + (1.0 - A)*accYangle;
	}
	//	angleX = gotXangle -180.0+1.8;
	angleY = gotYangle -180.0-18.4;

}

void MPU6050_getGyro(double *gyroXrate, double *gyroYrate)
{
	uint8_t buffer[6] = {0, };
	int16_t gyroX, gyroY, gyroZ;
	//	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x43, I2C_MEMADD_SIZE_8BIT, &buffer[0], 1, 500);
	//	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x44, I2C_MEMADD_SIZE_8BIT, &buffer[1], 1, 500);
	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x45, I2C_MEMADD_SIZE_8BIT, &buffer[2], 1, 500);
	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x46, I2C_MEMADD_SIZE_8BIT, &buffer[3], 1, 500);
	//	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x47, I2C_MEMADD_SIZE_8BIT, &buffer[4], 1, 500);
	//	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x48, I2C_MEMADD_SIZE_8BIT, &buffer[5], 1, 500);
	//	gyroX = (int)buffer[0] << 8 | (int)buffer[1];
	gyroY = (int)buffer[2] << 8 | (int)buffer[3];
	//	gyroZ = (int)buffer[4] << 8 | (int)buffer[5];
	//	*gyroXrate = (double)gyroX/131.0+6.2;
	*gyroYrate = -((double)gyroY/131.0)+0.8;
}

void MPU6050_getAccel(double *accXangle, double *accYangle)
{
	uint8_t buffer[6] = {0, };
	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x3B, I2C_MEMADD_SIZE_8BIT, &buffer[0], 1, 500);
	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x3C, I2C_MEMADD_SIZE_8BIT, &buffer[1], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x3D, I2C_MEMADD_SIZE_8BIT, &buffer[2], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x3E, I2C_MEMADD_SIZE_8BIT, &buffer[3], 1, 500);
	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x3F, I2C_MEMADD_SIZE_8BIT, &buffer[4], 1, 500);
	HAL_I2C_Mem_Read(&hi2c1, I2C_DEVICE_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, &buffer[5], 1, 500);
	accX = (int)buffer[0] << 8 | (int)buffer[1];
	//	accY = (int)buffer[2] << 8 | (int)buffer[3];
	accZ = (int)buffer[4] << 8 | (int)buffer[5];
	//	*accXangle = (atan2(accY,accZ)+PI)*RAD2DEG;
	*accYangle = (atan2(accX,accZ)+PI)*RAD2DEG;
}
//
//
////////////////
//
//void MPU6050_printAngle_1(unsigned long milliseconds)
//{
//	MPU6050_load_Angle_1(milliseconds);
//
//#if mpu6050_de == 1
//	sprintf(MPU6050_str_1,"X:%.1f Y:%.1f A:%.2f", angleX_1, angleY_1,A_1);
//	lcdSetCursor(0, 120);
//	lcdPrintf(MPU6050_str_1);
//
//#elif mpu6050_de == 2
//	sprintf(MPU6050_str_1,"X:%.1f Y:%.1f A:%.2f", angleX_1, angleY_1,A_1);
//	lcdSetCursor(0, 120);
//	lcdPrintf(MPU6050_str_1);
//
//	sprintf(MPU6050_str_1,"acc:%.1f, gyro:%.1f",accXangle_1,gyroXrate_1 );
//	lcdSetCursor(0, 140);
//	lcdPrintf(MPU6050_str_1);
//
//	sprintf(MPU6050_str_1,"acc:%.1f, gyro:%.1f",accYangle_1,gyroYrate_1 );
//	lcdSetCursor(0, 160);
//	lcdPrintf(MPU6050_str_1);
//#endif
//}
//
//void MPU6050_reset_1(void)
//{
//	flag_1.angle_offset=1;
//	preTimeMPU_1=0;
//}
//
//
//void MPU6050_load_Temperature_1(void){
//	HAL_I2C_Mem_Read(&hi2c2, 0xD0, 0x41, 1, MPU6050_Rec_Data, 2, 100);
//	MPU6050_temper_1 = (int16_t)(MPU6050_Rec_Data_1[0] << 8 | MPU6050_Rec_Data_1[1]);
//	MPU6050_Temperature_1 = (float)((int16_t)MPU6050_temper_1 / (float)340.0 + (float)36.53);
//}
//void MPU6050_printTemper_1(void){
//	MPU6050_load_Temperature_1();
//	sprintf(MPU6050_str_1,"%.2f C",MPU6050_Temperature_1);
//}
//
//
//void MPU6050_load_Angle_1(unsigned long milliseconds)
//{
//	double timePass;
//	unsigned long present;
//	static const double k = 3.0;
//	HAL_I2C_Mem_Write(&hi2c2, I2C_DEVICE_ADDRESS, 0x6B, I2C_MEMADD_SIZE_8BIT, pData_1, 1, 500);								// Set Mode
//	MPU6050_getAccel_1(&accXangle_1, &accYangle_1);						// get accel value
//	MPU6050_getGyro_1(&gyroXrate_1, &gyroYrate_1);						// get gyro value
//
//	present = milliseconds;
//	timePass = (present - preTimeMPU_1)/1000.0;
//	preTimeMPU_1 = present;
//	A_1 = k/(k+timePass);
//
//	if(flag_1.angle_offset){											// initial state
//		flag_1.angle_offset = 0;
//		gotXangle_1 = accXangle_1;
//		gotYangle_1 = accYangle_1;
//	}
//	else{
//		gotXangle_1 = A_1*(gotXangle_1 + gyroXrate_1*timePass) + (1.0 - A_1)*accXangle_1;
//		gotYangle_1 = A_1*(gotYangle_1 + gyroYrate_1*timePass) + (1.0 - A_1)*accYangle_1;
//	}
//	angleX_1 = gotXangle_1 -180.0+0.3;
//	angleY_1 = gotYangle_1 -180.0 +3.0;
//
//}
//
//void MPU6050_getGyro_1(double *gyroXrate, double *gyroYrate)
//{
//	uint8_t buffer[6] = {0, };
//	int16_t gyroX, gyroY, gyroZ;
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x43, I2C_MEMADD_SIZE_8BIT, &buffer[0], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x44, I2C_MEMADD_SIZE_8BIT, &buffer[1], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x45, I2C_MEMADD_SIZE_8BIT, &buffer[2], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x46, I2C_MEMADD_SIZE_8BIT, &buffer[3], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x47, I2C_MEMADD_SIZE_8BIT, &buffer[4], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x48, I2C_MEMADD_SIZE_8BIT, &buffer[5], 1, 500);
//	gyroX = (int)buffer[0] << 8 | (int)buffer[1];
//	gyroY = (int)buffer[2] << 8 | (int)buffer[3];
//	gyroZ = (int)buffer[4] << 8 | (int)buffer[5];
//	*gyroXrate = (double)gyroX/131.0+3.0;
//	*gyroYrate = -((double)gyroY/131.0)+0.4;
//}
//
//void MPU6050_getAccel_1(double *accXangle, double *accYangle)
//{
//	uint8_t buffer[6] = {0, };
//	int16_t accX, accY, accZ;
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x3B, I2C_MEMADD_SIZE_8BIT, &buffer[0], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x3C, I2C_MEMADD_SIZE_8BIT, &buffer[1], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x3D, I2C_MEMADD_SIZE_8BIT, &buffer[2], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x3E, I2C_MEMADD_SIZE_8BIT, &buffer[3], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x3F, I2C_MEMADD_SIZE_8BIT, &buffer[4], 1, 500);
//	HAL_I2C_Mem_Read(&hi2c2, I2C_DEVICE_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, &buffer[5], 1, 500);
//	accX = (int)buffer[0] << 8 | (int)buffer[1];
//	accY = (int)buffer[2] << 8 | (int)buffer[3];
//	accZ = (int)buffer[4] << 8 | (int)buffer[5];
//	*accXangle = (atan2(accY,accZ)+PI)*RAD2DEG;
//	*accYangle = (atan2(accX,accZ)+PI)*RAD2DEG;
//}
//
//// application source
//
//
