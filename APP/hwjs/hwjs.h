#ifndef _hwjs_H
#define _hwjs_H

#include "system.h"


void Ir_InitPinA15(void);

void Ir_InitPinC13(void);

void Ir_InitPinB9(void);

void Ir_InitPinA12(void);

void Ir_InitPinA11(void);

void Ir_InitPinB14(void);

void Ir_InitPinA7(void);

void Shake_InitB0();

void Lead_Low_Time(void);


void Ir_Pin_Sw(u8 Ir_Pin_Rec,u8 Ir_Pin_Total);

u8 Ir_time(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin);


void IR_Poll(void);


//定义全局变量
extern u32 Ir_Decode_data;
extern u8 IrReceiveFinish;
extern u8 ir_flag;
extern u8 irReceive_data[4];


#endif
