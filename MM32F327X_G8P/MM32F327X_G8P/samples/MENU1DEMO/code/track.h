/*
 * track.h
 *
 *  Created on: 2023��10��24��
 *      Author: lychee
 */

#ifndef CODE_TRACK_H_
#define CODE_TRACK_H_
#include "zf_common_headfile.h"
#include "photo_chuli.h"

extern int16 trackline[MT9V03X_H];
//����Ѳ��ģʽö������
enum track_type_e {
    TRACK_LEFT,//�������
    TRACK_RIGHT,//���ұ���
    TRACK_MID,//������
};
//���嵱ǰѭ��ģʽ
extern enum track_type_e track_type;

void switch_trackline(void);
void choose_tracktype(void);

#endif /* CODE_TRACK_H_ */
