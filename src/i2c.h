#ifndef I2C_H
#define I2C_H

void I2C_Init(void);
uint8_t I2C_Start(uint8_t address);
void I2C_Stop(void);
uint8_t I2C_Write(uint8_t data);
uint8_t I2C_Read_Ack(void);
uint8_t I2C_Read_Nack(void);
uint8_t I2C_Transmit(uint8_t address, uint16_t length, uint8_t *data);
uint8_t I2C_Receive(uint8_t address, uint16_t length, uint8_t *data);
uint8_t I2C_WriteReg(uint8_t devaddr, uint8_t regaddr, uint16_t length, uint8_t *data);
uint8_t I2C_ReadReg(uint8_t devaddr, uint8_t regaddr, uint16_t length, uint8_t *data);

uint8_t I2C_ReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
uint8_t I2C_ReadBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);
uint8_t I2C_ReadBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
uint8_t I2C_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
uint8_t I2C_WriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
uint8_t I2C_WriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);

#endif
