#ifndef _APP_H_
#define _APP_H_

#include "stm32f4xx_hal.h"
#include <math.h>
#include <stdio.h>

extern unsigned long preTimeMPU;
extern double gotXangle;
extern double gotYangle;
extern double angleX, angleY;
extern double accXangle, accYangle;
extern double gyroXrate, gyroYrate;
extern double MPU6050_Temperature;
extern double A;

//extern unsigned long preTimeMPU_1;
//extern double gotXangle_1;
//extern double gotYangle_1;
//extern double angleX_1, angleY_1;
//extern double accXangle_1, accYangle_1;
//extern double gyroXrate, gyroYrate_1;
//extern double MPU6050_Temperature_1;
//extern double A_1;

extern volatile uint16_t MPU_milliseconds;
extern uint8_t Angle_on ,flag_for_MPU_RESET;



#define I2C_DEVICE_ADDRESS 0xD0
#define RAD2DEG 57.2957786
#define PI 3.14159265
#define I2C_MEMADD_SIZE_8BIT    0x00000001U

typedef struct FLAG
{
	char timer2;
	char angle_offset;
}FLAG;



/* IMU Routines */
//void MPU6050_load_Angle_1(unsigned long milliseconds);
//void MPU6050_getAccel_1(double *accXangle, double *accYangle);
//void MPU6050_getGyro_1(double *accXangle, double *accYangle);
//void MPU6050_printAngle_1(unsigned long milliseconds);
//void MPU6050_reset_1(void);
//void MPU6050_printTemper_1(void);
//void MPU6050_load_Temperature_1(void);

void MPU6050_load_Angle(unsigned long milliseconds);
void MPU6050_getAccel(double *accXangle, double *accYangle);
void MPU6050_getGyro(double *accXangle, double *accYangle);
void MPU6050_printAngle(unsigned long milliseconds);
void MPU6050_reset(void);
void MPU6050_printTemper(void);
void MPU6050_load_Temperature(void);

#endif	// add.h
