#ifndef __usart_H
#define __usart_H


#include "system.h" 
#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"	
#define USART1_REC_LEN  			200  	//����USART1�������ֽ���
#define USART2_REC_LEN  			200  	//�����������ֽ��� 200

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern char  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
#define false 0
#define true 1


#define USART3_PRINTF

#define USART_REC_LEN3  			200  	//�����������ֽ��� 200

	  	
extern u8  USART_RX_BUF3[USART_REC_LEN3]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA3;         		//����״̬���	


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

//�������鳤��
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2
typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
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


