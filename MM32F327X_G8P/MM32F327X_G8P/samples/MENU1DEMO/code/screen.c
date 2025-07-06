/*
 * screen.c
 *
 *  Created on: 2023年10月24日
 *      Author: lychee
 */
#include "screen.h"
extern uint16 centerline[MT9V03X_H];
extern uint16 leftline[MT9V03X_H];
extern uint16 rightline[MT9V03X_H];
void show_line(void){
    for(uint16 i = 0; i < MT9V03X_H; i = i + 2){
        ips200_draw_point((uint16)trackline[i], i, RGB565_BLACK);//黑色跟踪
    }
    for(uint16 i = 0; i < MT9V03X_H; i ++){
        ips200_draw_point((uint16)leftline[i], i+120, RGB565_RED);//红色左线
        ips200_draw_point((uint16)rightline[i], i+120, RGB565_BLUE);//蓝色右线
        ips200_draw_point((uint16)centerline[i], i+120, RGB565_PURPLE);//紫色中线
    }
}
