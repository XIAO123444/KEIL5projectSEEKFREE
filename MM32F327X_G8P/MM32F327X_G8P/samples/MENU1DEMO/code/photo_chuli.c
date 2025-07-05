#include "photo_chuli.h"




uint16 w_step,h_step,K,limit;
volatile uint16 left_edge[MT9V03X_H]={0};
volatile uint16 right_edge[MT9V03X_H]={0};
volatile uint16 middle_edge[MT9V03X_H]={0};
void chuli_init(void)
{
    w_step=1;h_step=1;K=1;limit=1;
}


bool cal_compare(uint8 a,uint8 b,uint16 K,uint16 limit)
{
    uint16 differ=(a>b)?((uint16)a-(uint16)b):((uint16)b-(uint16)a);
    uint16 result=(differ<<K)/((uint16)b+(uint16)a+1);
    if(result>=limit)
    {
        return true; //边界点
    }
    else
    {
        return false;
    }
}

/*
@处理图象
@const uint8 *image 原图象指针

@uint32 w_step      宽度步长
@uint32 h_step      高度步长
@uint16 K           比例系数
@uint16 limit       阈值
*/


void ips200_chuli_image(const uint8 *image,uint16 w_step,uint16 h_step,uint16 K,uint16 limit)
{
    uint16 i = 0, j = 0;
    uint16 temp1,temp2 = 0;
    uint16 width_index, height_index;
    uint16 mark1;                                //标记查找中线
    uint16 max=0;                                 //标记查找最大值（初始为0）
    for(i = 0; i < MT9V03X_W-w_step; i +=w_step)
    {
        width_index = i;
    
        for(j = MT9V03X_H-h_step; j > h_step; j -=h_step)
        {
            height_index = j;
            temp1 = *(image + height_index * MT9V03X_W + width_index);               // 读取像素点
            temp2 = *(image + (height_index-h_step) * MT9V03X_W + width_index);               // 读取像素点
            
            if(cal_compare(temp1,temp2,K,limit))
            {
                if(j<max)
                {
                    max=j;
                    mark1=width_index;
                }
                break;
            }
        }
        
    }

    
         for(j = MT9V03X_H-1; j >1 ; j --)//考虑到性能大幅度提升
        {
            uint16 mark=mark1;
            height_index=j;
                //接下来从mark处从右向左遍历
            for(i=mark;i>=w_step;i-=w_step)
            {
                width_index=i;
                temp1=*(image + height_index * MT9V03X_W + width_index);
                temp2=*(image + height_index * MT9V03X_W + width_index-w_step);
                if(cal_compare(temp1,temp2,K,limit))
                {                   
                    left_edge[height_index]=width_index;
                    break;
                }
       
                mark1=width_index;
            }
            //接下来从mark处从左向右处遍历
            for(i=mark;i<=MT9V03X_H-w_step;i+=w_step)
            {
                width_index=i;
               
                temp1=*(image + height_index * MT9V03X_W + width_index);
                temp2=*(image + height_index * MT9V03X_W + width_index+w_step);
                if(cal_compare(temp1,temp2,K,limit))
                {
                        right_edge[height_index]=width_index;
                        break;
                    
                }

            } 
            mark1=(mark1+width_index)/2;
            middle_edge[height_index]=mark1;
        }

}



void show_line(void)
{
    for(uint16 i = 0; i < MT9V03X_H-1; i ++)
    {
        
//        ips200_draw_point(left_edge[i],i+100,12);
//        ips200_draw_point(right_edge[i],i+100,12);
//        ips200_draw_point(middle_edge[i],i+100,12);

        printf("%d,",left_edge[i]);
        printf("%d,",right_edge[i]);
        printf("%d,",middle_edge[i]);


    }
                                             //可以再加入循迹线。。。后续制作                     
}

void photoShow(void)
{   
    if(mt9v03x_finish_flag){
    ips200_show_gray_image(0, 100, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);    
       ips200_chuli_image((const uint8 *)mt9v03x_image,w_step,h_step,K,limit);
        show_line();    mt9v03x_finish_flag=0;
        
    }
    
}
// 加法实现（直接递增，无溢出检查）
void w_step_add(void) { w_step++; }
void h_step_add(void) { h_step++; }
void K_add(void)     { K++; }
void limit_add(void) { limit++; }

// 减法实现（最小值保护为1）
void w_step_sub(void) { if (w_step > 1) w_step--;else w_step=1; }
void h_step_sub(void) { if (h_step > 1) h_step--;else h_step=1 ;}
void K_sub(void)     { if (K > 1) K--;  else K=1; }
void limit_sub(void) { if (limit > 1) limit--; else limit=1; }