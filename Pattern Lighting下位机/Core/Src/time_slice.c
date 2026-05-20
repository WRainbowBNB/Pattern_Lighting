#include "time_slice.h"

void time_slice_Init(time_slice* poll, uint32_t interval, void(*callback)(void)){
    poll -> in_last_time = HAL_GetTick();
    poll -> in_interval = interval;
    poll -> in_callback = callback;
}

void time_slice_run(time_slice* poll){
    if(HAL_GetTick() - poll->in_last_time >= poll -> in_interval){
        poll -> in_last_time = HAL_GetTick();
        poll -> in_callback();
    }
}

void Breathing_light(){
    if (system_flag == 1)
    {
        static uint16_t count = 0;
        count ++;
        if(count >= 1800) count = 0;
        uint16_t brightness = (count <= 1100)? ((count * 500) / 1100) 
                                            : (500 - (count - 1100) * 500 / 700);       
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, brightness);
    }else{
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    }
    
}

//ber我时间片20ms一次自己就消抖了吧
void KEY_callback(){
    static uint8_t key_state = 0; // 0:按键释放, 1:按键按下, 2:长按后消抖
    static uint8_t debounce_cnt = 0;
    uint8_t key_value = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
       switch(key_state){
        //按键没被按下
        case 0:
            debounce_cnt = 0; //重置消抖计数
            if(key_value == GPIO_PIN_SET){
                debounce_cnt ++;
                key_state = 1; //按键按下
            }else{
                key_state = 0; //按键释放
            }
            break;
        //按键被按下
        case 1:
            //判断时间是否超过系统时间，确认按键有效
            if(debounce_cnt * DEBOUNCE_TIME >= SYSTEM_TIME){
                //按键仍然按下，确认是想要切换系统状态
                if(key_value == GPIO_PIN_SET){
                    system_flag = !system_flag; // 切换系统标志
                    if(system_flag == 0){
                        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET); //系统运行状态，LED1熄灭
                    }
                    debounce_cnt = 0; // 重置消抖计数
                }
                key_state = 2; //进入消抖
            }else{
                if(key_value == GPIO_PIN_RESET){
                    stm32_to_computer();
                    debounce_cnt = 0;// 重置消抖计数
                    key_state = 0; //重置状态
                }else{
                    key_state = 1; //继续本状态
                    debounce_cnt ++;
                }
            }
            break;
        //消抖状态
        case 2:
            if(key_value == GPIO_PIN_RESET){
                key_state = 0; //按键释放，重置状态
                debounce_cnt = 0; // 重置消抖计数
            }
            break;
    }
}

void LED1_toggle(){
    // 非系统运行状态，LED1不闪烁
    if(system_flag == 1){
        if(num != 0){
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            num --;
        }
    }
    
}

void Rx_UART_callback(){
    if(system_flag){
        Ex_Data(&Rx_Buffer);
    }else{
        //非系统运行状态，直接更新DMA指针,避免读到旧数据
        uint16_t remaining_space = __HAL_DMA_GET_COUNTER(huart1.hdmarx);
        Rx_Buffer.old_pos = (Rx_Buffer.buf_size - remaining_space) % Rx_Buffer.buf_size;
        Rx_Buffer.line_pos = 0;
    }
}