#include "main.h"

struct IAP_BUF      //��������֡�Ĵ���ṹ��
{
	uint8_t   Buf[APP_FLASH_LEN];
	uint16_t  Len;
} AppBin;
struct IAP_BUF AppBin  __attribute__((at(0x20001000)))={{0},0};

uint8_t U1_RxData;
uint8_t Rx_Cplt_Flag = 0;

int main(void)
{  
	__enable_irq();
  HAL_Init();
  SysClk_Init();
	GPIO_Init();
  USART_Init();
	printf(" ����Boot ");
  while(1)                            
  {	 
    Read_IAP_Data();
		if(APP_Flag == 1)
    {
			printf(" ׼������ ");
			Send_Startup(0x0300,1);
		  while(1)
			{
				HAL_Delay(1000);
				printf("APP���� %d ��ǰָ�� %d",APP_Len,AppBin.Len);
			  for(uint16_t i=0;i<200;i++)
			  {
			  	if(AppBin.Buf[i] == 0xA5 && AppBin.Buf[i+1] == 0x5A && AppBin.Buf[i+2] == 0x83) //���߸���ָ��
				  {
					  printf(" �ٴ�׼������ ");
            
						uint16_t Len = (AppBin.Buf[i+6]<<8) + AppBin.Buf[i+7];
	          uint16_t Crc = AppBin.Buf[i+8];
		        uint16_t Flag = 0xA55A;
			
	          STMFLASH_Write(0x0803F820,(uint16_t*)&Len,1);
	          STMFLASH_Write(0x0803F820 + 2,(uint16_t*)&Crc,1);
		        STMFLASH_Write(0x0803F820 + 4,(uint16_t*)&Flag,1);
						
						Read_IAP_Data();
					  Send_Startup(0x0300,1);
						
				    AppBin.Len = 0;
						AppBin.Buf[i] = 0x00;   //���������־λ
						AppBin.Buf[i+1] = 0x00;
						AppBin.Buf[i+2] = 0x00;
						break;
				   }
			  }
			  if(Rx_Cplt_Flag == 1)
				{
					__set_PRIMASK(1);
					printf(" ��ʼУ�� ");
				  uint8_t Crc = 0;
					Rx_Cplt_Flag = 0;
				  Crc = Calculate_XOR(AppBin.Buf,AppBin.Len);
					if(Crc == APP_Crc)
					{
					  if(((*( __IO uint32_t * )(0x20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
						{
              IAP_Write_App_Bin(APP_ADDR, AppBin.Buf,AppBin.Len);								 
							if(((*( __IO uint32_t * )(APP_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
							{	 
								Clear_IAP_Data();
								Send_State(1,0);
								printf(" ��д�ɹ� ");
								printf(" ����APP ");
								IAP_ExecuteApp(APP_ADDR);
							}
						}
						else
						{
							Clear_IAP_Data();
							Send_State(2,2);
							printf(" ��дʧ�� ");
							printf(" ����APP ");
							IAP_ExecuteApp(APP_ADDR);
						}
          }
					else
          {
						Clear_IAP_Data();
						Send_State(2,1);
						printf(" У��ʧ�� ");
						printf(" ����APP ");
						IAP_ExecuteApp(APP_ADDR);
					}
				}
			}
    }
		else if(APP_Flag == 0)
		{
			printf(" ����APP ");
      IAP_ExecuteApp(APP_ADDR);
		}
	}    
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if(huart->Instance == USART1)
  {
    if(AppBin.Len<APP_FLASH_LEN)
    {
		  AppBin.Buf[AppBin.Len++] = U1_RxData;
		  if(AppBin.Len == APP_Len)
		  {
			  Rx_Cplt_Flag = 1;
		  }
    }
    HAL_UART_Receive_IT(&husart1,&U1_RxData,1);
	}
}
