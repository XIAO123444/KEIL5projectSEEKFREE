/*
 * track.c
 *  Created on: 2023年10月24日
 *      Author: lychee
 */
#include "track.h"
extern uint16 centerline[MT9V03X_H];
extern uint16 leftline[MT9V03X_H];
extern uint16 rightline[MT9V03X_H];
extern uint8 pix_per_meter;//每米的像素数

int16 trackline[MT9V03X_H];//跟踪线
enum mark
{
    straight=1,
    crossroad_pre=2,
    crossroad_now=3,
    crossroad_after=4,
    round_in=5,
    round_now=6,
    round_out=7
    
};


enum track_type_e track_type = TRACK_MID;//默认循中线
//将边线赋值给循迹跟踪线
void switch_trackline(void){
    if(track_type == TRACK_MID){
        for(int i = 0; i < MT9V03X_H;i ++){
            trackline[i] = centerline[i];//循中线
        }
    }
    if(track_type == TRACK_LEFT){
        for(int i = 0; i < MT9V03X_H;i ++){
            trackline[i] = (leftline[i] + pix_per_meter) > 187 ? 187 : (int16)(leftline[i] + 0.2 * pix_per_meter);//循左线，左线应向右侧偏移道路宽度一半，即0.2米对应的像素数
        }
    }
    if(track_type == TRACK_RIGHT){
        for(int i = 0; i < MT9V03X_H;i ++){
            trackline[i] = ((rightline[i]) - pix_per_meter) < 0 ? 0 : (int16)(rightline[i] - 0.2 * pix_per_meter);//循左线，右线应向左侧偏移
        }
    }
}
//选择如何循线，大家可以自由发挥
void choose_tracktype(void){
    //track_type = TRACK_LEFT;
}
