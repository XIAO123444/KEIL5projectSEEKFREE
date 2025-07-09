/*
 * camera.c
 *
 *  Created on: 2023��10��24��
 *      Author: lychee
 */
#include "photo_chuli.h"
#include "math.h"
uint16 centerline[MT9V03X_H];
uint16 leftline[MT9V03X_H];
uint16 rightline[MT9V03X_H];
uint16 rightfollowline[MT9V03X_H];
uint16 leftfollowline[MT9V03X_H];



uint8 leftline_num;//���ߵ�����
uint8 rightline_num;//���ߵ�����
uint16 sar_thre = 17;//��Ⱥ���ֵ
uint8 pix_per_meter = 20;//ÿ�׵�������

int right_down_line =0;
//����Ѱ�ұ߽��
void image_boundary_process(void){
    uint8 row;//��
    //uint8 col = MT9V03X_W/2;//��
    uint8 start_col = MT9V03X_W / 2;//��������������,Ĭ��ΪMT9V03X_W / 2
    //����֮ǰ�ļ���
    leftline_num = 0;
    rightline_num = 0;

    for(row = MT9V03X_H - 1; row >= 1; row--){
        //ѡ����һ�е��е���Ϊ��һ�м�����ʼ�㣬��ʡ�ٶȣ�ͬʱ��ֹ������������߾������뻭��һ��
        if(row != MT9V03X_H - 1){
            start_col = (uint8)(0.4 * centerline[row] + 0.3 * start_col + 0.1 * MT9V03X_W);//һ�׵�ͨ�˲�����ֹ�������Ӱ����һ�е���ʼ��
        }
        else if(row == MT9V03X_H - 1){
            start_col = MT9V03X_W / 2;
        }
        //��������Ⱥ� 
        difsum_left(row,start_col);
        difsum_right(row,start_col);
        centerline[row] = 0.5 * (rightline[row] + leftline[row]);
    }
}
//��Ⱥ�Ѱ�����߽��
void difsum_left(uint8 y,uint8 x){
    float sum,dif,sar;//�ͣ����
    uint8 col;//��
    uint8 mov = 2;//ÿ���������ƶ���,Ĭ��Ϊ2�����Ը��ݻ���ֱ��ʵ���
    //�����x�е���߽�
    leftline[y] = 0;//δ�ҵ���߽�ʱ���Ϊ0
    for(col = x; col >= mov + 1; col -= mov){
        dif = (float)((mt9v03x_image[y][col] - mt9v03x_image[y][col - mov - 1])<<8);//����8λ����256���ɱ��⸡�����ˣ��ӿ��ٶ�
        sum = (float)((mt9v03x_image[y][col] + mt9v03x_image[y][col - mov - 1]));
        sar = fabs(dif / sum);//��ȡ��Ⱥ�
        if(sar > sar_thre){//��Ⱥʹ�����ֵ������ǳɫͻ��
            leftline[y] = (int16)(col - mov);
            leftline_num ++;//���ߵ����+
            break;//�ҵ��߽���˳�
        }
    }
}
//��Ⱥ�Ѱ���Ҳ�߽��
void difsum_right(uint8 y,uint8 x){
    float sum,dif,sar;//�ͣ����
    uint8 col;//��
    uint8 mov = 2;//ÿ���������ƶ���,Ĭ��Ϊ2�����Ը��ݻ���ֱ��ʵ���
    //�����x�е���߽�
    rightline[y] = MT9V03X_W - 1;//δ�ҵ��ұ߽�ʱ���Ϊ187
    for(col = x; col <= MT9V03X_W - mov - 1; col += mov){
        dif = (float)((mt9v03x_image[y][col] - mt9v03x_image[y][col + mov + 1])<<8);//����8λ����256���ɱ��⸡�����ˣ��ӿ��ٶ�
        sum = (float)((mt9v03x_image[y][col] + mt9v03x_image[y][col + mov + 1]));
        sar = fabs(dif / sum);//��ȡ��Ⱥ�
        if(sar > sar_thre){//��Ⱥʹ�����ֵ������ǳɫͻ��
            rightline[y] = (int16)(col + mov);
            rightline_num ++;//���ߵ����+
            break;//�ҵ��߽���˳�
        }
    }
}
/*
------------------------------------------------------------------------------------------------------------------
�������     ����е�λ�ã������Ǵӵ����������ĸ�
����˵��     ��
���ز���     uint16���е�ĺ�����
ʹ��ʾ��     
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/
uint16 output_middle(void)
{
    return centerline[MT9V03X_H-4];
}


bool stop_flag(void)
{
    int count ;
    for(int i= 0;i<MT9V03X_H;i++)
    {
        for(int j=0;j<MT9V03X_W;j++)
        {
            if(mt9v03x_image[i][j]<30)
            {
                    count++;
                
            }
        }
    }
    if(count>MT9V03X_H*MT9V03X_W)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//Բ���ж�
//1.��Բ��
/*
------------------------------------------------------------------------------------------------------------------
�������     ���Ҳ�Ľǵ�
����˵��     uint8�������յ�
���ز���     ���ؽǵ������int����
ʹ��ʾ��     
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/
//���Ҳ�ǵ�
int find_pointright(uint8 start,uint8 end)
{
    right_down_line=0;
    if(start>MT9V03X_H-6)
    {
        start=MT9V03X_H-6;
    }
    if(end<5){end=5;}
    if(start<end)
    {
        uint8 t=start;
        start=end;
        end=t;
    }
    for(int i=start;i>=end;i--)
    {
        if(right_down_line==0&&abs(rightline[i]
            -rightline[i+1]<=5)&&rightline[i]-rightline[i-2]
        <=-10&&rightline[i]-rightline[i-3]<=-10)
        {
            right_down_line=i;
            break;
        }
    }
//    printf("find_pointright%d,",right_down_line);
    return right_down_line;                                                         //��i���нǵ�
}
/*
------------------------------------------------------------------------------------------------------------------
�������     ȷ�������ԣ����ڴ����Ҳ��ߡ�
����˵��     uint8�����յ�
���ز���     �е�λ��
ʹ��ʾ��     
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/
//ȷ�������Ժ������Ҳࣩ
int continuity_right(uint8 start,uint8 end)
{
    int i;
    int continuity_change_flag=0;
    if(start>=MT9V03X_H-5)//����Խ�籣��
        start=MT9V03X_H-5;
    if(end<=5)
    {
        end=5;
    }
        if(start<end)
    {
        uint8 t=start;
        start=end;
        end=t;
    }
    for(i=start;i>=end;i--)
    {
        if(abs(rightline[i]-rightline[i-1])>=5)//��������ֵ��5���ɸ���
       {
            continuity_change_flag=i;                                         //��i����������
            break;
       }

    }
    //    printf("continuity_right%d,\n",continuity_change_flag);

    return continuity_change_flag;
}
/*
------------------------------------------------------------------------------------------------------------------
�������     ȷ�������ԣ����ڴ�������ߡ�
����˵��     uint8�����յ�
���ز���     �е�λ��
ʹ��ʾ��     
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/
int continuity_left(uint8 start,uint8 end)
{
    int i;
    int continuity_change_flag=0;
    if(start>=MT9V03X_H-5)//����Խ�籣��
        start=MT9V03X_H-5;
    if(end<=5)
    {
        end=5;
    }
        if(start<end)
    {
        uint8 t=start;
        start=end;
        end=t;
    }
    for(i=start;i>=end;i--)
    {
        if(abs(leftline[i]-leftline[i-1])>=5)//��������ֵ��5���ɸ���
       {
            continuity_change_flag=i;                                         //��i����������
            break;
       }

    }
    //    printf("continuity_left%d,\n",continuity_change_flag);

    return continuity_change_flag;
}
//�����Ա仯
int montonicity_right(uint8 start,uint8 end)
{
    int i;
    int result=0;
    int montonicity_add=0;//������
    int montonicity_sub=0;//������
    if(start>=MT9V03X_H-5)//����Խ�籣��
        start=MT9V03X_H-5;
    if(end<=5)
    {
        end=5;
    }
        if(start<end)
    {
        uint8 t=start;
        start=end;
        end=t;
    }
    uint8 mark1last=0;//����ĩβ
    uint8 mark2first=0;//������ʼ
    for(i=start;i>=end;i--)
    {
        if((rightline[i]-rightline[i-1])>0)//����������������ֵ��5���ɸ���
       {
           montonicity_add++;
           mark1last=i;
       }
        if((rightline[i]-rightline[i-1])<0)//��������
       {
            montonicity_sub++;
            if(mark2first==0){mark2first=i;}
       }

        if(montonicity_add>5&&montonicity_sub>5)
        {

            result= (mark1last+mark2first)/2;                                   //��i�������Կ�ʼ�仯
        }
    }
    return result;

}


//�ж�Բ��������������Ҳ಻�������Ҳ��нǵ㣬�Ҳ��е����Ա仯��
/*
------------------------------------------------------------------------------------------------------------------
�������     ������
����˵��     ���x��y�յ�y��������dx
���ز���     ��
ʹ��ʾ��     
��ע��Ϣ     ������dx����x1-x2/y1-y2
-------------------------------------------------------------------------------------------------------------------
*/
void draw_Lline_k(uint16 startx, uint16 starty, uint16 endy, float dx) {
    if (dx == 0) {
        for (int i = (starty< endy)?starty:endy; i <=(starty>endy)?starty:endy; i++) {
            leftfollowline[i] = startx;
        }
        return;
    }
    int step = (starty < endy) ? 1 : -1;
    for (int i = starty; i != endy; i += step) {
        leftfollowline[i] = startx + (uint16)((i - starty) * dx + 0.5f);
    }
}
/*
------------------------------------------------------------------------------------------------------------------
�������     ������
����˵��     ���x��y�յ�y��������dx
���ز���     ��  
ʹ��ʾ��     
��ע��Ϣ     ������dx����x1-x2/y1-y2
-------------------------------------------------------------------------------------------------------------------
*/
void draw_Rline_k(uint16 startx, uint16 starty, uint16 endy, float dx) {


    if (dx == 0) {
        for (int i = (starty< endy)?starty:endy; i <=(starty>endy)?starty:endy; i++) {
            rightfollowline[i] = startx;
        }
        return;
    }
    int step = (starty < endy) ? 1 : -1;
    for (int i = starty; i != endy; i += step) {
        rightfollowline[i] = startx + (uint16)((i - starty) * dx + 0.5f); 
    }
}