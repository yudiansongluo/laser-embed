#ifndef _function_H
#define _function_H
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"



void strHexToUint8Array(const char *str, uint8_t *bytes, size_t bytes_len) ;
void atRecvDecode( char const * recv_str,  uint8_t * decoded_bytes, int * fport);
int getAtRecvFport( char const * recv_str);
uint8_t * getAtRecvBytes(char const * recv_str);
int getAtRecvBytesLength(char const * recv_str);
int getDataSize(int bufferSize);
void mergeData(char* merged, unsigned char* buffer, int bufferSize);
void printHex(unsigned char * str,int bufferSize);
char * newMergedData(unsigned char * buffer, int bufferSize);
 u8 reverse_bit8(u8 x);
unsigned char cal_table_high_first(unsigned char value);

#endif
