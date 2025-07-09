#include "steer_pid.h"
#include "photo_chuli.h"
struct steer_pid S_PID;
void PID_init(void)
{
    S_PID.p=0;
    S_PID.i=0;
    S_PID.d=0;
    S_PID.outputmax=140;
    S_PID.outputmin=-140;
    S_PID.target=188/2;
}
float LastLasterror=0;

volatile float derivative=0;
volatile float intgral=0;
volatile float error=0;
volatile float Lasterror=0;
float turn;
float addturn;
struct steer_pid* SPID_vget_param(void)
{
    return &S_PID;
}

int S_PID_CAL()
{
    int16 measure=output_middle();
    error = 94-(float)measure;//大于0的时候是左偏移<0右偏
    intgral+=error;
    derivative=error-Lasterror;
    int result=( int )(S_PID.p*error+S_PID.i*intgral+S_PID.d*derivative);
    if(intgral>80)
    {
        intgral=80;
    }
    if(intgral<-80)
    {
        intgral=-80;
    }
    if(result>140)
    {
        result=140;
    }
    if(result<-140)
    {
        result=-140;

    }
    Lasterror=error;
    return result;
}

//int S_PID_CAL()//
//{
//    int16 measure=output_middle();
//    error= 94 - (float)measure;
//    addturn=S_PID.p*(error-Lasterror)+S_PID.i*error+S_PID.d*(error+LastLasterror-2*Lasterror);
//    if(1)
//    { 
//        turn+=addturn;
//    }
//    if(turn>140)
//        turn=140;
//    if(turn<-140)
//        turn=-140;
//    LastLasterror=Lasterror;
//    Lasterror=error;
//    printf("%d\n",(int)turn);
//    return (int)turn;
//}







void S_PIDadd_p(void){
    
    S_PID.p+=0.1;
}
void S_PIDadd_i(void) {
    S_PID.i += 0.1;  


}
void S_PIDadd_d(void) {
    S_PID.d += 0.01;  

}    


void S_PIDadd_outputmax(void) {
    S_PID.outputmax += 100;
    S_PID.outputmin -= 100;

}
void S_PIDsub_p(void) {
    if (S_PID.p > 0.1) {  
        S_PID.p -= 0.1;   
        if (S_PID.p < 0) S_PID.p = 0;  
    }
}
void S_PIDsub_i(void) {
    if (S_PID.i > 0.01) {
        S_PID.i -= 0.01;
        if (S_PID.i < 0) S_PID.i = 0;
    }
}
void S_PIDsub_d(void) {
    if (S_PID.d > 0.1) {
        S_PID.d -= 0.1;
        if (S_PID.d < 0) S_PID.d = 0;
        // 微分限幅保护
        }
}


void S_PIDsub_outputmax(void) {
    if (S_PID.outputmax > 100) {  
        S_PID.outputmax -= 100;
        S_PID.outputmin += 100;

    }
}
