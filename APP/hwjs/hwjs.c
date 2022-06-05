#include "hwjs.h"
#include "SysTick.h"


u8 ir_flag = 77;
u32 Ir_Decode_data = 0x00de21ee;      //定义一个32位数据变量，保存接收码
u8 IrReceiveFinish;  //定义一个8位数据的变量，用于指示接收标志
u8 irReceive_data[4]; //保存数据数组

u8 Lead_Low_Flag = 8;
u8 Ir_Pin_Rec = 8;

u8 Ir_Pin_Total = 7;

/**
*	@brief 红外接收引脚A15初始化，初始化10-15中断
*	@param[in] void
*	@return void
**/
void Ir_InitPinA15() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 开启GPIO时钟及管脚复用时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//红外接收
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15); //选择GPIO管脚用作外部中断线路
    EXTI_ClearITPendingBit(EXTI_Line15);

    /* 设置外部中断的模式 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 设置NVIC参数 */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;   //打开全局中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     //响应优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //使能
    NVIC_Init(&NVIC_InitStructure);

}


/**
*	@brief 红外接收引脚C13初始化，初始化9-15中断
*	@param[in] void
*	@return void
**/
void Ir_InitPinC13() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 开启GPIO时钟及管脚复用时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//红外接收
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); //选择GPIO管脚用作外部中断线路
    EXTI_ClearITPendingBit(GPIO_Pin_13);

    /* 设置外部中断的模式 */
    EXTI_InitStructure.EXTI_Line = GPIO_Pin_13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}


/**
*	@brief 红外接收引脚A12初始化，初始化9-15中断
*	@param[in] void
*	@return void
**/
void Ir_InitPinA12() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 开启GPIO时钟及管脚复用时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//红外接收
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12); //选择GPIO管脚用作外部中断线路
    EXTI_ClearITPendingBit(GPIO_Pin_12);

    /* 设置外部中断的模式 */
    EXTI_InitStructure.EXTI_Line = GPIO_Pin_12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}


/**
*	@brief 红外接收引脚b9初始化
*	@param[in] void
*	@return void
**/
void Ir_InitPinB9() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 开启GPIO时钟及管脚复用时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//红外接收
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9); //选择GPIO管脚用作外部中断线路
    EXTI_ClearITPendingBit(GPIO_Pin_9);

    /* 设置外部中断的模式 */
    EXTI_InitStructure.EXTI_Line = GPIO_Pin_9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}


/**
*	@brief 红外接收引脚A11初始化
*	@param[in] void
*	@return void
**/

void Ir_InitPinA11() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 开启GPIO时钟及管脚复用时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//红外接收
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); //选择GPIO管脚用作外部中断线路
    EXTI_ClearITPendingBit(GPIO_Pin_11);

    /* 设置外部中断的模式 */
    EXTI_InitStructure.EXTI_Line = GPIO_Pin_11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}


/**
*	@brief 红外接收引脚A7初始化
*	@param[in] void
*	@return void
**/

void Ir_InitPinA7() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 开启GPIO时钟及管脚复用时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//红外接收
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); //选择GPIO管脚用作外部中断线路
    EXTI_ClearITPendingBit(EXTI_Line7);

    /* 设置外部中断的模式 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 设置NVIC参数 */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   //打开全局中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     //响应优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //使能
    NVIC_Init(&NVIC_InitStructure);
}

/**
*	@brief 红外引脚14初始化
*	@param[in] void
*	@return void
**/
void Ir_InitPinB14() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 开启GPIO时钟及管脚复用时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//红外接收
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); //选择GPIO管脚用作外部中断线路
    EXTI_ClearITPendingBit(EXTI_Line14);

    /* 设置外部中断的模式 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


}


/**
*	@brief 震动引脚初始化
*	@param[in] void
*	@return void
**/

void Shake_InitB0() {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 开启GPIO时钟及管脚复用时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//红外接收
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //设置推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //设置传输速率
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
*	@brief 高电平持续时间，将记录的时间保存在t中返回，其中一次大约20us 
*	@param[in] void
*	@return u8 t
**/

u8 Ir_time(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin) {
    u8 t = 0;
    while (GPIO_ReadInputDataBit(GPIO, GPIO_Pin) == 1)//高电平
    {
        t++;
        delay_us(20);
        if (t >= 250) return t;//超时溢出
    }
    return t;
}


/**
*	@brief 用引导码判断那一路红外被击中 
*	@param[in] void
*	@return u8 t
**/

void
Lead_Low_Time()//判断中断在那个脚，或许可以用这个程序：https://blog.csdn.net/weibo1230123/article/details/80798200?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164630881416780261986784%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=164630881416780261986784&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_ulrmf~default~default-5-80798200.pc_search_insert_ulrmf&utm_term=stm32+exti&spm=1018.2226.3001.4187
{
    u8 i, Pin_state;
    u16 Ir_Pin_time[6] = {3};
    TIM_SetCounter(TIM4, 0);
    while (PAin(11) == 0 || PBin(9) == 0 || PAin(12) == 0 || PCin(13) == 0 || PAin(15) == 0 || PAin(7) == 0 ||
           PBin(14) == 0) {
        for (i = 0; i < Ir_Pin_Total; i++) {
            switch (i) {
                case 0:
                    Pin_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
                    break;
                case 1:
                    Pin_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11);
                    break;
                case 2:
                    Pin_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12);
                    break;
                case 3:
                    Pin_state = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
                    break;
                case 4:
                    Pin_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);
                    break;
                case 5:
                    Pin_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
                    break;
                case 6:
                    Pin_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
                    break;
            }

            if (Pin_state == 0) {
                Ir_Pin_time[i] = TIM_GetCounter(TIM4);
                if ((Ir_Pin_time[i] > 1700) && (Ir_Pin_time[i] < 1900))            //判断低电平脉宽是否在8.5ms-9.5ms之间
                {

                    Lead_Low_Flag = 1;
                    Ir_Pin_Rec = i;  //将为零的口取出来
                    break;

                }
            }
        }
    }
}


