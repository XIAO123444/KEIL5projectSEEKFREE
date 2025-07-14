#ifndef PHOTO_CHULI_H__
#define PHOTO_CHULI_H__

#include "zf_common_headfile.h"
void difsum_left(uint8 y,uint8 x);
void difsum_right(uint8 y,uint8 x);
void image_boundary_process(void);
bool stop_flag(void);

int Find_Left_Down_Point(int start,int end);
int Find_Right_Down_Point(uint8 start,uint8 end);
int continuity_left(uint8 start,uint8 end);
int montonicity_right(uint8 start,uint8 end);
int continuity_right(uint8 start,uint8 end);

//Ê®×Ö
void Find_Down_Point(int start,int end);
void Find_Up_Point(int start,int end);

void draw_Lline_k(int16 startx, int16 starty, int16 endy, float dx);
void draw_Rline_k(int16 startx, int16 starty, int16 endy, float dx);

void add_Rline_k(int16 startx, int16 starty, int16 endy,int16 endx);
void add_Lline_k(int16 startx, int16 starty, int16 endy,int16 endx);

void lenthen_Left_bondarise(int16 start);
void lenthen_Right_bondarise(int16 start);

void banmaxian_check(void);

int16 output_middle(void);

#endif