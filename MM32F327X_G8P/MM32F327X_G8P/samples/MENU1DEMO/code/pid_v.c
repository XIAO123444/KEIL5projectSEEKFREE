#include "zf_common_headfile.h"
#include "encoder.h"
#include "motor.h"
volatile float  duty1=0;
volatile float  duty2=0;
volatile float integral1=0;
volatile float derivative1=0;
volatile float integral2=0;
volatile float derivative2=0;
volatile float lasterror1=0;
volatile float lasterror2=0;
  struct pid_v
{
    float output_max;       //输出限幅   
    float p;            //比例系数
    float i;            //积分系数
    float d;            //微分系数
    float i_max;        //积分限幅
    float d_max;        //微分限幅
    
}PID_V;
void pid_vparam_init(void)
{
    PID_V.d=0.5;
    PID_V.d_max=0.5;
    PID_V.i=0.5;
    PID_V.i_max=0.5;
    PID_V.output_max=0.5;
    PID_V.p=0.5;
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
void pid_control(int16 target1,int16 target2)
{
    float error1=(target1-Encoder_GetInfo_L())/60;//左
    float error2=(target2-Encoder_GetInfo_R())/60;//右
    integral1+=error1;
    integral2+=error2;
    derivative1=error1-lasterror1;
    derivative2=error2-lasterror2;
    if(integral1>PID_V.i_max){integral1=PID_V.i_max;}
    if(integral1<-PID_V.i_max){integral1=-PID_V.i_max;}
    if(derivative1>PID_V.d_max){derivative1=PID_V.d_max;}
    if(derivative1<-PID_V.d_max){derivative1=-PID_V.d_max;}
    
    if(integral2>PID_V.i_max){integral2=PID_V.i_max;}
    if(integral2<-PID_V.i_max){integral2=-PID_V.i_max;}
    if(derivative2>PID_V.d_max){derivative2=PID_V.d_max;}
    if(derivative2<-PID_V.d_max){derivative2=-PID_V.d_max;}


    lasterror1=error1;
    lasterror2=error2;
    
    duty1=(PID_V.p*error1+PID_V.i*integral1+PID_V.d*derivative1);
    duty2=(PID_V.p*error2+PID_V.i*integral2+PID_V.d*derivative2);
  

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
        // 微分限幅保护
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
    if (PID_V.output_max > 0.1) {  // 输出限幅通常需大于执行机构最小需求
        PID_V.output_max -= 0.1;
    }
}







