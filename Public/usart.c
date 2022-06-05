#include "usart.h"
#include "stdarg.h"
#include "SysTick.h"

u16 point1 = 0;
char rxdatabufer;
_SaveData Save_Data;
int Flag, j;
char *gpsdelim = ".";
char gpsdest[10] = {""};
char gpsdest1[10] = {""};
char *gpsp;
char *gpsp1;
char *gpsdest_string;
u16 lathalf1 = 0;
u16 lathalf2 = 0;
u16 loghalf1 = 0;
u16 loghalf2 = 0;
u8 gpssendflag = 0;
u8 gpsposition[8] = {0};
uint8_t USART1_FINISH;
uint32_t Lora_data_len = 0;
uint8_t USART1_FINISH = 0;






void UART_PutChar(USART_TypeDef *USARTx, uint8_t Data) {
    USART_SendData(USARTx, Data);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) {}

}

void UART_PutStr(USART_TypeDef *USARTx, uint8_t *str) {
    while (0 != *str) {
        UART_PutChar(USARTx, *str);
        str++;
    }
}

void UART_PutBytes(USART_TypeDef *USARTx, uint8_t *str, uint8_t length) {
    int i = 0;
    for (i = 0; i < length; i++) {
        UART_PutChar(USARTx, *str);
        str++;
    }
}


void USART1_printf(char *fmt, ...) {
    char buffer[USART1_REC_LEN + 1];  // 数据长度
    u8 i = 0;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(buffer, USART1_REC_LEN + 1, fmt, arg_ptr);
    while ((i < USART1_REC_LEN) && (i < strlen(buffer))) {
        USART_SendData(USART1, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
    va_end(arg_ptr);
}


void USART2_printf(char *fmt, ...) {
    char buffer[USART_REC_LEN + 1];  // 数据长度
    u8 i = 0;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(buffer, USART_REC_LEN + 1, fmt, arg_ptr);
    while ((i < USART_REC_LEN) && (i < strlen(buffer))) {
        USART_SendData(USART2, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    va_end(arg_ptr);
}

void USART3_printf(char *fmt, ...) {
    char buffer[USART_REC_LEN3 + 1];  // 数据长度
    u8 i = 0;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(buffer, USART_REC_LEN3 + 1, fmt, arg_ptr);
    while ((i < USART_REC_LEN3) && (i < strlen(buffer))) {
        USART_SendData(USART3, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
    va_end(arg_ptr);
}

/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/
void USART1_Init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);     //打开时钟


    /*  配置GPIO的模式和IO口 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX			   //串口输出PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);  /* 初始化串口输入IO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RX			 //串口输入PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          //模拟输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); /* 初始化GPIO */


    //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口1

    USART_Cmd(USART1, ENABLE);  //使能串口1

    USART_ClearFlag(USART1, USART_FLAG_TC);

//	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);//开启相关中断

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器、
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
}

/*******************************************************************************
* 函 数 名         : USART1_IRQHandler
* 函数功能		   : USART1中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void USART1_IRQHandler(void) {                    //串口1中断服务程序
//	uint8_t ucTemp;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        //把接受的数据给ucTemp,再通过USART_SendData发送回去
        Lora_data[Lora_data_len++] = USART_ReceiveData(USART1);


    } else if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {

        USART1_FINISH = 1;
        USART1->SR;
        USART1->DR;

    }
}


#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
char USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0;       //接收状态标记

void USART2_Init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //使能USART1，GPIOA时钟
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 


    CLR_Buf();//清空缓存
}

void USART2_IRQHandler(void)                    //串口1中断服务程序
{
    u8 Res;
//#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
//	OSIntEnter();    
//#endif
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        Res = USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据

        if (Res == '$') {
            point1 = 0;
        }


        USART_RX_BUF[point1++] = Res;

        if (USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' &&
            USART_RX_BUF[5] == 'C')            //确定是否收到"GPRMC/GNRMC"这一帧数据
        {
            if (Res == '\n') {
                memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
                memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1);    //保存数据
                Save_Data.isGetData = true;
                point1 = 0;
                memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空
            }

        }

        if (point1 >= USART_REC_LEN) {
            point1 = USART_REC_LEN;
        }
    }
#ifdef OS_TICKS_PER_SEC        //如果时钟节拍数定义了,说明要使用ucosII了.
    OSIntExit();
#endif
}


u8 Hand(char *a)                   // 串口命令识别函数
{
    if (strstr(USART_RX_BUF, a) != NULL)
        return 1;
    else
        return 0;
}

void CLR_Buf(void)                           // 串口缓存清理
{
    memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空
    point1 = 0;
}

void clrStruct()//??GPS??????
{
    Save_Data.isGetData = false;
    Save_Data.isParseData = false;
    Save_Data.isUsefull = false;
    memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
    memset(Save_Data.UTCTime, 0, UTCTime_Length);
    memset(Save_Data.latitude, 0, latitude_Length);
    memset(Save_Data.N_S, 0, N_S_Length);
    memset(Save_Data.longitude, 0, longitude_Length);
    memset(Save_Data.E_W, 0, E_W_Length);

}

#endif


uint32_t Lc12s_len = 0;
uint8_t USART3_FINISH = 0;


u8 USART_RX_BUF3[USART_REC_LEN3];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA3 = 0;       //接收状态标记

/**
*	@brief 蜂鸟串口初始化
*	@param[in] bound u32
*	@return void
**/
void USART3_Init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


    /*  配置GPIO的模式和IO口 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//TX			   //串口输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);  /* 初始化串口输入IO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//RX			 //串口输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          //模拟输入
    GPIO_Init(GPIOB, &GPIO_InitStructure); /* 初始化GPIO */


    //USART3 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口3

    USART_Cmd(USART3, ENABLE);  //使能串口3

    USART_ClearFlag(USART3, USART_FLAG_TC);



    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器?
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
}



void USART3_IRQHandler(void) {                    //串口1中断服务程序
//	uint8_t ucTemp;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        //把接受的数据给ucTemp,再通过USART_SendData发送回去
        Lc12s_data[Lc12s_len] = USART_ReceiveData(USART3);
        Lc12s_len = Lc12s_len + 1;


    } else if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) {

        USART3_FINISH = 1;
        USART3->SR;
        USART3->DR;

    }
} 	







