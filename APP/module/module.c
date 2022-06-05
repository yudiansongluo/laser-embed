#include "function.h"
#include "string.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "SysTick.h"
#include "usart.h"
#include "module.h"
#include "hwjs.h"
#include "gps.h"
#include "voice.h"

u8 order[] = {0xaa, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x12, 0x00,
              0x24};
u8 Lora_join_flag = 1;
u8 Lora_Loading_flag = 1;
u8 Lc12s_flag = 1;
char Lora_data[] = "";
char Lc12s_data[] = "";
char *dest_string = "";
char *dest_string1 = "";
char *p;
u8 i, s;
int fport, bytes_length;
u8 lorasendflag = 0;
u8 data = 0;
u8 y = 0;
u8 port = 0;
u8 USART1_RX_BUF[USART1_REC_LEN];
u8 test = 2;
u8 count = 0;
u8 beat_count = 0;
uint32_t Lora_buffer_index = 0;
u8 Lc12s_net_data[] = {22, 61, 97, 78, 78, ID_Num1, ID_Num2};
u8 flagdata[] = {0x58, 0x63};
char *delim = ",";
char dest[10] = {""};
char dest1[10] = {""};
uint8_t converted[8];
u16 color = 0;

char str[20] = "";
char str1[20] = "";
char str2[20] = "";
char str3[20] = "";
char str0[20] = "";
char str4[100] = "";
char str5[100] = "";
u8 a[40] = {0};
char *ret;
char *Hit_data;
char *Ping_data;


//����
char joinCmd[] = "at+join\r\n";
//������Ϣ
char testCmd[] = "at+send=lora:1:5A00\r\n";

char atCmd[] = "at+set_config=lora:confirm:1\r\n";
//	char position []	= "at+send=lora:1:008000002e3e04d109980d1201c88a\r\n";
//ʿ������
char position[] = "at+send=lora:1:008000002e3e04d109980d12007b8a\r\n";

char FeedbackOrder[] = "OK Join Success\r\n";
//char FeedbackOrder1[]="OK \r\nat";
char FeedbackOrder1[] = "OK";
const int bufferSize1 = 15;
const int bufferSize2 = 6;
//������Ϣ1,ping��Ϣ
unsigned char Ping_Message[] = {0x00, 0x80, 0x00, 0x00, 0x2e, 0x3e, 0x04, 0xd1, 0x09, 0x99, 0x0d, 0x11, ID_Num1, ID_Num2,0x8A};
//������Ϣ2��������Ϣ
unsigned char Hit_Message[] = {0x40, 0x00, 0x00, ID_Num1, ID_Num2, 0x5b};

/**
*	@brief lora��ʱʱ�䣬����������ӳٴ�������λΪs
*	@param[in] u8
*	@return void
**/

void Join_delay(u8 freq) {
    for (int i = 0; i < freq; i++) {
        delay_ms(1000);
    }
}

/**
*	@brief ���ڻ�����գ�ȷ��ÿ�ν��ܵ�������Ϣ
*	@param[in] buffer,buffer_index,buffer_len
*	@return void
**/

void Uart_clr(char buffer[], uint32_t buffer_len) {
    for (int buffer_index = 0; buffer_index < buffer_len; buffer_index++) {
        buffer[buffer_index] = 0x00;
    }
    buffer_len = 0;
}

//lora������
void Join_Net() {
    USART_ClearFlag(USART1, USART_FLAG_TC);
    //������
    USART1_printf("%s", joinCmd);
    Join_delay(7);
    //�����յ����ط�����
    while (Lora_join_flag == 1) {
        if (USART1_FINISH) {
            USART3_printf("%s", joinCmd);
            //�����ɹ�
            if (strncmp(Lora_data, FeedbackOrder, 2) == 0) {
                Lora_join_flag = 0;
                //��ջ���
                Uart_clr(Lora_data,  Lora_data_len);
            } else {
                Uart_clr(Lora_data,  Lora_data_len);
            }
            USART1_FINISH = 0;
        } else {
            Uart_clr(Lora_data,  Lora_data_len);
            USART1_FINISH = 0;
            USART1_printf("%s", joinCmd);
            Join_delay(7);
        }
    }


    //ʿ������;
    USART1_printf("%s", position);
    Join_delay(7);
    USART3_printf("%d", position);

    while (Lora_Loading_flag == 1) {
        if (USART1_FINISH) {
            if (strncmp(Lora_data, FeedbackOrder1, 2) == 0) {
                Lora_Loading_flag = 0;
                Uart_clr(Lora_data,  Lora_data_len);
            } else {
                Join_delay(7);
                Uart_clr(Lora_data,  Lora_data_len);
                USART1_FINISH = 0;
            }
        }
            //����û�յ�����֮ǰ����������ʿ��
        else {
            USART1_printf("%s", position);
            Join_delay(7);
        }
        USART1_FINISH = 0;
    }
    USART3_printf("%s", position);
}

