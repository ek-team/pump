#include "htx.h"

void Send_Zero()
{
  uint8_t TxBuf[1] = {' '};
  HAL_UART_Transmit(&husart1,TxBuf,1,1000);
}

void Send_Startup(uint16_t Cmd,uint8_t Cs)
{
  uint8_t TxBuf[20];
	uint8_t i = 0;
	uint8_t Crc = 0;
	TxBuf[i++] = 0xA5;
	TxBuf[i++] = 0x5A;
	TxBuf[i++] = Cmd>>8;
	TxBuf[i++] = Cmd;
	TxBuf[i++] = 0;
	TxBuf[i++] = 1;
	TxBuf[i++] = Cs;
	Crc = Calculate_XOR(TxBuf,i);
	TxBuf[i++] = Crc;
	HAL_UART_Transmit(&husart1,TxBuf,i,1000);
}

void Send_State(uint8_t State,uint8_t Cs)
{
  uint8_t TxBuf[10];
	uint8_t i = 0;
	uint8_t Crc = 0;
	TxBuf[i++] = 0xA5;
	TxBuf[i++] = 0x5A;
	TxBuf[i++] = 03;
	TxBuf[i++] = 02;
	if(State == 1)
	{
	  TxBuf[i++] = 0;
	  TxBuf[i++] = 1;
	  TxBuf[i++] = State;
	}
	else if(State == 2)
	{
	  TxBuf[i++] = 0;
	  TxBuf[i++] = 2;
	  TxBuf[i++] = State;
		TxBuf[i++] = Cs;
	}
	Crc = Calculate_XOR(TxBuf,i);
	TxBuf[i++] = Crc;
	HAL_UART_Transmit(&husart1,TxBuf,i,1000);
}

