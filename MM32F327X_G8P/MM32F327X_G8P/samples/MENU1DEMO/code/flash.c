#include "flash.h"
#include "pid_v.h"
extern struct pid_v PID_V;
extern uint16 w_step,h_step,K,limit;


void flash_init(void)
{
    flash_buffer_clear();
////这里写要载入的数据
    flash_read_page_to_buffer(100, 0);                            // 将数据从 flash 读取到缓冲区
    PID_V.p                =flash_union_buffer[0].float_type;
    PID_V.i                =flash_union_buffer[1].float_type;
    PID_V.d                =flash_union_buffer[2].float_type;
    PID_V.i_max            =flash_union_buffer[3].float_type;
    PID_V.d_max            =flash_union_buffer[4].float_type;
    PID_V.output_max       =flash_union_buffer[5].float_type;
    flash_buffer_clear();
    flash_read_page_to_buffer(100, 1);                            // 将数据从 flash 读取到缓冲区
    w_step                 =flash_union_buffer[0].uint16_type;
    h_step                 =flash_union_buffer[1].uint16_type;
    K                      =flash_union_buffer[2].uint16_type;
    limit                  =flash_union_buffer[3].uint16_type;
    
    pit_ms_init(TIM7_PIT,10000);
    interrupt_set_priority(TIM7_IRQn, 10);

}