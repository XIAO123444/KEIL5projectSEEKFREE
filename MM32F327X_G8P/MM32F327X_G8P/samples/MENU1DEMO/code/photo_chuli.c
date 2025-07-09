/*
 * camera.c
 *
 *  Created on: 2023年10月24日
 *      Author: lychee
 */
#include "photo_chuli.h"
#include "math.h"
uint16 centerline[MT9V03X_H];
uint16 leftline[MT9V03X_H];
uint16 rightline[MT9V03X_H];
uint16 rightfollowline[MT9V03X_H];
uint16 leftfollowline[MT9V03X_H];



uint8 leftline_num;//左线点数量
uint8 rightline_num;//右线点数量
uint16 sar_thre = 17;//差比和阈值
uint8 pix_per_meter = 20;//每米的像素数

int right_down_line =0;
//逐行寻找边界点
void image_boundary_process(void){
    uint8 row;//行
    //uint8 col = MT9V03X_W/2;//列
    uint8 start_col = MT9V03X_W / 2;//各行起点的列坐标,默认为MT9V03X_W / 2
    //清零之前的计数
    leftline_num = 0;
    rightline_num = 0;

    for(row = MT9V03X_H - 1; row >= 1; row--){
        //选用上一行的中点作为下一行计算起始点，节省速度，同时防止弯道的左右两边均出现与画面一侧
        if(row != MT9V03X_H - 1){
            start_col = (uint8)(0.4 * centerline[row] + 0.3 * start_col + 0.1 * MT9V03X_W);//一阶低通滤波，防止出现噪点影响下一行的起始点
        }
        else if(row == MT9V03X_H - 1){
            start_col = MT9V03X_W / 2;
        }
        //逐行作差比和 
        difsum_left(row,start_col);
        difsum_right(row,start_col);
        centerline[row] = 0.5 * (rightline[row] + leftline[row]);
    }
}
//差比和寻找左侧边界点
void difsum_left(uint8 y,uint8 x){
    float sum,dif,sar;//和，差，比
    uint8 col;//列
    uint8 mov = 2;//每次作差后的移动量,默认为2，可以根据画面分辨率调整
    //计算第x行的左边界
    leftline[y] = 0;//未找到左边界时输出为0
    for(col = x; col >= mov + 1; col -= mov){
        dif = (float)((mt9v03x_image[y][col] - mt9v03x_image[y][col - mov - 1])<<8);//左移8位即乘256，可避免浮点数乘，加快速度
        sum = (float)((mt9v03x_image[y][col] + mt9v03x_image[y][col - mov - 1]));
        sar = fabs(dif / sum);//求取差比和
        if(sar > sar_thre){//差比和大于阈值代表深浅色突变
            leftline[y] = (int16)(col - mov);
            leftline_num ++;//左线点计数+
            break;//找到边界后退出
        }
    }
}
//差比和寻找右侧边界点
void difsum_right(uint8 y,uint8 x){
    float sum,dif,sar;//和，差，比
    uint8 col;//列
    uint8 mov = 2;//每次作差后的移动量,默认为2，可以根据画面分辨率调整
    //计算第x行的左边界
    rightline[y] = MT9V03X_W - 1;//未找到右边界时输出为187
    for(col = x; col <= MT9V03X_W - mov - 1; col += mov){
        dif = (float)((mt9v03x_image[y][col] - mt9v03x_image[y][col + mov + 1])<<8);//左移8位即乘256，可避免浮点数乘，加快速度
        sum = (float)((mt9v03x_image[y][col] + mt9v03x_image[y][col + mov + 1]));
        sar = fabs(dif / sum);//求取差比和
        if(sar > sar_thre){//差比和大于阈值代表深浅色突变
            rightline[y] = (int16)(col + mov);
            rightline_num ++;//右线点计数+
            break;//找到边界后退出
        }
    }
}
/*
------------------------------------------------------------------------------------------------------------------
函数简介     输出中点位置：这里是从底向上数第四个
参数说明     无
返回参数     uint16，中点的横坐标
使用示例     
备注信息     
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


//圆环判断
//1.找圆环
/*
------------------------------------------------------------------------------------------------------------------
函数简介     找右侧的角点
参数说明     uint8的起点和终点
返回参数     返回角点横坐标int类型
使用示例     
备注信息     
-------------------------------------------------------------------------------------------------------------------
*/
//找右侧角点
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
    return right_down_line;                                                         //在i处有角点
}
/*
------------------------------------------------------------------------------------------------------------------
函数简介     确定连续性，用于处理右侧线。
参数说明     uint8起点和终点
返回参数     中点位置
使用示例     
备注信息     
-------------------------------------------------------------------------------------------------------------------
*/
//确定连续性函数（右侧）
int continuity_right(uint8 start,uint8 end)
{
    int i;
    int continuity_change_flag=0;
    if(start>=MT9V03X_H-5)//数组越界保护
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
        if(abs(rightline[i]-rightline[i-1])>=5)//连续性阈值是5，可更改
       {
            continuity_change_flag=i;                                         //在i处不连续了
            break;
       }

    }
    //    printf("continuity_right%d,\n",continuity_change_flag);

    return continuity_change_flag;
}
/*
------------------------------------------------------------------------------------------------------------------
函数简介     确定连续性，用于处理左侧线。
参数说明     uint8起点和终点
返回参数     中点位置
使用示例     
备注信息     
-------------------------------------------------------------------------------------------------------------------
*/
int continuity_left(uint8 start,uint8 end)
{
    int i;
    int continuity_change_flag=0;
    if(start>=MT9V03X_H-5)//数组越界保护
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
        if(abs(leftline[i]-leftline[i-1])>=5)//连续性阈值是5，可更改
       {
            continuity_change_flag=i;                                         //在i处不连续了
            break;
       }

    }
    //    printf("continuity_left%d,\n",continuity_change_flag);

    return continuity_change_flag;
}
//单调性变化
int montonicity_right(uint8 start,uint8 end)
{
    int i;
    int result=0;
    int montonicity_add=0;//单调增
    int montonicity_sub=0;//单调减
    if(start>=MT9V03X_H-5)//数组越界保护
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
    uint8 mark1last=0;//左增末尾
    uint8 mark2first=0;//右增起始
    for(i=start;i>=end;i--)
    {
        if((rightline[i]-rightline[i-1])>0)//向上左增连续性阈值是5，可更改
       {
           montonicity_add++;
           mark1last=i;
       }
        if((rightline[i]-rightline[i-1])<0)//向上右增
       {
            montonicity_sub++;
            if(mark2first==0){mark2first=i;}
       }

        if(montonicity_add>5&&montonicity_sub>5)
        {

            result= (mark1last+mark2first)/2;                                   //在i处单调性开始变化
        }
    }
    return result;

}


//判断圆环？左侧连续，右侧不连续，右侧有角点，右侧有单调性变化。
/*
------------------------------------------------------------------------------------------------------------------
函数简介     补线左
参数说明     起点x，y终点y，增长率dx
返回参数     无
使用示例     
备注信息     增长率dx等于x1-x2/y1-y2
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
函数简介     补线右
参数说明     起点x，y终点y，增长率dx
返回参数     无  
使用示例     
备注信息     增长率dx等于x1-x2/y1-y2
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