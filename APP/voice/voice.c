#include "stm32f10x.h"
#include "voice.h"
#include "SysTick.h"
/*
*********************************************************************************************************
*	�� �� ��: GPIO_Configuration
*	����˵��: ����PA9ΪTXD��PA10��RXD
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ�� GPIOA clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

	/* ��ֹ�ϵ������� */
	GPIO_SetBits(GPIOA, GPIO_Pin_5);

	/* ����PA9Ϊ������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����PA10Ϊ�������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}



void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

  	TIM_TimeBaseStructure.TIM_Period = 65535 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;	//������������Ϊ TIM3 ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//ʱ�ӷָ�Ϊ0
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM3 ���ϼ���ģʽ

  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3,ENABLE);    

}



/*
*********************************************************************************************************
*	�� �� ��: Delay_Ms
*	����˵��: ��ʱ
*	��    �Σ�nTime,��λΪuS
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Delay_Ms(volatile u32 nTime)
{ 

	u16 tmp;

	tmp = TIM_GetCounter(TIM3);		//��� TIM3 ��������ֵ
	
	if(tmp + nTime <= 65535)
		while( (TIM_GetCounter(TIM3) - tmp) < nTime );
	else
	{
		TIM_SetCounter(TIM3, 0);//���� TIM3 �������Ĵ���ֵΪ0
		while( TIM_GetCounter(TIM3) < nTime );
	}
			
}


/*
*********************************************************************************************************
*	�� �� ��: SendOneByte
*	����˵��: ģ�⴮�ڷ���һ�ֽ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SendOneByte(u8 datatoSend)
{
	u8 i, tmp;

	// ��ʼλ
	TXD_low();	//��TXD�����ŵĵ�ƽ�õ�
	Delay_Ms(SendingDelay);	

	for(i = 0; i < 8; i++)
	{
		tmp	= (datatoSend >> i) & 0x01;

		if(tmp == 0)
		{
			TXD_low();
			Delay_Ms(SendingDelay);	//0		
		}
		else
		{
			TXD_high();
			Delay_Ms(SendingDelay);	//1		
		}	
	}
	
	// ����λ
	TXD_high();//��TXD�����ŵĵ�ƽ�ø�
	Delay_Ms(SendingDelay);	
}

void USART_Send(u8 *buf, u8 len)
{
	u8 t;
	for(t = 0; t < len; t++)
	{
		SendOneByte(buf[t]);
	}
}


/**
*	@brief ������оƬ�������ݲ�������
*	@param[in] addr uint8_t
*	@return void
**/
void Senddata(u8 addr)
{
	u8 i;
	TXD_high();//����
	delay_ms(200);
	TXD_low();//����������
	delay_ms(2);
	for(i =0;i<8;i++)
	{
		TXD_high();
		if(addr&0x01)//1��1.9ms+10.7ms
		{
			delay_us(1900);
			TXD_low();
			delay_us(700);
		}
		else//0��0.7ms+1.9ms
		{
			delay_us(700);
			TXD_low();
			delay_us(1900);//1.9ms
		}
		addr>>=1;//�������ݼ�������
	}
	TXD_high();//����
	delay_ms(700);
}