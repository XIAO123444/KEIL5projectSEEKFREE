/*
 * track.c
 *  Created on: 2023��10��24��
 *      Author: lychee
 */
#include "track.h"
#include "photo_chuli.h"
extern uint16 centerline[MT9V03X_H];
extern uint16 leftline[MT9V03X_H];
extern uint16 rightline[MT9V03X_H];
extern uint8 pix_per_meter;//ÿ�׵�������
extern uint16 rightfollowline[MT9V03X_H];
extern uint16 leftfollowline[MT9V03X_H];
uint16 centerline2[MT9V03X_H];
int16 trackline[MT9V03X_H];//������
enum mark
{
    straight,
    crossroad_pre,
    crossroad_now,
    crossroad_after,
    round_pre,
    round_now,
    round_out
    
}MARK;
int carstatus_now=0;

int dx1=0;int dx2=0;
//�����߸�ֵ��ѭ��������
void switch_mark(void){
    
    switch (MARK)
    {
        
    	case straight:
        
    		break;
        case crossroad_pre:
            default:
    		break;
    }
}
//ѡ�����ѭ�ߣ���ҿ������ɷ���
void choose_tracktype(void){
    //track_type = TRACK_LEFT;
}
