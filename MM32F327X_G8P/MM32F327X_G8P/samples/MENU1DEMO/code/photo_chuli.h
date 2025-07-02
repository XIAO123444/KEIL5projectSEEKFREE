#ifndef PHOTO_CHULI_H__
#define PHOTO_CHULI_H__

#include "zf_common_headfile.h"
extern uint8 image_copy2[MT9V03X_H][MT9V03X_W];
void ips200_chuli_image(const uint8 *image,uint8 *image2, uint16 width, uint16 height,uint16 w_step,uint16 h_step,uint16 K,uint16 limit);

#endif