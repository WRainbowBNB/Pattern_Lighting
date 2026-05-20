#ifndef __EX_MESSAGE_H__
#define __EX_MESSAGE_H__

#include "main.h"
#include "usart.h"
#include "tim.h"

//电脑->单片机
#define HEADER1 0xFA
#define HEADER2 0xAF
#define TAILER1 0xFB
#define TAILER2 0xBF
//单片机->电脑
#define HEADER3 0xFC
#define HEADER4 0xCF
#define TAILER3 0xFD
#define TAILER4 0xDF

typedef struct{
	uint8_t *buf;
	uint8_t buf_size;
	uint8_t old_pos;
	uint16_t line_pos; 		// 当前行数据位置
	uint8_t line_temp[128]; // 用于存储一行数据
	uint16_t sum;
	
} RXBUFFER;

void Ex_Data(RXBUFFER *rx_buf);
void stm32_to_computer(void);
void stm32_response(void);

extern uint8_t rx_buf[64];
extern uint8_t tx_buf[64];
extern RXBUFFER Rx_Buffer;
extern uint16_t num;
extern uint8_t command_flag; 

#endif 

