#include "pid_v.h"

struct pid_v
{
    float p;            //比例系数
    float i;            //积分系数
    float d;            //微分系数
    float i_max;        //积分限幅
    float d_max;        //微分限幅
    float output;       //输出限幅
}PID_V;

void pid_init(void)
{
    
}