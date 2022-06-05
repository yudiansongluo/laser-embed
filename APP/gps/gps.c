#include "gps.h"
#include "SysTick.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "function.h"


unsigned char gpstest1[]={0xb5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00 ,0x00 ,0xd0, 0x08, 0x00 ,0x00 ,0x00, 0xc2 ,0x01 ,0x00 ,0x01 , 0x00, 0x02 ,0x00, 0x00  ,0x00  ,0x00  ,0x00  ,0xb9 ,0x48};//115200
unsigned char gpstest2[]={0xB5 , 0x62, 0x06 , 0x01 , 0x02 , 0x00 , 0xF0 , 0x04 , 0xFD , 0x15};  //RMC
unsigned char gpstest3[]={0xB5 , 0x62 , 0x06 , 0x01 , 0x08 , 0x00 , 0x01 , 0x07 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x17 , 0xDC };//pvt
unsigned char gpstest4[]={0xB5 , 0x62 , 0x06 , 0x09 , 0x0D , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0xFF , 0xFF , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x03 , 0x1D , 0xAB};
unsigned char gpstest8[]={0xb5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00 ,0x00 ,0xd0, 0x08, 0x00 ,0x00 ,0x00, 0xc2 ,0x01 ,0x00 ,0x01 , 0x00, 0x02 ,0x00, 0x00  ,0x00  ,0x00  ,0x00  ,0xb9 ,0x48,0x0a,0x0d};
unsigned char gpstest5[]={0xB5 , 0x62, 0x06 , 0x01 , 0x03 , 0x00 , 0xF0 , 0x00 , 0x00 , 0xFA ,0x0F};  //GGA
unsigned char gpstest6[]={0xB5 , 0x62, 0x06 , 0x01 , 0x03 , 0x00 , 0xF0 , 0x01 , 0x00 , 0xFB ,0x11};  //GLL
unsigned char gpstest7[]={0xB5 , 0x62, 0x06 , 0x01 , 0x03 , 0x00 , 0xF0 , 0x02 , 0x00 , 0xFC ,0x13};  //GSA
unsigned char gpstest9[]={0xB5 , 0x62, 0x06 , 0x01 , 0x03 , 0x00 , 0xF0 , 0x05 , 0x00 , 0xFF ,0x19};  //GLL
unsigned char gpstest10[]={0xB5 , 0x62, 0x06 , 0x01 , 0x03 , 0x00 , 0xF0 , 0x03 , 0x00 , 0xFD ,0x15};  //GSA
unsigned char gpstest11[]={0xB5 , 0x62, 0x06 , 0x01 , 0x08 , 0x00 , 0xF0 , 0x04 , 0x00 , 0x5A ,0x00,0x00,0x00,0x00,0x5D,0x01};  //GSA


void gpsinit()
{

USART2_printf("$CCSIR,3,3*48\r\n");	
	delay_us(50);
USART2_printf("$CCSIR,3,3*48\r\n");	
		delay_us(50);
	
	

	
USART2_printf("$CCMSG,GGA,1,0,*19\r\n");	
	delay_us(50);
USART2_printf("$CCMSG,GGA,1,0,*19\r\n");	
		delay_us(50);
	
USART2_printf("$CCMSG,GSA,1,0,*0D\r\n");	
	delay_us(50);
USART2_printf("$CCMSG,GSA,1,0,*0D\r\n");	
		delay_us(50);
	
	USART2_printf(" $CCMSG,GSV,1,0,*1A\r\n");	
	delay_us(50);
USART2_printf(" $CCMSG,GSV,1,0,*1A\r\n");	
		delay_us(50);
	
USART2_printf("$CCINV,100,*60\r\n");
delay_us(50);
USART2_printf("$CCINV,100,*60\r\n");
	delay_us(50);

	USART2_printf("$CCMSG,RMC,1,40,*30\r\n");	
	delay_us(50);
  USART2_printf("$CCMSG,RMC,1,40,*30\r\n");	
		delay_us(50);
	
	USART2_printf("$CCSIR,3,3*48\r\n");
delay_us(50);
USART2_printf("$CCSIR,3,3*48\r\n");
	delay_us(50);
		
}
		


