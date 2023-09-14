#include "one.h"

void Tmp_Init(void)
{
  DS18B20_Init();
}

void DS18B20_Delay(uint16_t time)
{
  uint8_t i;
  while(time)
  {    
    for(i=0;i<10;i++)
    {
    }
    time--;
  }
}

uint8_t DS18B20_Init(void)
{
  DS18B20_GPIO_CLK_ENABLE();
  DS18B20_Mode_Out_PP();
  DS18B20_GPIO_HIGH();	
  DS18B20_RST();
  return DS18B20_Presence();
}

void DS18B20_Mode_IPU(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = DS18B20_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DS18B20_GPIO, &GPIO_InitStruct);
}

void DS18B20_Mode_Out_PP(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = DS18B20_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DS18B20_GPIO, &GPIO_InitStruct); 	 
}

void DS18B20_RST(void)
{
  DS18B20_Mode_Out_PP();
  DS18B20_GPIO_LOW();
  DS18B20_Delay(750);
  DS18B20_GPIO_HIGH();
  DS18B20_Delay(15);
}

uint8_t DS18B20_Presence(void)
{
  uint8_t pulse_time = 0;
	
  DS18B20_Mode_IPU();
  while(DS18B20_Data_IN() && pulse_time < 100)
  {
    pulse_time++;
    DS18B20_Delay(1);
  }	
  if(pulse_time >= 100)
  {
    return 1;
  }
  else
  {
    pulse_time = 0;
  }
  while(!DS18B20_Data_IN() && pulse_time < 240)
  {
    pulse_time++;
    DS18B20_Delay(1);
  }	
  if(pulse_time >= 240)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

uint8_t DS18B20_Read_Bit(void)
{
  uint8_t dat;

  DS18B20_Mode_Out_PP();
  DS18B20_GPIO_LOW();
  DS18B20_Delay(10);
  DS18B20_Mode_IPU();
  if(DS18B20_Data_IN() == SET)
  {
    dat = 1;
  }
  else
  {
    dat = 0;
  }
  DS18B20_Delay(45);
  return dat;
}

uint8_t DS18B20_Read_Byte(void)
{
  uint8_t i, j, dat = 0;	
  for(i=0; i<8; i++) 
  {
    j = DS18B20_Read_Bit();		
    dat = (dat) | (j<<i);
  }
  return dat;
}

void DS18B20_Write_Byte(uint8_t dat)
{
  uint8_t i, testb;
  DS18B20_Mode_Out_PP();
  for(i=0;i<8;i++)
  {
    testb = dat&0x01;
    dat = dat>>1;		
    if(testb)
    {			
      DS18B20_GPIO_LOW();
      DS18B20_Delay(8);
      DS18B20_GPIO_HIGH();
      DS18B20_Delay(58);
    }		
    else
    {			
      DS18B20_GPIO_LOW();
      DS18B20_Delay(70);
      DS18B20_GPIO_HIGH();
      DS18B20_Delay(2);
    }
  }
}

void DS18B20_Skip_Rom(void)
{
  DS18B20_RST();	   	
  DS18B20_Presence();	 	
  DS18B20_Write_Byte(0xCC);
}

void DS18B20_Match_Rom(void)
{
  DS18B20_RST();	   	
  DS18B20_Presence();	 	
  DS18B20_Write_Byte(0x55);
}

float BSP_Get_Tmp_Skip_Rom(void)
{
  uint8_t tpmsb,tplsb;
  short s_tem;
  float f_tem;
	
  DS18B20_Skip_Rom();
  DS18B20_Write_Byte(0x44);				 
  DS18B20_Skip_Rom();
  DS18B20_Write_Byte(0xBE);
	
  tplsb = DS18B20_Read_Byte();		 
  tpmsb = DS18B20_Read_Byte(); 
  s_tem = tpmsb<<8;
  s_tem = s_tem | tplsb;
	
  if(s_tem < 0)
  {
    f_tem = (~s_tem+1)*0.0625;
  }
  else
  {
    f_tem = s_tem*0.0625;
  }
  return f_tem;
}

void DS18B20_Read_Id(uint8_t * DS18B20_ID)
{
  uint8_t uc;
  DS18B20_Write_Byte(0x33);
  for(uc=0;uc<8;uc++)
  {
    DS18B20_ID[uc] = DS18B20_Read_Byte();
  }
}

float BSP_Get_Tmp_Match_Rom(uint8_t * DS18B20_ID)
{
  uint8_t tpmsb,tplsb,i;
  short s_tem;
  float f_tem;
	
  DS18B20_Match_Rom();
  for(i=0;i<8;i++)
  {
    DS18B20_Write_Byte(DS18B20_ID[i]);	
  }
  DS18B20_Write_Byte(0x44);
  DS18B20_Match_Rom();
  for(i=0;i<8;i++)
  {
    DS18B20_Write_Byte(DS18B20_ID[i]);
  }
  DS18B20_Write_Byte(0xBE);
  tplsb = DS18B20_Read_Byte();		 
  tpmsb = DS18B20_Read_Byte(); 
  s_tem = tpmsb<<8;
  s_tem = s_tem | tplsb;
	
  if(s_tem < 0)
  {
    f_tem = (~s_tem+1)*0.0625;
  }
  else
  {
    f_tem = s_tem*0.0625;
  }
  return f_tem; 		
}
