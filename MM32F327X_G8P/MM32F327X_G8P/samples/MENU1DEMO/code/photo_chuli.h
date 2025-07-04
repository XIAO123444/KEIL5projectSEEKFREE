#ifndef PHOTO_CHULI_H__
#define PHOTO_CHULI_H__

#include "zf_common_headfile.h"
extern uint8 image_copy2[MT9V03X_H][MT9V03X_W];
uint16 ips200_chuli_image(const uint8 *image,uint8 *image2, uint16 width, uint16 height,uint16 w_step,uint16 h_step,uint16 K,uint16 limit);
void Photo_Chuli(void);

void w_step_add(void);
void h_step_add(void);
void K_add(void);
void limit_add(void);

// 减法函数（最小值保护为1）
void w_step_sub(void);
void h_step_sub(void);
void K_sub(void);
void limit_sub(void);

#endif