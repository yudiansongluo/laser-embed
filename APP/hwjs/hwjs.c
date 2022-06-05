#include "hwjs.h"
#include "SysTick.h"


u8 ir_flag = 77;
u32 Ir_Decode_data = 0x00de21ee;      //����һ��32λ���ݱ��������������
u8 IrReceiveFinish;  //����һ��8λ���ݵı���������ָʾ���ձ�־
u8 irReceive_data[4]; //������������

u8 Lead_Low_Flag = 8;
u8 Ir_Pin_Rec = 8;

u8 Ir_Pin_Total = 7;

/**
*	@brief �����������A15��ʼ������ʼ��10-15�ж�
*	@param[in] void
*	@return void
**/
void Ir_InitPinA15() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15); //ѡ��GPIO�ܽ������ⲿ�ж���·
    EXTI_ClearITPendingBit(EXTI_Line15);

    /* �����ⲿ�жϵ�ģʽ */
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* ����NVIC���� */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;   //��ȫ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     //��Ӧ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //ʹ��
    NVIC_Init(&NVIC_InitStructure);

}


/**
*	@brief �����������C13��ʼ������ʼ��9-15�ж�
*	@param[in] void
*	@return void
**/
void Ir_InitPinC13() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); //ѡ��GPIO�ܽ������ⲿ�ж���·
    EXTI_ClearITPendingBit(GPIO_Pin_13);

    /* �����ⲿ�жϵ�ģʽ */
    EXTI_InitStructure.EXTI_Line = GPIO_Pin_13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}


/**
*	@brief �����������A12��ʼ������ʼ��9-15�ж�
*	@param[in] void
*	@return void
**/
void Ir_InitPinA12() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12); //ѡ��GPIO�ܽ������ⲿ�ж���·
    EXTI_ClearITPendingBit(GPIO_Pin_12);

    /* �����ⲿ�жϵ�ģʽ */
    EXTI_InitStructure.EXTI_Line = GPIO_Pin_12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}


/**
*	@brief �����������b9��ʼ��
*	@param[in] void
*	@return void
**/
void Ir_InitPinB9() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9); //ѡ��GPIO�ܽ������ⲿ�ж���·
    EXTI_ClearITPendingBit(GPIO_Pin_9);

    /* �����ⲿ�жϵ�ģʽ */
    EXTI_InitStructure.EXTI_Line = GPIO_Pin_9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}


/**
*	@brief �����������A11��ʼ��
*	@param[in] void
*	@return void
**/

void Ir_InitPinA11() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); //ѡ��GPIO�ܽ������ⲿ�ж���·
    EXTI_ClearITPendingBit(GPIO_Pin_11);

    /* �����ⲿ�жϵ�ģʽ */
    EXTI_InitStructure.EXTI_Line = GPIO_Pin_11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}


/**
*	@brief �����������A7��ʼ��
*	@param[in] void
*	@return void
**/

void Ir_InitPinA7() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); //ѡ��GPIO�ܽ������ⲿ�ж���·
    EXTI_ClearITPendingBit(EXTI_Line7);

    /* �����ⲿ�жϵ�ģʽ */
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* ����NVIC���� */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   //��ȫ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     //��Ӧ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //ʹ��
    NVIC_Init(&NVIC_InitStructure);
}

/**
*	@brief ��������14��ʼ��
*	@param[in] void
*	@return void
**/
void Ir_InitPinB14() {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); //ѡ��GPIO�ܽ������ⲿ�ж���·
    EXTI_ClearITPendingBit(EXTI_Line14);

    /* �����ⲿ�жϵ�ģʽ */
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


}


/**
*	@brief �����ų�ʼ��
*	@param[in] void
*	@return void
**/

void Shake_InitB0() {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�����������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //���ô�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
*	@brief �ߵ�ƽ����ʱ�䣬����¼��ʱ�䱣����t�з��أ�����һ�δ�Լ20us 
*	@param[in] void
*	@return u8 t
**/

u8 Ir_time(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin) {
    u8 t = 0;
    while (GPIO_ReadInputDataBit(GPIO, GPIO_Pin) == 1)//�ߵ�ƽ
    {
        t++;
        delay_us(20);
        if (t >= 250) return t;//��ʱ���
    }
    return t;
}


/**
*	@brief ���������ж���һ·���ⱻ���� 
*	@param[in] void
*	@return u8 t
**/

void
Lead_Low_Time()//�ж��ж����Ǹ��ţ�����������������https://blog.csdn.net/weibo1230123/article/details/80798200?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164630881416780261986784%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=164630881416780261986784&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_ulrmf~default~default-5-80798200.pc_search_insert_ulrmf&utm_term=stm32+exti&spm=1018.2226.3001.4187
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
                if ((Ir_Pin_time[i] > 1700) && (Ir_Pin_time[i] < 1900))            //�жϵ͵�ƽ�����Ƿ���8.5ms-9.5ms֮��
                {

                    Lead_Low_Flag = 1;
                    Ir_Pin_Rec = i;  //��Ϊ��Ŀ�ȡ����
                    break;

                }
            }
        }
    }
}


/**
*	@brief �Ժ���������ӳ��
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
    }//�ٴζԽ��յ������Ž���ӳ��

    u8 Bit_Time = 0, Ok = 0, Data, Num = 0;
    while (EXTI_GetITStatus(Exit_Gp[Ir_Pin_Rec]) != RESET) {
        if (GPIO_ReadInputDataBit(exit_AB[Ir_Pin_Rec], Exit_Io_Gp[Ir_Pin_Rec]) == 1) {
            Bit_Time = Ir_time(exit_AB[Ir_Pin_Rec], Exit_Io_Gp[Ir_Pin_Rec]);//��ô˴θߵ�ƽʱ��
            if (Bit_Time >= 250) break;//�������õ��ź�
            if ((Bit_Time >= 200 && Bit_Time < 250)) {
                Ok = 1;//�յ���ʼ�ź�
            } else if (Bit_Time >= 60 && Bit_Time < 95)        //60~90   73~95
            {
                Data = 1;//�յ����� 1
            } else if (Bit_Time >= 10 && Bit_Time < 39)  //10~50  17~39
            {
                Data = 0;//�յ����� 0
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
*	@brief �����жϷ�����������15-10
*	@param[in] void
*	@return u8 t
**/
void EXTI15_10_IRQHandler(void)      //����ң���ⲿ�ж�
{
    Lead_Low_Time();
    if (Lead_Low_Flag == 1) {
        ir_flag = Ir_Pin_Rec;
        Lead_Low_Flag = 0;//��־λ����
        Ir_Pin_Sw(Ir_Pin_Rec, Ir_Pin_Total-1);
    }
    EXTI_ClearITPendingBit(EXTI_Line14);
    EXTI_ClearITPendingBit(EXTI_Line15);
    EXTI_ClearITPendingBit(EXTI_Line13);
    EXTI_ClearITPendingBit(EXTI_Line12);
    EXTI_ClearITPendingBit(EXTI_Line11);

}


/**
*	@brief �����жϷ�����������9-5
*	@param[in] void
*	@return u8 t
**/
void EXTI9_5_IRQHandler(void)      //����ң���ⲿ�ж�
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





































