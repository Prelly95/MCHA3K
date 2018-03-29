#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/twi.h>
#include <avr/pgmspace.h>

#include "i2c.h"

#define F_SCL 400000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)//formula from data sheet

void I2C_Init(void)
{
	TWBR = (uint8_t)TWBR_val;
}

uint8_t I2C_Start(uint8_t address)
{
	// reset TWI control register
	TWCR = 0;
	// transmit START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)));

	// check if the start condition was successfully transmitted
	if((TWSR & 0xF8) != TW_START)
	{
		return 1;
		printf_P(PSTR("\nERROR\n\n"));
	}

	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );

	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) )
	{
		return 1;
		printf_P(PSTR("\nERROR\n\n"));
	}

	return 0;
}

uint8_t I2C_Write(uint8_t data)
{
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)));

	if( (TWSR & 0xF8) != TW_MT_DATA_ACK )
	{
		return 1;
		printf_P(PSTR("\nERROR\n\n"));
	}

	return 0;
}

uint8_t I2C_Read_Ack(void)
{

	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

uint8_t I2C_Read_Nack(void)
{

	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

uint8_t I2C_Transmit(uint8_t address, uint16_t length, uint8_t* data)
{
	if (I2C_Start(address))
	{
		return 1;
		printf_P(PSTR("\nERROR 1\n\n"));
	}

	for (uint16_t i = 0; i < length; i++)
	{
		if (I2C_Write(data[i]))
		{
			return 1;
			printf_P(PSTR("\nERROR 2\n\n"));
		}
	}

	I2C_Stop();

	return 0;
}

uint8_t I2C_Receive(uint8_t address, uint16_t length, uint8_t* data)
{
	if (I2C_Start(address))
	{
		return 1;
		printf_P(PSTR("\nERROR 3\n\n"));
	}

	for (uint16_t i = 0; i < (length-1); i++)
	{
		data[i] = I2C_Read_Ack();
	}
	data[(length-1)] = I2C_Read_Nack();

	I2C_Stop();

	return 0;
}

uint8_t I2C_WriteReg(uint8_t devaddr, uint8_t regaddr, uint16_t length, uint8_t* data)
{
	if (I2C_Start(devaddr))
	{
		return 1;
		printf_P(PSTR("\nERROR 4\n\n"));
	}

	I2C_Write(regaddr);

	for (uint16_t i = 0; i < length; i++)
	{
		if (I2C_Write(data[i]))
		{
			return 1;
			printf_P(PSTR("\nERROR 5\n\n"));
		}
	}

	I2C_Stop();

	return 0;
}

uint8_t I2C_ReadReg(uint8_t devaddr, uint8_t regaddr, uint16_t length, uint8_t* data)
{
	if (I2C_Start(devaddr))
	{
		return 1;
		printf_P(PSTR("\nERROR 6\n\n"));
	}

	I2C_Write(regaddr);

	if (I2C_Start(devaddr | 0x01))
	{
		return 1;
		printf_P(PSTR("\nERROR 7\n\n"));
	}

	for (uint16_t i = 0; i < (length-1); i++)
	{
		data[i] = I2C_Read_Ack();
	}

	data[(length-1)] = I2C_Read_Nack();

	I2C_Stop();

	return 0;
}

void I2C_Stop(void)
{
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	while(TWCR & (1<<TWSTO));
}

uint8_t I2C_ReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data)
{
	if(I2C_ReadReg(devAddr, regAddr, 1, data))
	{
		return 1;
	}
	return 0;
}

uint8_t I2C_ReadBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data)
{
	uint8_t b = 0;

	if(I2C_ReadByte(devAddr, regAddr, &b))
	{
		return 1;
	}

	*data = b & (1 << bitNum);

	return 0;
}

uint8_t I2C_ReadBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
{
	uint8_t b;
	uint8_t mask;

	if (I2C_ReadByte(devAddr, regAddr, &b))
	{
		return 1;
	}

	mask = ((1 << length) - 1) << (bitStart - length + 1);
	b &= mask;
	b >>= (bitStart - length + 1);
	*data = b;

	return 0;
}

uint8_t I2C_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
	if(I2C_WriteReg(devAddr, regAddr, 1, &data))
	{
		return 1;
	}
	return 0;
}

uint8_t I2C_WriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data)
{
	uint8_t b = 0;

	if(I2C_ReadBit(devAddr, regAddr, bitNum, &b))
	{
		return 1;
	}

	if(data)
	{
		b |= (1 << bitNum);
	}
	else
	{
		b &= ~(1 << bitNum);
	}

	if(I2C_WriteByte(devAddr, regAddr, b))
	{
		return 1;
	}
	return 0;

}

uint8_t I2C_WriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
	uint8_t b = 0;
	uint8_t mask = 0;

	if(I2C_ReadByte(devAddr, regAddr, &b))
	{
		return 1;
	}

	mask = ((1 << length) - 1) << (bitStart - length + 1);
	data <<= (bitStart - length + 1); // shift data into correct position
	data &= mask; // zero all non-important bits in data
	b &= ~(mask); // zero all important bits in existing byte
	b |= data; // combine data with existing byte

	if(I2C_WriteByte(devAddr, regAddr, b))
	{
		return 1;
	}
	return 0;
}
