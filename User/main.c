#include "SysTick.h"
#include "usart.h"
#include "picture.h"
#include "hwjs.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "gps.h"
#include "module.h"
#include "function.h"
#include "time.h"
#include "voice.h"   //虚拟串口


int main() {
    //时钟系统与串口初始化
    SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
    USART1_Init(115200);
    USART3_Init(9600);

    //各路红外初始化
    Ir_InitPinA7();
    Ir_InitPinA15();
    Ir_InitPinC13();
    Ir_InitPinB9();
    Ir_InitPinA12();
    Ir_InitPinA11();
    Ir_InitPinB14();

    Shake_InitB0;

    /* 语音模块IO初始化 */
    GPIO_Configuration();


    //gps初始化，将模块波特率改为115200
    delay_us(50);
    USART2_Init(9600);
    clrStruct();
    USART2_printf("$CCCAS,1,5*55\r\n");
    delay_us(50);
    USART2_printf("$CCCAS,1,5*55\r\n");
    delay_us(50);
    USART2_Init(115200);     //串口初始化为115200
    delay_us(50);



    //gps参数设置
    gpsinit();

    //Lora 入网初始化，模块上线
    Join_Net();
    Senddata(ready);

    //上线提示音
    while (1) {
        //收到网关下行信息解析；
        Usart_Receive();

        //GPS更新信息；
        Gps_RecToSend();

        //无线模块接收信息；
        Usart3_Receive();

        //无线模块发送网关信息；
        USART3_Send();

        //红外击中处理；
        IR_Poll();

        //轮询GPS；
        parseGpsBuffer();
        printGpsBuffer();

    }
}