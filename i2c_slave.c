#include "i2c_slave.h"


void I2C_Slave_Init(i2c_slave_init_t *i2c_init){
  HAL_I2C_EnableListen_IT(i2c_init->hi2c);
  i2c_data.transferState = waiting;
  _error_handler = i2c_init->error_handler;
}

void I2C_Error_Handler(){
  _error_handler();
}

void I2C_Set_Register(uint8_t reg, uint8_t data){
  i2c_data.i2c_memory[reg] = data;
}

uint8_t I2C_Get_Register(uint8_t reg){
  return i2c_data.i2c_memory[reg];
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t direction, uint16_t addrMatchCode) {
	switch (direction) {
		case I2C_DIRECTION_TRANSMIT:
			i2c_data.transferState = getRegisterAddress;
			if (HAL_I2C_Slave_Seq_Receive_IT(hi2c, &i2c_data.receiveBuffer, 1, I2C_FIRST_FRAME) != HAL_OK) {
				I2C_Error_Handler();
			}
		break;
		case I2C_DIRECTION_RECEIVE:
			i2c_data.transferState = sendData;
			if (HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &i2c_data.i2c_memory[i2c_data.registerAddress++], 255, I2C_NEXT_FRAME) != HAL_OK) {
				I2C_Error_Handler();
			}
		break;
    default:
      I2C_Error_Handler();
    break;
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	switch (i2c_data.transferState) {
		case getRegisterAddress:
			i2c_data.registerAddress = i2c_data.receiveBuffer;
			i2c_data.transferState = getData;
			if (HAL_I2C_Slave_Seq_Receive_IT(hi2c,&i2c_data.i2c_memory[i2c_data.registerAddress++], 1, I2C_FIRST_FRAME) != HAL_OK) {
				I2C_Error_Handler();
			}
		break;
		case getData:
			if (HAL_I2C_Slave_Seq_Receive_IT(hi2c, &i2c_data.i2c_memory[i2c_data.registerAddress++], 255, I2C_FIRST_FRAME) != HAL_OK) {
				I2C_Error_Handler();
			}
		break;
    default:
      I2C_Error_Handler();
    break;
	}
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
	if (HAL_I2C_GetError(hi2c) != HAL_I2C_ERROR_AF) {
		I2C_Error_Handler();
	}
}
