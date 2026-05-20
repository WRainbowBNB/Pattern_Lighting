#include "ex_message.h"

uint8_t rx_buf[64];
uint8_t command_flag = 0; // 用于指示是否接收到完整指令
uint8_t action_flag = 0;  // 用于指示是否执行了指令
uint8_t key_cnt = 0; 
uint16_t num = 0; // LED1闪烁次数
RXBUFFER Rx_Buffer = {
    .buf = rx_buf,
    .buf_size = sizeof(rx_buf),
    .old_pos = 0,
    .line_pos = 0,
    .line_temp = {0},
	.sum = 0
};

void Handle_Command(uint8_t command, uint8_t data){
    if(command == 0x01){
        switch(data){
            case 0x00:
                HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                action_flag = 1; // 设置指令执行标志
                break;
            case 0x01:
                HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                action_flag = 1; // 设置指令执行标志
                break;
            default:
                action_flag = 0; // 重置指令执行标志
                break;
        }
    }else if(command == 0x02){
        num = data * 2;
        action_flag = 1; // 设置指令执行标志
    }
    stm32_response(); // 发送响应
}

void Ex_Data(RXBUFFER *rx_buf)
{
    //获取DMA剩余空间
    uint16_t remaining_space = __HAL_DMA_GET_COUNTER(huart1.hdmarx);
    //计算当前数据下标
    uint8_t dma_write_ptr = (rx_buf->buf_size - remaining_space) % rx_buf->buf_size;
    //写指针变化，说明有新数据到达
    if(dma_write_ptr != rx_buf->old_pos){
        //处理新数据
        while(dma_write_ptr != rx_buf->old_pos){
            //获取当前数据字节
            uint8_t byte = rx_buf->buf[rx_buf->old_pos];
            rx_buf->line_temp[rx_buf->line_pos] = byte;
            //根据当前行数据位置进行状态机处理
            switch (rx_buf->line_pos){  
                //帧头1
                case 0:
                    if(byte == HEADER1){
                        rx_buf->sum = byte;
                        rx_buf->line_pos = 1;
                    }else{
                        rx_buf->line_pos = 0;
                    }
                    break;
                //帧头2
                case 1:
                    if(byte == HEADER2){
                        rx_buf->sum += byte;
                        rx_buf->line_pos = 2;
                    }else{
                        rx_buf->line_pos = 0;
                    }
                    break;
                //指令码0x01或0x02
                case 2:
                    if(byte == 0x01 || byte == 0x02){
                        rx_buf->sum += byte;
                        rx_buf->line_pos = 3;
                    }else{
                        rx_buf->line_pos = 0;
                    }
                    break;
                //指令内容
                case 3:
                    rx_buf->sum += byte;
                    rx_buf->line_pos = 4;
                    break;
                //校验位
                case 4:
                    if(byte == (rx_buf->sum & 0xFF)){
                        rx_buf->line_pos = 5;
                    }else{
                        rx_buf->line_pos = 0;
                    }
                    break;
                //帧尾1
                case 5:
                    if(byte == TAILER1){
                        rx_buf->line_pos = 6;
                    }else{
                        rx_buf->line_pos = 0;
                    }
                    break;
                //帧尾2
                case 6:
                    if(byte == TAILER2){
                        //接收完整指令，后续处理
                        Handle_Command(rx_buf->line_temp[2], rx_buf->line_temp[3]);
                        rx_buf->line_pos = 0;
                    }else{
                        rx_buf->line_pos = 0;
                    }
                    break;
                //后续处理
                default:
                    break;
            }
            //更新读指针位置
            rx_buf->old_pos++;
            //让读指针指向下一个数据位置
            if(rx_buf->old_pos >= rx_buf->buf_size) rx_buf->old_pos = 0;
        }
        
    }
}

//按键短按触发这个
void stm32_to_computer(){
    uint8_t a[2] = {0x01, 0x00}; //要发的
    static uint8_t tx_buf[64];
    //发送数据到计算机
    tx_buf[0] = HEADER1;
    tx_buf[1] = HEADER2;
    tx_buf[2] = 0x01; //指令码
    tx_buf[5] = TAILER1;
    tx_buf[6] = TAILER2;
    tx_buf[3] = a[key_cnt]; //指令内容
    key_cnt = !key_cnt;
    //校验位
    tx_buf[4] = tx_buf[0] + tx_buf[1] + tx_buf[2] + tx_buf[3]; // Checksum
    HAL_UART_Transmit_DMA(&huart1, tx_buf, 7);
}

//发完命令单片机再响应
void stm32_response(){
    static uint8_t tx_buf[64];
    //发送数据到计算机
    tx_buf[0] = HEADER3;
    tx_buf[1] = HEADER4;
    tx_buf[5] = TAILER3;
    tx_buf[6] = TAILER4;

    if(command_flag == 1){
        tx_buf[2] = 0x03; //确认接收指令
        command_flag = 0; //重置指令标志位
    }else{
        tx_buf[2] = 0x00; //未接收到指令
    }
    if(action_flag == 1){
        tx_buf[3] = 0x01; //指令执行成功
        action_flag = 0; //重置指令执行标志位
    }else{
        tx_buf[3] = 0x00; //指令执行失败
        action_flag = 0; //重置指令执行标志位
    }
    //校验位
    tx_buf[4] = tx_buf[0] + tx_buf[1] + tx_buf[2] + tx_buf[3]; // Checksum
    HAL_UART_Transmit_DMA(&huart1, tx_buf, 7);
}