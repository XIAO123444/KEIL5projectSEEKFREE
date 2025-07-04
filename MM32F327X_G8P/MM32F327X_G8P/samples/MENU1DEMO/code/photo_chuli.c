#include "photo_chuli.h"




uint8 image_copy2[MT9V03X_H][MT9V03X_W]={0};

uint16 w_step,h_step,K,limit;
void chuli_init(void)
{
    w_step=1;h_step=1;K=1;limit=1;
}

//bool cal_compare(uint8 a,uint8 b,uint16 K,uint16 limit)
//{
//    uint16 differ=(a>b)?((uint16)a-(uint16)b):((uint16)b-(uint16)a);
//    uint16 result=(differ<<K)/((uint16)b+(uint16)a+1);
//    if(result>=limit)
//    {
//        return true; //�߽��
//    }
//    else
//    {
//        return false;
//    }
//}
bool cal_compare(uint8 a,uint8 b,uint16 K,uint16 limit)
{
    uint16 differ=(a>b)?((uint16)a-(uint16)b):((uint16)b-(uint16)a);
    uint16 result=(differ<<K)/((uint16)b+(uint16)a+1);
    if(result>=limit)
    {
        return true; //�߽��
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
@����ͼ��
@const uint8 *image ԭͼ��ָ��
@uint8 *image2      ������ͼ��ָ��
@uint16 width       ͼ����
@uint16 height      ͼ��߶�
@uint32 w_step      ��Ȳ���
@uint32 h_step      �߶Ȳ���
@uint16 K           ����ϵ��
@uint16 limit       ��ֵ
*/


uint16 ips200_chuli_image(const uint8 *image,uint8 *image2, uint16 width, uint16 height,uint16 w_step,uint16 h_step,uint16 K,uint16 limit)
{
    memset(image2,0,width*height);
    uint16 i = 0, j = 0;
    uint16 color = 0,temp1,temp2 = 0;
    uint32 width_index = 0, height_index = 0;
    uint16 mark1;                                //��ǲ�������
    uint16 max=0;                                 //��ǲ������ֵ����ʼΪ0��
    for(i = 0; i < width; i +=w_step)
    {
        width_index = i;
        temp1 = *(image + width_index);               // ��ȡ���ص�
    
        for(j = h_step; j < height-h_step; j +=h_step)
        {
            height_index = j;
            temp1 = *(image + height_index * width + width_index);               // ��ȡ���ص�
            temp2 = *(image + (height_index+h_step) * width + width_index);               // ��ȡ���ص�
            
            if(cal_compare(temp1,temp2,K,limit))
            {
                if(j>max)
                {
                    max=j;
                    mark1=width_index;
                }
                break;
            }
        }
        
    }

    
         for(j = h_step; j < height-h_step; j +=h_step)
        {
            uint16 mark=mark1;
            height_index=j;
                //��������mark�������������
            for(i=mark;i>=w_step;i-=w_step)
            {
                width_index=i;
                temp1=*(image + height_index * width + width_index);
                temp2=*(image + height_index * width + width_index-w_step);
                if(cal_compare(temp1,temp2,K,limit))
                {
                     image2[height_index*width+width_index]=255;
                    break;
                }mark1=width_index;
            }
            //��������mark���������Ҵ�����
            for(i=mark;i<=width-w_step;i+=w_step)
            {
                width_index=i;
               
                temp1=*(image + height_index * width + width_index);
                temp2=*(image + height_index * width + width_index+w_step);
                if(cal_compare(temp1,temp2,K,limit))
                {
                      image2[height_index*width+width_index]=255;
                        break;
                    
                }
            } 
            mark1=(mark1+width_index)/2;
        }
    
   
return mark1;
}

void Photo_Chuli(void)
{
    if(mt9v03x_finish_flag){
        memcpy(image_copy2, mt9v03x_image, MT9V03X_H*MT9V03X_W);             //�˴�����ͼ��
        ips200_chuli_image((const uint8 *)mt9v03x_image,(uint8 *)image_copy2,MT9V03X_W,MT9V03X_H,w_step,h_step,K,limit);
        mt9v03x_finish_flag=1;
    }
}

// �ӷ�ʵ�֣�ֱ�ӵ������������飩
void w_step_add(void) { w_step++; }
void h_step_add(void) { h_step++; }
void K_add(void)     { K++; }
void limit_add(void) { limit++; }

// ����ʵ�֣���Сֵ����Ϊ1��
void w_step_sub(void) { if (w_step > 1) w_step--;else w_step=1; }
void h_step_sub(void) { if (h_step > 1) h_step--;else h_step=1 ;}
void K_sub(void)     { if (K > 1) K--;  else K=1; }
void limit_sub(void) { if (limit > 1) limit--; else limit=1; }