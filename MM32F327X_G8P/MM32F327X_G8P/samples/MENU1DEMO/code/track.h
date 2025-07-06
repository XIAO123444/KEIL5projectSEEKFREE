/*
 * track.h
 *
 *  Created on: 2023年10月24日
 *      Author: lychee
 */

#ifndef CODE_TRACK_H_
#define CODE_TRACK_H_
#include "zf_common_headfile.h"
#include "photo_chuli.h"

extern int16 trackline[MT9V03X_H];
//定义巡线模式枚举类型
enum track_type_e {
    TRACK_LEFT,//沿左边线
    TRACK_RIGHT,//沿右边线
    TRACK_MID,//沿中线
};
//定义当前循线模式
extern enum track_type_e track_type;

void switch_trackline(void);
void choose_tracktype(void);

#endif /* CODE_TRACK_H_ */
