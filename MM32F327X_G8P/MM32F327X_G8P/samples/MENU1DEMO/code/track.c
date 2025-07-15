/*
 * track.c
 *  Created on: 2023年10月24日
 *      Author: lychee
 */
#include "track.h"
#include "photo_chuli.h"
extern int16 centerline[MT9V03X_H];
extern int16 leftline[MT9V03X_H];
extern int16 rightline[MT9V03X_H];
extern uint8 pix_per_meter;//每米的像素数
extern int16 rightfollowline[MT9V03X_H];
extern int16 leftfollowline[MT9V03X_H];
int16 centerline2[MT9V03X_H];


extern int16 Right_Down_Find;
extern int16 Left_Down_Find;
extern int16 Right_Up_Find;
extern int16 Left_Up_Find;
int16 right_down_guai=0; //圆环用
int16 left_up_guai=0;



int16 output_middle2(void)
{

    return centerline2[MT9V03X_H-5];
}

int16 trackline[MT9V03X_H];//跟踪线
enum mark
{
    straight,
    crossroad,
    round_pre,
    round_now,
    round_out
    
};
enum mark carstatus_now=straight;
void centerline2_change(void)
{
    for(int16 i=0;i<MT9V03X_H;i++)
    {
        centerline2[i]=(rightfollowline[i]+leftfollowline[i])/2;
    }
}
void element_check(void)
{    

    memcpy(leftfollowline,leftline,sizeof(leftline));
    memcpy(rightfollowline,rightline,sizeof(rightline));
    centerline2_change();

    if(carstatus_now==straight)
    {
        //十字路口判断
        if(continuity_left(40,MT9V03X_H-5)>0&& continuity_right(40,MT9V03X_H-5)>0)//左侧不连续，右侧不连续
        {
            Find_Up_Point( MT9V03X_H-5, 40 );
            if(Left_Up_Find==0&&Right_Up_Find==0)
            {
                return;//这里没问题
                
            }
            if(Left_Up_Find!=0&&Right_Up_Find!=0)
            {    

                carstatus_now=crossroad ;
                return;
            }
        }
        
        //圆环判断
        if(continuity_left(40,MT9V03X_H-5)>0&&Find_Right_Down_Point(MT9V03X_H-5,80))//左侧连续，右侧有下拐点
        {
             carstatus_now=round_pre;
            return;
        }
        
    }
    if(carstatus_now==crossroad)
    {            

        Find_Up_Point(40 ,MT9V03X_H-5  );


        int start_down_point=(Right_Up_Find<Left_Up_Find)?Right_Up_Find:Left_Up_Find;

        Find_Down_Point(MT9V03X_H-5,start_down_point);
        if(Left_Down_Find<=Left_Up_Find)
        {
            Left_Down_Find=0;//下点不可能比上点 还靠上
        }
        if(Right_Down_Find<=Right_Up_Find)
        {
            Right_Down_Find=0;//下点不可能比上点还靠上
        }

        if(Left_Down_Find!=0&&Right_Down_Find!=0)
        {
            add_Rline_k(rightline[Right_Down_Find],Right_Down_Find,Right_Up_Find,rightline[Right_Up_Find]);
            add_Lline_k(leftline[Left_Down_Find],Left_Down_Find,Left_Up_Find,leftline[Left_Up_Find]);
//        ips200_show_int(60,280,11,2);


        }
        else if(Left_Down_Find==0&&Right_Down_Find!=0)
        {
            add_Rline_k(rightline[Right_Down_Find],Right_Down_Find,Right_Up_Find,rightline[Right_Up_Find]);
            lenthen_Left_bondarise(Left_Up_Find);

        }
        else if(Left_Down_Find!=0&&Right_Down_Find==0)
        {
            lenthen_Right_bondarise(Right_Up_Find);
            add_Lline_k(leftline[Left_Down_Find],Left_Down_Find,Left_Up_Find,leftline[Left_Up_Find]);

        }
        else if(Left_Down_Find==0&&Right_Down_Find==0)
        {

            lenthen_Right_bondarise(Right_Up_Find);
            lenthen_Left_bondarise(Left_Up_Find);


        }
        if(Right_Up_Find==0)
        {
            memcpy(rightfollowline,rightline,sizeof(rightline));

        }
        if(Left_Up_Find==0)
        {
            memcpy(leftfollowline,leftline,sizeof(leftline));

        }
        if(Right_Up_Find==0&&Left_Up_Find==0)
        {
            carstatus_now=straight;
        }
        centerline2_change();
        ips200_show_int(40,300,centerline2[MT9V03X_H-5],3);
    }
    if(carstatus_now==round_pre)
    {
            //补线思路：斜率对称法
        right_down_guai=Find_Right_Down_Point(MT9V03X_H-5,80);
        int zhongduandianzuo =continuity_left(MT9V03X_H-5,80);
        int bulianxudian=montonicity_right(MT9V03X_H-5,80);
        if(zhongduandianzuo==0&&right_down_guai&&bulianxudian)//左端连续，右端同时出现拐点和不连续点
        {
//          float dx3=(float)(rightfollowline[right_down_guai]-rightfollowline[bulianxudian])/(float)(right_down_guai-bulianxudian);
            add_Rline_k(rightfollowline[bulianxudian],bulianxudian,right_down_guai,rightfollowline[right_down_guai]);
        }
//        else if()//左端连续，拐点消失且不连续点任然在
    }
}





//选择如何循线，大家可以自由发挥
void choose_tracktype(void){
    //track_type = TRACK_LEFT;
}
