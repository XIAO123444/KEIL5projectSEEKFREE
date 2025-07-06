/*
 * track.c
 *  Created on: 2023��10��24��
 *      Author: lychee
 */
#include "track.h"
extern uint16 centerline[MT9V03X_H];
extern uint16 leftline[MT9V03X_H];
extern uint16 rightline[MT9V03X_H];
extern uint8 pix_per_meter;//ÿ�׵�������

int16 trackline[MT9V03X_H];//������
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


enum track_type_e track_type = TRACK_MID;//Ĭ��ѭ����
//�����߸�ֵ��ѭ��������
void switch_trackline(void){
    if(track_type == TRACK_MID){
        for(int i = 0; i < MT9V03X_H;i ++){
            trackline[i] = centerline[i];//ѭ����
        }
    }
    if(track_type == TRACK_LEFT){
        for(int i = 0; i < MT9V03X_H;i ++){
            trackline[i] = (leftline[i] + pix_per_meter) > 187 ? 187 : (int16)(leftline[i] + 0.2 * pix_per_meter);//ѭ���ߣ�����Ӧ���Ҳ�ƫ�Ƶ�·���һ�룬��0.2�׶�Ӧ��������
        }
    }
    if(track_type == TRACK_RIGHT){
        for(int i = 0; i < MT9V03X_H;i ++){
            trackline[i] = ((rightline[i]) - pix_per_meter) < 0 ? 0 : (int16)(rightline[i] - 0.2 * pix_per_meter);//ѭ���ߣ�����Ӧ�����ƫ��
        }
    }
}
//ѡ�����ѭ�ߣ���ҿ������ɷ���
void choose_tracktype(void){
    //track_type = TRACK_LEFT;
}
