#include "photo_chuli.h"
uint8 image_copy2[MT9V03X_H][MT9V03X_W]={0};
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
void set_area(uint16 x,uint16 y,uint8 *image2,uint16 width,uint16 height,uint16 r)
{
    if(x>r&&x<(width-r)&&y>r&&y<height-r)
    {
        for(int i=x-r;i<=x+r;i++)
        {
            for(int j=y-r;j<=y+r;j++)
            {
                image2[(y)*width+x]=255;

            }
        }
    }
}
/*
@处理图象
@const uint8 *image 原图象指针
@uint8 *image2      处理结果图象指针
@uint16 width       图象宽度
@uint16 height      图象高度
@uint32 w_step      宽度步长
@uint32 h_step      高度步长
@uint16 K           比例系数
@uint16 limit       阈值
*/


void ips200_chuli_image(const uint8 *image,uint8 *image2, uint16 width, uint16 height,uint16 w_step,uint16 h_step,uint16 K,uint16 limit)
{
    memset(image2,0,width*height);
    uint16 i = 0, j = 0;
    uint16 color = 0,temp1,temp2 = 0;
    uint32 width_index = 0, height_index = 0;
    uint16 mark;                                //标记查找中线
    uint16 max=0;                                 //标记查找最大值（初始为0）
    for(i = 0; i < width; i +=w_step)
    {
        width_index = i;
        temp1 = *(image + width_index);               // 读取像素点
        if(temp1<20)                                  //初始起点判断。如果是黑色直接不要了
        {
            break;
        }
        for(j = h_step; j < height-h_step; j +=h_step)
        {
            height_index = j;
            temp1 = *(image + height_index * width + width_index);               // 读取像素点
            temp2 = *(image + (height_index+h_step) * width + width_index);               // 读取像素点
            
            if(cal_compare(temp1,temp2,K,limit))
            {
                if(j>max)
                {
                    max=j;
                    mark=width_index;
                }
                break;
            }
        }
    }

    for(j = h_step; j < height-h_step; j +=h_step)
        {
            height_index=j;
                //接下来从mark处从右向左遍历
            for(i=mark;i>=w_step;i-=w_step)
            {
                width_index=i;
                temp1=*(image + height_index * width + width_index);
                temp2=*(image + height_index * width + width_index-w_step);
                if(cal_compare(temp1,temp2,K,limit))
                {
                    set_area(width_index,height_index,image2,width,height,w_step);
                    break;//考虑要不要删去
                }
            }
            //接下来从mark处从左向右处遍历
            for(i=mark;i<=width-w_step;i+=w_step)
            {
                width_index=i;
               
                temp1=*(image + height_index * width + width_index);
                temp2=*(image + height_index * width + width_index+w_step);
                if(cal_compare(temp1,temp2,K,limit))
                {
                    set_area(width_index,height_index,image2,width,height,w_step);
                    break;
                }
            }        
        }
}
