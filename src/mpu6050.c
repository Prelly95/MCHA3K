#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/twi.h>
#include <avr/pgmspace.h>

#include "i2c.h"

#include "mpu6050.h"

uint8_t buffer[50];

uint8_t MPU6050_Init(void)
{
	MPU6050_SetClockSource(MPU6050_CLOCK_PLL_XGYRO);
	MPU6050_SetFullScaleGyroRange(MPU6050_GYRO_FS_250);
	MPU6050_SetFullScaleAccelRange(MPU6050_ACCEL_FS_2);
	MPU6050_SetSleepEnabled(FALSE);

	return 0;
}

void MPU6050_SetClockSource(uint8_t source)
{
    I2C_WriteBits(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

void MPU6050_SetFullScaleAccelRange(uint8_t range)
{
	I2C_WriteBits(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

void MPU6050_SetFullScaleGyroRange(uint8_t range)
{
	I2C_WriteBits(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

void MPU6050_SetSleepEnabled(uint8_t state)
{
	I2C_WriteBit(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, state);
}

void MPU6050_GetAcceleration(int16_t* x, int16_t* y, int16_t* z)
{
	I2C_ReadReg(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, 6, buffer);
	*x = (((int16_t)buffer[0]) << 8) | buffer[1];
	*y = (((int16_t)buffer[2]) << 8) | buffer[3];
	*z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void MPU6050_GetRotation(int16_t* x, int16_t* y, int16_t* z)
{
    I2C_ReadReg(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, 6, buffer);
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void MPU6050_GetMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz)
{
    I2C_ReadReg(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
    *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    *az = (((int16_t)buffer[4]) << 8) | buffer[5];
    *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}

int16_t MPU6050_GetTemperature(void)
{
    I2C_ReadReg(MPU6050_ADDRESS, MPU6050_RA_TEMP_OUT_H, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

uint8_t MPU6050_GetDeviceID(void)
{
    I2C_ReadBits(MPU6050_ADDRESS, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, buffer);
	printf_P(PSTR("Device ID = 0x%x\n"), buffer[0]);

    return buffer[0];
}


// /* Divide raw value by sensitivity scale factor */
// 		Xa = Acc_x/16384.0;
// 		Ya = Acc_y/16384.0;
// 		Za = Acc_z/16384.0;
//
// 		Xg = Gyro_x/16.4;
// 		Yg = Gyro_y/16.4;
// 		Zg = Gyro_z/16.4;
//
// 		/* Convert temperature in /c using formula */
// 		t = (Temperature/340.00)+36.53;
