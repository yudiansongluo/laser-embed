#include "system.h"
#include "SysTick.h"
#include "input.h"
#include "usart.h"
#include "stdarg.h"
#include "string.h"
#include "core_cm3.h"



	char joinCmd [] = "at+join\r\n";
	char testCmd [] = "at+send=lora:1:5A00\r\n";
	char position []	= "at+send=lora:1:008000002e58119309bd1964007b8a\r\n";
	char position1 []	= "at+send=lora:1:008000002e58119309bd1964007b8a\r\n";
	char FeedbackOrder[]="OK Join Success";
	char FeedbackOrder1[]="OK";
	
	
	void JoinNet(){
		delay_ms (5);
		 USART1_printf(joinCmd);

		printf("111 : %s\r\n",string);

		
//	 do
//	{
//	 USART1_printf(joinCmd);
//		delay_ms (4000);
//		delay_ms (4000);
//		  USART1_printf("/r/n");
//  USART1_printf(string);
//	}	while(strncmp(string ,FeedbackOrder,3));
//		
//		do
//	{
//	 	USART1_printf(position);	
//		delay_ms (4000);
//		delay_ms (4000);
//	}	while(strncmp(string ,FeedbackOrder1,2));
//		

//		
	
	}