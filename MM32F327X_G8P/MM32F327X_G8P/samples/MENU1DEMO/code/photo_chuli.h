#ifndef PHOTO_CHULI_H__
#define PHOTO_CHULI_H__

#include "zf_common_headfile.h"
void difsum_left(uint8 y,uint8 x);
void difsum_right(uint8 y,uint8 x);
void image_boundary_process(void);
bool stop_flag(void);

int find_pointright(uint8 start,uint8 end);
int continuity_left(uint8 start,uint8 end);
int montonicity_right(uint8 start,uint8 end);
int continuity_right(uint8 start,uint8 end);


uint16 output_middle(void);

#endif