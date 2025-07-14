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





int16 output_middle2(void)
{

    return centerline2[5];
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
        centerline2[i]=(rightfollowline[i]+leftfollowline[i])*0.5;
    }
}
float dx1=0;float dx2=0;
void element_check(void)
{    

    memcpy(leftfollowline,leftline,sizeof(leftline));
    memcpy(rightfollowline,rightline,sizeof(rightline));
    if(carstatus_now==straight)
    {
        if(continuity_left(40,MT9V03X_H-5)>0&& continuity_right(40,MT9V03X_H-5)>0)//左侧不连续，右侧不连续
        {
            Find_Up_Point( MT9V03X_H-5, 40 );
            if(Left_Up_Find==0&&Right_Up_Find==0)
            {
                return;//这里没问题
                
            }
        }
        if(Left_Up_Find!=0&&Right_Up_Find!=0)
        {    

           carstatus_now=crossroad ;
        }
        centerline2_change();
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
        ips200_show_int(0,280,Right_Down_Find,3);
        ips200_show_int(30,280,Left_Down_Find,3);
        ips200_show_int(0,300,Right_Up_Find,3);
        ips200_show_int(30,300,Left_Up_Find,3);
        if(Left_Down_Find!=0&&Right_Down_Find!=0)
        {
            add_Rline_k(rightline[Right_Down_Find],Right_Down_Find,Right_Up_Find,rightline[Right_Up_Find]);
            add_Lline_k(leftline[Left_Down_Find],Left_Down_Find,Left_Up_Find,leftline[Left_Up_Find]);
        ips200_show_int(60,280,11,2);


        }
        else if(Left_Down_Find==0&&Right_Down_Find!=0)
        {
            add_Rline_k(rightline[Right_Down_Find],Right_Down_Find,Right_Up_Find,rightline[Right_Up_Find]);
            lenthen_Left_bondarise(Left_Up_Find);
                    ips200_show_int(90,280,10,2);

        }
        else if(Left_Down_Find!=0&&Right_Down_Find==0)
        {
            lenthen_Right_bondarise(Right_Up_Find);
            add_Lline_k(leftline[Left_Down_Find],Left_Down_Find,Left_Up_Find,leftline[Left_Up_Find]);
                                ips200_show_int(60,300,1,2);

        }
        else if(Left_Down_Find==0&&Right_Down_Find==0)
        {
//            add_Rline_k(rightline[Right_Down_Find],Right_Down_Find,Right_Up_Find,rightline[Right_Up_Find]);
//            add_Lline_k(leftline[Left_Down_Find],Left_Down_Find,Left_Up_Find,leftline[Left_Up_Find]);
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

    }
}




//将边线赋值给循迹跟踪线
//void switch_mark(void){
//    
//    switch (MARK)
//    {
//        
//    	case straight:
//        memcpy(leftfollowline,leftline,sizeof(leftline));
//        memcpy(rightfollowline,rightline ,sizeof(rightline));
//        dx1=Find_Left_Down_Point(5,MT9V03X_H-1-5);
//    		break;
//        case crossroad_pre:
//            default:
//    		break;
//    }
//}
//选择如何循线，大家可以自由发挥
void choose_tracktype(void){
    //track_type = TRACK_LEFT;
}