void ublox_gpsinit()
{
//pvt
	delay_ms(1000);
UART_PutBytes(USART2 ,gpstest3,sizeof(gpstest3));
	USART2_printf("\r\n");
		delay_us(50);
UART_PutBytes(USART2 ,gpstest3,sizeof(gpstest3));
	USART2_printf("\r\n");
	
	//115200
	
		delay_us(50);
UART_PutBytes(USART2 ,gpstest1,sizeof(gpstest1));
		USART2_printf("\r\n");
		delay_us(50);
UART_PutBytes(USART2 ,gpstest1,sizeof(gpstest1));
		USART2_printf("\r\n");
	//RMC
		delay_us(50);
UART_PutBytes(USART2 ,gpstest2,sizeof(gpstest2));
		USART2_printf("\r\n");
		delay_us(50);
UART_PutBytes(USART2 ,gpstest2,sizeof(gpstest2));
	USART2_printf("\r\n");
	// deserve
		delay_us(50);
UART_PutBytes(USART2 ,gpstest4,sizeof(gpstest4));
	USART2_printf("\r\n");
		delay_us(50);
UART_PutBytes(USART2 ,gpstest10,sizeof(gpstest10));
	USART2_printf("\r\n");
	
		//RMC
		delay_us(50);
UART_PutBytes(USART2 ,gpstest5,sizeof(gpstest5));
		USART2_printf("\r\n");
		delay_us(50);
UART_PutBytes(USART2 ,gpstest6,sizeof(gpstest6));
	USART2_printf("\r\n");
	// deserve
		delay_us(50);
UART_PutBytes(USART2 ,gpstest7,sizeof(gpstest7));
	USART2_printf("\r\n");
		delay_us(50);
UART_PutBytes(USART2 ,gpstest9,sizeof(gpstest9));
	USART2_printf("\r\n");
	
		delay_us(50);
UART_PutBytes(USART2 ,gpstest11,sizeof(gpstest11));
		USART2_printf("\r\n");
		delay_us(50);
UART_PutBytes(USART2 ,gpstest11,sizeof(gpstest11));
	USART2_printf("\r\n");


}



u16 str2int(const char *str)
{
	u16 temp = 0;
	const char *ptr = str;
	if (*str == '-' || *str == '+')
		str++;
	while (*str != 0)
	{
		if ((*str) < '0' || (*str) > '9')
		break;
		temp = temp * 10 + (*str - '0');
		str++;
	}
	if (*ptr == '-')
		temp = -temp;
	return temp;

}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
//		USART3_printf("**************\r\n");
//	USART3_printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}
}

void errorLog(int num)
{
	
	while (1)
	{
	  	USART2_printf("ERROR%d\r\n",num);
	}
}
void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		USART2_printf("Save_Data.UTCTime = ");
		USART2_printf(Save_Data.UTCTime);
		USART2_printf("\r\n");

		if(Save_Data.isUsefull)
		{
		 gpssendflag=1;
			Save_Data.isUsefull = false;
					//	latitude
			USART2_printf("Save_Data.latitude = ");
			USART2_printf(Save_Data.latitude);
			USART2_printf("\r\n");

				 
	    gpsdest_string = strstr(Save_Data.latitude,".");
			strncpy( gpsdest,  gpsdest_string+1, 4);
	//	strncpy(dest, dest_string, 9);
			USART2_printf( gpsdest);
			lathalf1=atoi(gpsdest);
	gpsposition[0] = (lathalf1 >> 8)  & 0xff;
	gpsposition[1]  =  lathalf1       & 0xff;
			

			
			gpsp=strtok(Save_Data.latitude,  gpsdelim);
			USART2_printf( gpsp);
			lathalf2=atoi(gpsp);
		gpsposition[2] = (lathalf2 >> 8)  & 0xff;
		gpsposition[3]  =  lathalf2       & 0xff;
			
		//	longitude
			USART2_printf("Save_Data.longitude = ");
			USART2_printf(Save_Data.longitude);
			USART2_printf("\r\n");
			
			gpsdest_string = strstr(Save_Data.longitude,".");
			strncpy( gpsdest1,  gpsdest_string+1, 4);
	//	strncpy(dest, dest_string, 9);
			USART2_printf( gpsdest1);
			loghalf1=atoi(gpsdest1);
			
		gpsposition[4] = (loghalf1 >> 8)  & 0xff;
		gpsposition[5]  =  loghalf1       & 0xff;
			
 
			
			gpsp1=strtok(Save_Data.longitude,  gpsdelim);
			USART2_printf( gpsp1);
			loghalf2=atoi(gpsp1);
		gpsposition[6] = (loghalf2 >> 8)  & 0xff;
		gpsposition[7]  =  loghalf2       & 0xff;
			
		}
		else
		{
			USART2_printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}
