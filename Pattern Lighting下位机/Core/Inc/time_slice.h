#ifndef __TIME_SLICE_H__
#define __TIME_SLICE_H__

#include "main.h"
#include "tim.h"
#include "ex_message.h"

#define DEBOUNCE_TIME 20 // 消抖时间，单位：ms
#define SYSTEM_TIME 2000 // 按键按下时间，单位：ms
typedef struct {
    uint32_t in_interval;
    uint32_t in_last_time;
    void(*in_callback)(void);
} time_slice;

typedef enum {
    KEY_PRESS,
    KEY_RELEASE,
    KEY_DEBOUNCE,
} KEY_STATE;

void time_slice_Init(time_slice* poll, uint32_t interval, void(*callback)(void));
void time_slice_run(time_slice* poll);
void Breathing_light(void);
void KEY_callback(void);
void LED1_toggle(void);
void Rx_UART_callback(void);



#endif 


