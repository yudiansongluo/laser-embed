#ifndef __usart_H
#define __usart_H


#include "system.h" 
#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"	
#define USART1_REC_LEN  			200  	//定义USART1最大接收字节数
#define USART2_REC_LEN  			200  	//定义最大接收字节数 200

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern char  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
#define false 0
#define true 1


#define USART3_PRINTF

#define USART_REC_LEN3  			200  	//定义最大接收字节数 200

	  	
extern u8  USART_RX_BUF3[USART_REC_LEN3]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA3;         		//接收状态标记	


void USART3_Init(u32 bound);

extern u8 gpssendflag;
extern	u8	gpsposition[8];
extern	char *gpsdelim;
extern	char gpsdest[10];
extern		char gpsdest1[10];
extern	char*gpsp;
extern		char*gpsp1;
extern		char * gpsdest_string;
extern	u16 lathalf1;
extern	u16 lathalf2;
extern	u16 loghalf1;
extern	u16 loghalf2;
extern uint32_t Lora_data_len;
extern uint32_t Lc12s_len;
extern char Lora_data[100];
extern char Lc12s_data[100];
//void parseGpsBuffer(void);
//void printGpsBuffer(void);

//定义数组长度
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2
typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//定位信息是否有效
} _SaveData;


	void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data);
void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str);
void UART_PutBytes (USART_TypeDef* USARTx, uint8_t *str, uint8_t length);


void USART2_printf (char *fmt, ...);
void USART1_printf (char *fmt, ...);
 void USART3_printf (char *fmt, ...);
void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
extern char rxdatabufer;
extern u16 point1;
extern _SaveData Save_Data;
extern uint8_t USART1_FINISH;
extern uint8_t USART3_FINISH;
void CLR_Buf(void);
u8 Hand(char *a);
void clrStruct(void);



#endif


