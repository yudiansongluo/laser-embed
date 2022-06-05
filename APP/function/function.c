#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "hwjs.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "nrf24l01.h"
#include "spi.h"
#include "gps.h"
#include "module.h"





void strHexToUint8Array(const char *str, uint8_t *bytes, size_t bytes_len) {
  uint8_t pos;
  uint8_t idx0;
  uint8_t idx1;

  // mapping of ASCII characters to hex values
  const uint8_t hashmap[] = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 01234567
      0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89:;<=>?
      0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // @ABCDEFG
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HIJKLMNO
  };

  memset(bytes, 0, bytes_len);
  for (pos = 0; ((pos < (bytes_len * 2)) && (pos < strlen(str))); pos += 2) {
    idx0 = ((uint8_t)str[pos + 0] & 0x1F) ^ 0x10;
    idx1 = ((uint8_t)str[pos + 1] & 0x1F) ^ 0x10;
    bytes[pos / 2] = (uint8_t)(hashmap[idx0] << 4) | hashmap[idx1];
  }
}

// at+recv=<RSSI>,<SNR>,<fport>:<dataHex>
void atRecvDecode( char const * recv_str,  uint8_t * decoded_bytes, int * fport){
    char * semicolon_ptr=strstr(recv_str, ":"); // cannot find semicolon, return 0x00
    int bytes_length=strlen(semicolon_ptr+1) / 2; // for non space hex
    char * last_comma_ptr=semicolon_ptr;
    int fport_digit_length=0;
    while (* last_comma_ptr!=','){
      last_comma_ptr--;
      fport_digit_length++;
    }
    fport_digit_length --;
    // char * fport_ascii = malloc(sizeof(char) * fport_digit_length); 
    char * fport_ascii = malloc(sizeof(char) * fport_digit_length); 
    memcpy(fport_ascii,last_comma_ptr+1,fport_digit_length);
    * fport = atoi(fport_ascii); 
    free(fport_ascii);
    strHexToUint8Array(semicolon_ptr+1,decoded_bytes,bytes_length);
}

int getAtRecvFport( char const * recv_str){
  char * semicolon_ptr=strstr(recv_str, ":"); // cannot find semicolon, return 0x00
  if (semicolon_ptr == 0x00){
    return 0x00;
  }
  char * last_comma_ptr=semicolon_ptr;
  int fport_digit_length=0;
  while (* last_comma_ptr!=','){
    last_comma_ptr--;
    fport_digit_length++;
  }
  fport_digit_length --;
  // char * fport_ascii = malloc(sizeof(char) * fport_digit_length); 
  char * fport_ascii = malloc(sizeof(char) * fport_digit_length); 
  memcpy(fport_ascii,last_comma_ptr+1,fport_digit_length);
  int fport = atoi(fport_ascii); 
  free(fport_ascii);
  return fport;
}

// at+recv=<RSSI>,<SNR>,<fport>:<dataHex>
// remember to free
uint8_t * getAtRecvBytes(char const * recv_str){
	char * at_prt = strstr(recv_str, "at");
	if (at_prt == 0x00){
    return NULL;
  }
  char * semicolon_ptr=strstr(recv_str, ":"); // cannot find semicolon, return 0x00
	if (semicolon_ptr == 0x00 || at_prt == 0x00){
    return NULL;
  }
	
  int bytes_length=strlen(semicolon_ptr+1) / 2; // for non space hex
  uint8_t * decoded_bytes=malloc(sizeof(char) * bytes_length);
  strHexToUint8Array(semicolon_ptr+1,decoded_bytes,bytes_length);
  return decoded_bytes;
}

// I hope I can use OOP
int getAtRecvBytesLength(char const * recv_str){
  char * semicolon_ptr=strstr(recv_str, ":"); // cannot find semicolon, return 0x00
  if (semicolon_ptr == 0x00){
    return 0x00;
  }
  int bytes_length=strlen(semicolon_ptr+1) / 2; // for non space hex
  return bytes_length;
}

int getDataSize(int bufferSize){
	/*
	98 01 02 03 04 05 06 07 08 09 8b 9e 01 17 37 78 56 24 00 00 00 00 00 00 
	24
	at+send=lora:1:980102030405060708098B9E011737785624000000000000
	61 ... 30 00 00 00 00 00 0d 0a 00 
	multiple 0x00 before 0d 0a (\r\n)
	*/
	return bufferSize*2+15+2+1;
}
void mergeData(char* merged, unsigned char* buffer, int bufferSize){
	const char prefix [] ="at+send=lora:1:";
	const int convertedSize = bufferSize*2+1;
	const int dataSize = getDataSize(bufferSize);
	/*
	use of vla (variable length array). not leagal
	use malloc array instead
	char converted[bufferSize*2 + 1];
	*/
	char* converted = (char*) malloc(sizeof(char)*convertedSize);
	converted[convertedSize-1]=0;
	/* equivalent using snprintf, notice len field keeps reducing
	with each pass, to prevent overruns
	snprintf(&converted[i*2], sizeof(converted)-(i*2),"%02X", buffer[i]);
	*/
	/*
	'for' loop initial declarations are only allowed in C99 or C11 mode
	*/
	for (int i=0;i<bufferSize;i++) {
		sprintf(&converted[i*2], "%02X", buffer[i]&0xff);
	}
	strcpy(merged, prefix);
	/*
	printf("%02X\r\n",buffer[0]&0xff);
	printf("%s\r\n",converted);
	*/
	strcat(merged, converted);
	free(converted);
	merged[dataSize-3]=13; 
	merged[dataSize-2]=10; 
	merged[dataSize-1]=0;
}
void printHex(unsigned char * str,int bufferSize){
	for (int i=0;i<bufferSize;i++) {
		printf("%02x ", str[i]&0xff);
	}
	printf("\r\n");
}

/**
 * remember to free the Hit_data data.
 */
char * newMergedData(unsigned char * buffer, int bufferSize){
	const int dataSize = getDataSize(bufferSize);
	char* merged = (char*) malloc(sizeof(char)*dataSize);
	mergeData(merged, buffer, bufferSize);
	return merged;
}

//big small consversion
 u8 reverse_bit8(u8 x)
{
	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
	return (x << 4) | (x >> 4);
}






unsigned char cal_table_high_first(unsigned char value)
{
    unsigned char i, crc;

    crc = value;
  
    for (i=8; i>0; --i)
    {
        if (crc & 0x80) 
        {
  
        // 0x31
            crc = (crc << 1) ^ 0x31;        }
        else
        {
  
            crc = (crc << 1);
        }
    }

    return crc;
}