//����1�յ����ص����ݣ����д���
void Usart_Receive() {
    if (USART1_FINISH) {
        USART1_FINISH = 0;
        //�յ������ݰ����� ����
        if (strstr(Lora_data, ":")) {
            lorasendflag = 1;
            dest_string = strstr(Lora_data, ":");
            strncpy(dest, dest_string + 1, 6);//�ָ�
            //USART1_printf("333 : %s\r\n",dest);
            bytes_length = strlen(dest) / 2;
            strHexToUint8Array(dest, converted, bytes_length);
            //USART1_printf("converted:%d\r\n",converted[0]);
        }
        //�յ������ݰ����� at��
        if (strstr(Lora_data, "at")) {
            dest_string1 = strstr(Lora_data, "=");
            p = strtok(dest_string1, delim);
            //	printf("Lora_data : %s\r\n",p);
            strncpy(dest1, p + 1, 16);
            fport = atoi(dest1);    //�ַ���ת��������ȡ�˿ںţ�
            //USART1_printf("fport:%d\r\n",fport);
            //printf("222 : %s\r\n",dest1);
        }
        for (Lora_buffer_index = 0; Lora_buffer_index < Lora_data_len; Lora_buffer_index++) {
            Lora_data[Lora_buffer_index] = 0x00;
        }
        Lora_data_len = 0;
    }
}

/**
*	@brief �𶯺���
*	@param[in] void
*	@return void
**/