/**
*	@brief 对红外进行组别映射
*	@param[in] Ir_Pin_Rec u8 , Ir_Pin_Total u8
*	@return u8 t
**/

void Ir_Pin_Sw(u8 Ir_Pin_Rec, u8 Ir_Pin_Total) {
    u32 Exit_Gp[Ir_Pin_Total];
    u32 Exit_Io_Gp[Ir_Pin_Total];
    GPIO_TypeDef *exit_AB[Ir_Pin_Total];
    switch (Ir_Pin_Rec) {
        case 0:
            Exit_Gp[0] = EXTI_Line9;
            Exit_Io_Gp[0] = GPIO_Pin_9;
            exit_AB[0] = GPIOB;
            break;
        case 1:
            Exit_Gp[1] = EXTI_Line11;
            Exit_Io_Gp[1] = GPIO_Pin_11;
            exit_AB[1] = GPIOA;
            break;
        case 2:
            Exit_Gp[2] = EXTI_Line12;
            Exit_Io_Gp[2] = GPIO_Pin_12;
            exit_AB[2] = GPIOA;
            break;
        case 3:
            Exit_Gp[3] = EXTI_Line13;
            Exit_Io_Gp[3] = GPIO_Pin_13;
            exit_AB[3] = GPIOC;
            break;
        case 4:
            Exit_Gp[4] = EXTI_Line15;
            Exit_Io_Gp[4] = GPIO_Pin_15;
            exit_AB[4] = GPIOA;
            break;
        case 5:
            Exit_Gp[5] = EXTI_Line7;
            Exit_Io_Gp[5] = GPIO_Pin_7;
            exit_AB[5] = GPIOA;
            break;
        case 6:
            Exit_Gp[6] = EXTI_Line14;
            Exit_Io_Gp[6] = GPIO_Pin_14;
            exit_AB[6] = GPIOB;
            break;
    }//再次对接收到的引脚进行映射

    u8 Bit_Time = 0, Ok = 0, Data, Num = 0;
    while (EXTI_GetITStatus(Exit_Gp[Ir_Pin_Rec]) != RESET) {
        if (GPIO_ReadInputDataBit(exit_AB[Ir_Pin_Rec], Exit_Io_Gp[Ir_Pin_Rec]) == 1) {
            Bit_Time = Ir_time(exit_AB[Ir_Pin_Rec], Exit_Io_Gp[Ir_Pin_Rec]);//获得此次高电平时间
            if (Bit_Time >= 250) break;//不是有用的信号
            if ((Bit_Time >= 200 && Bit_Time < 250)) {
                Ok = 1;//收到起始信号
            } else if (Bit_Time >= 60 && Bit_Time < 95)        //60~90   73~95
            {
                Data = 1;//收到数据 1
            } else if (Bit_Time >= 10 && Bit_Time < 39)  //10~50  17~39
            {
                Data = 0;//收到数据 0
            } else break;

            if (Ok == 1) {
                Ir_Decode_data <<= 1;
                Ir_Decode_data += Data;
                if (Num >= 32) {
                    IrReceiveFinish = 1;
                    break;
                }
            }

            Num++;

        }
    }
}


/**
*	@brief 红外中断服务函数，引脚15-10
*	@param[in] void
*	@return u8 t
**/
void EXTI15_10_IRQHandler(void)      //红外遥控外部中断
{
    Lead_Low_Time();
    if (Lead_Low_Flag == 1) {
        ir_flag = Ir_Pin_Rec;
        Lead_Low_Flag = 0;//标志位清零
        Ir_Pin_Sw(Ir_Pin_Rec, Ir_Pin_Total-1);
    }
    EXTI_ClearITPendingBit(EXTI_Line14);
    EXTI_ClearITPendingBit(EXTI_Line15);
    EXTI_ClearITPendingBit(EXTI_Line13);
    EXTI_ClearITPendingBit(EXTI_Line12);
    EXTI_ClearITPendingBit(EXTI_Line11);

}


/**
*	@brief 红外中断服务函数，引脚9-5
*	@param[in] void
*	@return u8 t
**/
void EXTI9_5_IRQHandler(void)      //红外遥控外部中断
{
    Lead_Low_Time();
    if (Lead_Low_Flag == 1) {
        ir_flag = Ir_Pin_Rec;
        Lead_Low_Flag = 0;
        Ir_Pin_Sw(Ir_Pin_Rec,Ir_Pin_Total-1);
    }
    EXTI_ClearITPendingBit(EXTI_Line7);
    EXTI_ClearITPendingBit(EXTI_Line9);
}





































