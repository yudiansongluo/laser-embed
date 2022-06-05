
#ifndef __voice_H
#define __voice_H
#include "stm32f10x.h"


#define TXD_high() GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define TXD_low() GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define BaudRateUsed 9600
#define SendingDelay 104

#define ready 0x01
#define head 0x0e
#define front 0x0f
#define fubu 0x10
#define back 0x11
#define l_arm 0x12
#define r_arm 0x13
#define l_leg 0x14
#define r_leg 0x15
#define zhongdan 0x16
#define zhengwang 0x17
#define zhongshang 0x18
#define fa   0x19
#define zhuangda   0x1a
#define qingchongdan   0x1b
#define qingchongdian   0x1c

extern u32 TimingDelay;
extern u8 dataReceived[100];
extern u8 receivedFlag, SendingFlag;
extern u8 receivedNum, tmpreceivedNum;
extern u8 cnt;


extern void GPIO_Configuration(void);
extern void EXTI_Configuration(void);
extern void TIM1_Configuration(void);
extern void TIM2_Configuration(void);
extern void TIM3_Configuration(void);
void USART_Send(u8 *buf, u8 len);

extern void Delay_Ms(volatile u32 nTime);
extern void SendOneByte(u8 datatoSend);
extern void Senddata(u8 addr);
#endif





