#include "pid_v.h"

struct pid_v
{
    float p;            //����ϵ��
    float i;            //����ϵ��
    float d;            //΢��ϵ��
    float i_max;        //�����޷�
    float d_max;        //΢���޷�
    float output;       //����޷�
}PID_V;

void pid_init(void)
{
    
}