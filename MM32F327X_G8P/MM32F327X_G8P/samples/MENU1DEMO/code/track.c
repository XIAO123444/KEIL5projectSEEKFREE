/*
 * track.c
 * 赛道识别与车辆状态控制模块
 * 创建日期：2023年10月24日
 * 作者：lychee
 */
#include "track.h"
#include "photo_chuli.h"
#include "buzzer.h"
// 外部变量声明
extern int16 centerline[MT9V03X_H];      // 中心线数组（图像高度维度）
extern int16 leftline[MT9V03X_H];       // 左边界线数组
extern int16 rightline[MT9V03X_H];      // 右边界线数组
extern uint8 pix_per_meter;             // 像素/米比例系数
extern int16 rightfollowline[MT9V03X_H]; // 右边界跟踪线
extern int16 leftfollowline[MT9V03X_H];  // 左边界跟踪线
int16 centerline2[MT9V03X_H];           // 二次计算的中心线

// 边界点检测结果
extern int16 Right_Down_Find;  // 右下边界点行号
extern int16 Left_Down_Find;   // 左下边界点行号
extern int16 Right_Up_Find;    // 右上边界点行号
extern int16 Left_Up_Find;     // 左上边界点行号

// 特殊场景标志
int16 right_down_guai = 0;    // 右下拐点标志（用于圆环检测）
int16 left_up_guai = 0;       // 左上拐点标志（保留未使用）


int16 output_middle2(void) {
    return centerline2[MT9V03X_H-5];
}

int16 trackline[MT9V03X_H];  // 赛道线缓存（未使用）


enum mark {
    straight,    // 直道行驶
    crossroad,   // 十字路口
    round_pre,   // 圆环预识别
    round_now,   // 圆环内行驶（未使用）
    round_out    // 圆环出口（未使用）
};
enum mark carstatus_now = straight;  // 当前车辆状态


void centerline2_change(void) {
    for(int16 i=0; i<MT9V03X_H; i++) {
        centerline2[i] = (rightfollowline[i] + leftfollowline[i]) / 2;
    }
}


void element_check(void) {    
    // 更新左右跟踪线
    memcpy(leftfollowline, leftline, sizeof(leftline));
    memcpy(rightfollowline, rightline, sizeof(rightline));
    centerline2_change();

    /*---------- 直道状态检测 ----------*/
    if(carstatus_now == straight) {
        // 十字路口检测条件：左右边界均不连续
        if(continuity_left(40, MT9V03X_H-5) > 0 && 
           continuity_right(40, MT9V03X_H-5) > 0) {
            
            // 查找边界突变点（从上往下扫描）
            Find_Up_Point(MT9V03X_H-5, 40);
            
            // 未找到突变点则退出
            if(Left_Up_Find == 0 && Right_Up_Find == 0) return;
            
            // 同时检测到左右突变点则判定为十字路口
            if(Left_Up_Find != 0 && Right_Up_Find != 0) {
                carstatus_now = crossroad;
                BUZZ_cycle();
                return;
            }
        }
        
        // 圆环预检测条件：左边界不连续且右下存在拐点
        if(continuity_left(40, MT9V03X_H-5) > 0 && 
           Find_Right_Down_Point(MT9V03X_H-5, 80)) {
            carstatus_now = round_pre;
            return;
        }
    }

    /*---------- 十字路口状态处理 ----------*/
    if(carstatus_now == crossroad) {
        // 重新扫描边界突变点（从下往上）
        Find_Up_Point(40, MT9V03X_H-5);
        
        // 确定扫描起点（取左右突变点的较高位置）
        int start_down_point = (Right_Up_Find < Left_Up_Find) ? Right_Up_Find : Left_Up_Find;
        
        // 查找下半段边界点
        Find_Down_Point(MT9V03X_H-5, start_down_point);
        
        // 校验下点位置有效性（必须低于上点）
        if(Left_Down_Find <= Left_Up_Find) Left_Down_Find = 0;
        if(Right_Down_Find <= Right_Up_Find) Right_Down_Find = 0;

        /* 边界线拟合策略 */
        if(Left_Down_Find != 0 && Right_Down_Find != 0) {
            // 情况1：左右下点均有效 → 双边界直线拟合
            add_Rline_k(rightline[Right_Down_Find], Right_Down_Find, 
                       Right_Up_Find, rightline[Right_Up_Find]);
            add_Lline_k(leftline[Left_Down_Find], Left_Down_Find, 
                       Left_Up_Find, leftline[Left_Up_Find]);
        }
        else if(Left_Down_Find == 0 && Right_Down_Find != 0) {
            // 情况2：仅右下点有效 → 右边界拟合+左边界延长
            add_Rline_k(rightline[Right_Down_Find], Right_Down_Find, 
                       Right_Up_Find, rightline[Right_Up_Find]);
            lenthen_Left_bondarise(Left_Up_Find);
        }
        else if(Left_Down_Find != 0 && Right_Down_Find == 0) {
            // 情况3：仅左下点有效 → 左边界拟合+右边界延长
            lenthen_Right_bondarise(Right_Up_Find);
            add_Lline_k(leftline[Left_Down_Find], Left_Down_Find, 
                       Left_Up_Find, leftline[Left_Up_Find]);
        }
        else {
            // 情况4：无有效下点 → 双边界延长
            lenthen_Right_bondarise(Right_Up_Find);
            lenthen_Left_bondarise(Left_Up_Find);
        }

        // 异常处理：突变点失效时恢复原始边界
        if(Right_Up_Find == 0) memcpy(rightfollowline, rightline, sizeof(rightline));
        if(Left_Up_Find == 0) memcpy(leftfollowline, leftline, sizeof(leftline));
        
        // 突变点全部失效时返回直道状态
        if(Right_Up_Find == 0 && Left_Up_Find == 0) {
            carstatus_now = straight;
        }
        
        centerline2_change();
    }

    /*---------- 圆环预识别状态处理 ----------*/
    if(carstatus_now == round_pre) {
        right_down_guai = Find_Right_Down_Point(MT9V03X_H-5, 80);
        int zhongduandianzuo = continuity_left(MT9V03X_H-5, 80);
        int bulianxudian = montonicity_right(MT9V03X_H-5, 80);
        
        // 圆环确认条件：左边界中断+右下拐点+右边界非单调
        if(zhongduandianzuo == 0 && right_down_guai && bulianxudian) {
            // 右边界拐点拟合
            add_Rline_k(rightfollowline[bulianxudian], bulianxudian, 
                       right_down_guai, rightfollowline[right_down_guai]);
        }
    }
}


void choose_tracktype(void) {
    // 待实现功能：根据赛道特征选择跟踪策略
    // track_type = TRACK_LEFT;
}