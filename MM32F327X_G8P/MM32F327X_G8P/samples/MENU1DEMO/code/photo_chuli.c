/*
 * camera.c
 *
 *  Created on: 2023��10��24��
 *      Author: lychee
 */
#include "photo_chuli.h"
#include "math.h"
int16 centerline[MT9V03X_H];
int16 leftline[MT9V03X_H];
int16 rightline[MT9V03X_H];
int16 rightfollowline[MT9V03X_H];
int16 leftfollowline[MT9V03X_H];

int16 Right_Down_Find=0;
int16 Left_Down_Find=0;
int16 Right_Up_Find=0;
int16 Left_Up_Find=0;

uint8 leftline_num;//���ߵ�����
uint8 rightline_num;//���ߵ�����
int16 sar_thre = 17;//��Ⱥ���ֵ
uint8 pix_per_meter = 20;//ÿ�׵�������
extern bool stop_flag1;


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
    uint8 mov = 3;//ÿ���������ƶ���,Ĭ��Ϊ2�����Ը��ݻ���ֱ��ʵ���
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
    uint8 mov = 3;//ÿ���������ƶ���,Ĭ��Ϊ2�����Ը��ݻ���ֱ��ʵ���
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
void banmaxian_check(void)
{
    int sum =0;
    for(int i=94;i>5;i--)
    {
        if(mt9v03x_image[ MT9V03X_H - 5][i]<80)
        {
            sum++;
        }
        if (sum>MT9V03X_W*0.8)
        {
            stop_flag1=true;
        }
    }
}
/*
------------------------------------------------------------------------------------------------------------------
�������     ����е�λ�ã������Ǵӵ����������ĸ�
����˵��     ��
���ز���     int16���е�ĺ�����
ʹ��ʾ��     
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/
int16 output_middle(void)
{
    return centerline[MT9V03X_H-4];
}


bool stop_flag(void)
{
    if(leftline_num<0.1*MT9V03X_H   &&rightline_num<0.1*MT9V03X_H)
    {
        return true;
    }
    else return false;
}
//ʮ���ж�
//ʮ���ж�
//ʮ���ж�
//ʮ���ж�
//ʮ���ж�
//ʮ���ж�


/*-------------------------------------------------------------------------------------------------------------------
  @brief     ������������յ㣬��ʮ��ʹ��
  @param     �����ķ�Χ��㣬�յ�
  @return    �޸�����ȫ�ֱ���
             Right_Down_Find=0;
             Left_Down_Find=0;
  Sample     Find_Down_Point(int start,int end)
  @note      ������֮��鿴��Ӧ�ı�����ע�⣬û�ҵ�ʱ��Ӧ��������0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Down_Point(int start,int end)
{
    int i,t;
    Right_Down_Find=0;
    Left_Down_Find=0;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_H-1-5)//����5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start=MT9V03X_H-1-5;
    if(end<=MT9V03X_H-10)
        end=MT9V03X_H-10;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(Left_Down_Find==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(leftline[i]-leftline[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(leftline[i+1]-leftline[i+2])<=5&&
           abs(leftline[i+2]-leftline[i+3])<=5&&
              (leftline[i]-leftline[i-2])>=8&&
              (leftline[i]-leftline[i-3])>=15&&
              (leftline[i]-leftline[i-4])>=15)
        {
            Left_Down_Find=i;//��ȡ��������
        }
        if(Right_Down_Find==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(rightline[i]-rightline[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(rightline[i+1]-rightline[i+2])<=5&&
           abs(rightline[i+2]-rightline[i+3])<=5&&
              (rightline[i]-rightline[i-2])<=-8&&
              (rightline[i]-rightline[i-3])<=-15&&
              (rightline[i]-rightline[i-4])<=-15)
        {
            Right_Down_Find=i;
        }
        if(Left_Down_Find!=0&&Right_Down_Find!=0)//�����ҵ����˳�
        {
            break;
        }
    }
}
 
/*-------------------------------------------------------------------------------------------------------------------
  @brief     ������������յ㣬��ʮ��ʹ��
  @param     �����ķ�Χ��㣬�յ�
  @return    �޸�����ȫ�ֱ���
             Left_Up_Find=0;
             Right_Up_Find=0;
  Sample     Find_Up_Point(int start,int end)
  @note      ������֮��鿴��Ӧ�ı�����ע�⣬û�ҵ�ʱ��Ӧ��������0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Up_Point(int start,int end)
{
    int i,t;
    Left_Up_Find=0;
    Right_Up_Find=0;
    if(start>end)
    {
        t=start;
        start=end;
        end=t;
    }
    //start<end��������
    if(end>=MT9V03X_H-5)
        end=MT9V03X_H-5;
//    if(end<=5)//��ʱ����зǳ�����ҲҪ�������ֵ㣬��ֹ����Խ��
//        end=5;
    if(start<=40)//����5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start=40;
    for(i=start;i<=end;i++)
    {
        if(Left_Up_Find==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(leftline[i]-leftline[i-1])<=5&&
           abs(leftline[i-1]-leftline[i-2])<=5&&
           abs(leftline[i-2]-leftline[i-3])<=5&&
              (leftline[i]-leftline[i+2])>=8&&
              (leftline[i]-leftline[i+3])>=15&&
              (leftline[i]-leftline[i+4])>=15)
        {
            Left_Up_Find=i;//��ȡ��������
            ips200_show_int(0,280,Left_Up_Find,8);


        }
        if(Right_Up_Find==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(rightline[i]-rightline[i-1])<=5&&//��������λ�ò��
           abs(rightline[i-1]-rightline[i-2])<=5&&
           abs(rightline[i-2]-rightline[i-3])<=5&&
              (rightline[i]-rightline[i+2])<=-8&&
              (rightline[i]-rightline[i+3])<=-15&&
              (rightline[i]-rightline[i+4])<=-15)
        {
            Right_Up_Find=i;//��ȡ��������

        }
        if(Left_Up_Find!=0&&Right_Up_Find!=0)//���������ҵ��ͳ�ȥ
        {
            break;
        }
    }
    if(abs(Right_Up_Find-Left_Up_Find)>=50)//����˺�ѹ�����Ϊ����
    {
        Right_Up_Find=0;
        Left_Up_Find=0;
    }
}


//Բ���ж�
//1.��Բ��

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���½ǵ���
  @param     ��ʼ�У���ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Down_Point(int start,int end)//�����½ǵ㣬����ֵ�ǽǵ����ڵ�����
 {
    int i,t;
    int left_down_line=0;
    if(leftline_num<=0.1*MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
       return left_down_line;
    if(start<end)//--���ʣ�Ҫ��֤start>end
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_H-1-5)//����5������5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start=MT9V03X_H-1-5;//��һ���棬���жϵ�i��ʱ������ʵ�i+3��i-4�У���ֹԽ��
    if(end<=MT9V03X_H-5)
        end=MT9V03X_H-5;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(left_down_line==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(leftline[i]-leftline[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(leftline[i+1]-leftline[i+2])<=5&&  
           abs(leftline[i+2]-leftline[i+3])<=5&&
              (leftline[i]-leftline[i-2])>=5&&
              (leftline[i]-leftline[i-3])>=10&&
              (leftline[i]-leftline[i-4])>=10)
        {
            left_down_line=i;//��ȡ��������
            break;
        }
    }
    return left_down_line;
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���½ǵ���
  @param     ��ʼ�У���ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     right_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Down_Point(uint8 start,uint8 end)
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
        if(right_down_line==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(rightline[i]-rightline[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(rightline[i+1]-rightline[i+2])<=5&&  
           abs(rightline[i+2]-rightline[i+3])<=5&&
              (rightline[i]-rightline[i-2])>=5&&
              (rightline[i]-rightline[i-3])>=10&&
              (rightline[i]-rightline[i-4])>=10)
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
        if(abs(rightline[i]-rightline[i-1])>=7)//��������ֵ��5���ɸ���
       {
            continuity_change_flag=i;                                         //��i����������

            break;
       }

    }
    //    printf("continuity_right%d,\n",continuity_change_flag);

    return continuity_change_flag-end-10;
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
//ȷ�������Ժ�������ࣩ

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
        if(abs(leftline[i]-leftline[i-1])>=7)//��������ֵ��5���ɸ���
       {

            continuity_change_flag=i;                                         //��i����������

            break;
       }

    }
    //    printf("continuity_left%d,\n",continuity_change_flag);
    return continuity_change_flag-end-10;
}
//�����Ա仯s
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


/*
------------------------------------------------------------------------------------------------------------------
�������     ������
����˵��     ���x��y�յ�y��������dx
���ز���     ��
ʹ��ʾ��     
��ע��Ϣ     ������dx����x1-x2/y1-y2
-------------------------------------------------------------------------------------------------------------------
*/
void draw_Lline_k(int16 startx, int16 starty, int16 endy, float dx) {
    if (dx == 0) {
        for (int i = (starty< endy)?starty:endy; i <=(starty>endy)?starty:endy; i++) {
            leftfollowline[i] = startx;
        }
        return;
    }
    int step = (starty < endy) ? 1 : -1;
    for (int i = starty; i != endy; i += step) {
        leftfollowline[i] = startx + (int16)((i - starty) * dx + 0.5f);
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
void draw_Rline_k(int16 startx, int16 starty, int16 endy, float dx) {


    if (dx == 0) 
    {
        for (int i = (starty< endy)?starty:endy; i <=(starty>endy)?starty:endy; i++) 
        {
            rightfollowline[i] = startx;
        }
        return;
    }
    int step = (starty < endy) ? 1 : -1;
    for (int i = starty; i != endy; i += step) {
        rightfollowline[i] = startx + (int16)((i - starty) * dx + 0.5f); 
    }
}
/*
------------------------------------------------------------------------------------------------------------------
�������     �����������
����˵��     ���xy���յ�xy
���ز���     ��
ʹ��ʾ��     
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/
void add_Rline_k(int16 startx, int16 starty, int16 endy,int16 endx)
{
    float dx=(float)(endx-startx)/(float)(endy-starty);
    draw_Rline_k(startx,starty,endy,dx);
}
/*
------------------------------------------------------------------------------------------------------------------
�������     �����������
����˵��     ���xy���յ�xy
���ز���     ��
ʹ��ʾ��     
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/
void add_Lline_k(int16 startx, int16 starty, int16 endy,int16 endx)
{
    float dx=(float)(endx-startx)/(float)(endy-starty);
    draw_Lline_k(startx,starty,endy,dx);
}
/*
------------------------------------------------------------------------------------------------------------------
�������     ���϶��²�����
����˵��     ���
���ز���     ��
ʹ��ʾ��     
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/
void lenthen_Left_bondarise(int16 start)
{
    if(start<7){start=7;}
    if(start>MT9V03X_H-7){start=MT9V03X_H-7;}
    float dx=(float)(leftline[start]-leftline[start-7])/7;
    for(int i=start;i<MT9V03X_H-1;i++)
    {
        if((float)leftline[start]+(float)(dx*(i-start))<0||(float)leftline[start]+dx*(float)(i-start)>(float)MT9V03X_W)
        {
            break;
        }
        else
        {
            leftfollowline[i]=(int)((float)leftline[start]+dx*(float)(i-start));
        }
    }
}
 /*
------------------------------------------------------------------------------------------------------------------
�������     ���϶��²�����
����˵��     ���
���ز���     ��
ʹ��ʾ��     
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/  
void lenthen_Right_bondarise(int16 start)
{
    if(start<7){start=7;}
    if(start>MT9V03X_H-7){start=MT9V03X_H-7;}
    float dx=(float)(rightline[start]-rightline[start-7])/7;
    for(int i=start;i<MT9V03X_H-1;i++)
    {
        if((float)rightline[start]+dx*(i-start)<MT9V03X_W-5||(float)rightline[start]+dx*(float)(i-start)<0)
        {
            break;
        }
        else
        {
            rightfollowline[i]=(int)((float)rightline[start]+dx*(float)(i-start));
        }
    }
}
   