void Shaking() {
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
    delay_ms(1000);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

/**
*	@brief ���в���
*	@param[in] body
*	@return void
**/

void Voice(u8 body)
{
    Senddata( body);
    delay_ms(1000);
    Senddata(zhongdan);
}

/**
*	@brief ���������ѯ���ϴ������û���Ϣ������
*	@param[in] void
*	@return void
**/
//���ױ�������Ϣ����
void IR_Poll() {
//	int w =0;
//	USART_Send(voice5,sizeof(voice5));
    //���б�־
    if (IrReceiveFinish == 1) {
        IrReceiveFinish = 0;

        //�ָ��4���ֽ�
        irReceive_data[0] = (Ir_Decode_data >> 24) & 0xff;
        irReceive_data[1] = (Ir_Decode_data >> 16) & 0xff;
        irReceive_data[2] = (Ir_Decode_data >> 8) & 0xff;
        irReceive_data[3] = Ir_Decode_data & 0xff;
        Ir_Decode_data = 0;
        //��С�˱仯
        irReceive_data[0] = reverse_bit8(irReceive_data[0]);
        irReceive_data[1] = reverse_bit8(irReceive_data[1]);
        irReceive_data[2] = reverse_bit8(irReceive_data[2]);
        irReceive_data[3] = reverse_bit8(irReceive_data[3]);
        //��Ϣ����������Ϣ��
        Hit_Message[1] = irReceive_data[0];//0x01
        Hit_Message[2] = irReceive_data[2];//0xc8

        //Hit_Message[3]=irReceive_data[2];//00
        //Hit_Message[4]=irReceive_data[3];//123
        //	IrRecSendflag=ir_flag;
        //	Voiceflag=1;

        // ���в�λ��ʾforward 80; back  40;	r_leg 20;l_leg 10; right 08; left 04;
        // 5�Ǳ��� 4��ǰ��.3�����֣�1�����֣�2�����ȣ�7������,6�Ǹ���,
//	NRF24L01_TX_Mode();
        enum PART {
            L_ARM = 1,
            R_ARM = 3,
            L_LEG = 2,
            R_LEG = 7,
            STOMACH = 6,
            FRONT = 4,
            BACK = 5,
        };
        enum PART_PROTOCOL {
            P_L_HEAD = 0b00000001,
            P_STOMACH = 0b00000010,
            P_L_ARM = 0b00000100,
            P_R_ARM = 0b00001000,
            P_L_LEG = 0b00010000,
            P_R_LEG = 0b00100000,
            P_FRONT = 0b01000000,
            P_BACK = 0b10000000
        };
        Ping_Message[3]++;
        Shaking();
        switch (ir_flag) {
            case FRONT:
                Hit_Message[0] = P_FRONT;
                Voice(front);
                break;
            case R_ARM:
                Hit_Message[0] = P_R_ARM;
                Voice(r_arm);
                break;
            case L_LEG:
                Hit_Message[0] = P_L_LEG;
                Voice(l_leg);
                break;
            case R_LEG:
                Hit_Message[0] = P_R_LEG;
                Voice(r_leg);
                break;
            case L_ARM:
                Hit_Message[0] = P_L_ARM;
                Voice(l_arm);
                break;
            case BACK:
                Hit_Message[0] = P_BACK;
                Voice(back);
                break;
            case STOMACH:
                Hit_Message[0] = P_STOMACH;//������־
                Voice(fubu);
                break;
        }
        //����������Ϣ2
        Hit_data = newMergedData(Hit_Message, bufferSize2);
        USART1_printf("%s", Hit_data);
        ir_flag = 77;
        free(Hit_data);//�ڴ漰ʱ�ͷţ�

    }
}

/**
*	@brief GPS��ѯ��������γ����Ϣ
*	@param[in] void
*	@return void
**/
//gps��Ϣ����
void Gps_RecToSend() {
    // GPS��Ч���±�־
    if (gpssendflag == 1) {
        gpssendflag = 0;

        //longtidu����
        Ping_Message[4] = gpsposition[6];
        Ping_Message[5] = gpsposition[7];
        Ping_Message[6] = gpsposition[4];
        Ping_Message[7] = gpsposition[5];

        //latitudeγ��
        Ping_Message[8] = gpsposition[2];
        Ping_Message[9] = gpsposition[3];
        Ping_Message[10] = gpsposition[0];
        Ping_Message[11] = gpsposition[1];

        Hit_data = newMergedData(Ping_Message, bufferSize1);
        USART1_printf("%s", Hit_data);
//	USART3_printf("%s",Hit_data);
        free(Hit_data);
    }
}

void Lc12s_Init() {
    for (int i = 0; i < 17; i++) {
        UART_PutChar(USART3, order[i]);
        delay_ms(3);
    }
}


//�����֮����շ�
void Usart3_Receive() {
    //lora recevive
    if (USART3_FINISH) {
        USART3_FINISH = 0;


//		char buf[8];
        //ȥ�����ź���λ
        if (Lc12s_data[0] == 0x16 && Lc12s_data[7] == 0x4d) {
            Ping_Message[2]++;
            Hit_Message[0] = 0x40;
            Hit_Message[1] = Lc12s_data[1];
            Hit_Message[2] = Lc12s_data[3];
            Senddata(zhengwang);
            Hit_data = newMergedData(Hit_Message, bufferSize2);
            USART1_printf("%s", Hit_data);
            //��������
            UART_PutChar(USART3, flagdata[1]);
            free(Hit_data);
            delay_ms(100);
        }

        //�����ж�2�����յ�ǹ��������Ϣ
        if (Lc12s_data[0] == 0x0b && Lc12s_data[7] == 0x4d) {

            Ping_Message[1] = Lc12s_data[0];
            Ping_data = newMergedData(Ping_Message, bufferSize1);
            USART1_printf("%s", Ping_data);
            free(Ping_data);
            //��������
            UART_PutChar(USART3, flagdata[0]);
            delay_ms(100);
        }


    }
}


void Lc12s(u8 data[])
//����Ҫ����һ����ʱ��ʱ������ʱ�ͷ����˴η��ͣ���һ��ʱ���ڽ���,��δ����
{
    USART_ClearFlag(USART3, USART_FLAG_TC);
    //�������ݰ�
    for (int i = 0; i < 8; i++) {
        UART_PutChar(USART3, data[i]);
    }
    delay_ms(1000);
    //�����յ�����
    while (Lc12s_flag == 1) {
        if (USART3_FINISH) {
            USART3_FINISH = 0;
            //�ж϶Է��Ƿ���յ���������յ��Ͳ��ڷ���
            if (Lc12s_data[0] == 0x58 || Lc12s_data[0] == 0x63)//88Ϊǹ��99Ϊͷ��
            {
                Lc12s_flag = 0; //���ͱ�־λ����
                Uart_clr(Lc12s_data,Lc12s_len);
            }
            else {
                for (int i = 0; i < 8; i++) {
                    UART_PutChar(USART3, data[i]);
                    delay_ms(3);
                }
                delay_ms(1000);
                Uart_clr(Lc12s_data,Lc12s_len);
            }
        } else {
            Uart_clr(Lc12s_data,Lc12s_len);
            //δ�յ��ظ�������Э��
            for (int i = 0; i < 8; i++) {
                UART_PutChar(USART3, data[i]);
                delay_ms(3);
            }
            delay_ms(1000);
            USART1_FINISH = 0;
        }
    }
}

void USART3_Send() {

    if (lorasendflag == 1) {
        //22 means send to head;
        //11 means send to gun;
        //78 means people number
        //send to head
        if (fport == 3) {
            if(converted[0] == 0x00)
            {
                Senddata(ready);
            }
            if(converted[0] == 0x01)
            {
                Senddata(zhengwang);
            }
        }

        //send to gun
        if (fport == 5) {
            Lc12s_net_data[0] = 11;
            Lc12s_net_data[1] = converted[0];
            Lc12s(Lc12s_net_data);
        }
        lorasendflag = 0;
        Lc12s_net_data[1] = 0;
        Lc12s_net_data[2] = 0;
    }
}


//gpsԭʼ���ݴ���
void GPS_LogIn() {
    parseGpsBuffer();
    printGpsBuffer();
}



