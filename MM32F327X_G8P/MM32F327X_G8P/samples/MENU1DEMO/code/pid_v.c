#include "zf_common_headfile.h"

   
  
  struct pid_v
{
    float output_max;       //����޷�   
    float p;            //����ϵ��
    float i;            //����ϵ��
    float d;            //΢��ϵ��
    float i_max;        //�����޷�
    float d_max;        //΢���޷�
    
}PID_V;
void pid_vparam_init(void)
{
    PID_V.d=0.0;
    PID_V.d_max=0.0;
    PID_V.i=0.0;
    PID_V.i_max=0.0;
    PID_V.output_max=0;
    PID_V.p=0.0;
}

struct pid_v* PID_vget_param(void)
{
    return &PID_V;
}
void PID_vset_param(struct pid_v* p)
{
    PID_V.p         =p          ->p;
    PID_V.d         =p          ->d;
    PID_V.d_max     =p          ->d_max;
    PID_V.i         =p          ->i;
    PID_V.i_max     =p          ->i_max;
    PID_V.output_max=p          ->output_max;

}
void pid_add_p(void){
    
    PID_V.p+=0.1;
}
void pid_add_i(void) {
    PID_V.i += 0.1;  


}
void pid_add_d(void) {
    PID_V.d += 0.1;  

}    
void pid_add_i_max(void) {
    PID_V.i_max += 0.1; 
}
void pid_add_d_max(void) {
    PID_V.d_max += 0.1;
}
void pid_add_output_max(void) {
    PID_V.output_max += 1.0;
}
void pid_sub_p(void) {
    if (PID_V.p > 0.1) {  
        PID_V.p -= 0.1;   
        if (PID_V.p < 0) PID_V.p = 0;  
    }
}
void pid_sub_i(void) {
    if (PID_V.i > 0.1) {
        PID_V.i -= 0.1;
        if (PID_V.i < 0) PID_V.i = 0;
    }
}
void pid_sub_d(void) {
    if (PID_V.d > 0.1) {
        PID_V.d -= 0.1;
        if (PID_V.d < 0) PID_V.d = 0;
        // ΢���޷�����
        }
}

void pid_sub_d_max(void) {
    if (PID_V.d_max > 0.1) {
        PID_V.d_max -= 0.1;
        if (PID_V.d_max < 0) PID_V.d_max = 0;
    }
}
void pid_sub_i_max(void) {
    if (PID_V.i_max > 0.1) {
        PID_V.i_max -= 0.1;
        if (PID_V.i_max < 0) PID_V.i_max = 0;
    }
}
void pid_sub_output_max(void) {
    if (PID_V.output_max > 0.1) {  // ����޷�ͨ�������ִ�л�����С����
        PID_V.output_max -= 0.1;
    }
}







