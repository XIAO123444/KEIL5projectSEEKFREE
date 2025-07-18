/*
 * track.c
 * ����ʶ���복��״̬����ģ��
 * �������ڣ�2023��10��24��
 * ���ߣ�lychee
 */
#include "track.h"
#include "photo_chuli.h"
#include "buzzer.h"
// �ⲿ��������
extern int16 centerline[MT9V03X_H];      // ���������飨ͼ��߶�ά�ȣ�
extern int16 leftline[MT9V03X_H];       // ��߽�������
extern int16 rightline[MT9V03X_H];      // �ұ߽�������
extern uint8 pix_per_meter;             // ����/�ױ���ϵ��
extern int16 rightfollowline[MT9V03X_H]; // �ұ߽������
extern int16 leftfollowline[MT9V03X_H];  // ��߽������
int16 centerline2[MT9V03X_H];           // ���μ����������

// �߽������
extern int16 Right_Down_Find;  // ���±߽���к�
extern int16 Left_Down_Find;   // ���±߽���к�
extern int16 Right_Up_Find;    // ���ϱ߽���к�
extern int16 Left_Up_Find;     // ���ϱ߽���к�

// Բ����־
int16 right_down_guai = 0;    // ���¹յ��־������Բ����⣩
int16 right_budandiao=0;
int16 right_up_guai=0;
int16 left_up_guai = 0;       // ���Ϲյ��־������δʹ�ã�


int16 output_middle2(void) {
    return centerline2[MT9V03X_H-60 ];
}
 


enum mark {
    straight,    // ֱ����ʻ
    crossroad,   // ʮ��·��
    round_1,   // Բ��Ԥʶ��
    round_2,   // �뻷��ֱ��
    round_3,   // Բ����б�ߣ�δʹ�ã�
    round_4,   // �뻷��ʻ
    round_5,   // ��յ㲹б��
    round_6    // ������ֱ��
};
enum mark carstatus_now = straight;  // ��ǰ����״̬


void centerline2_change(void) {
    for(int16 i=5; i<MT9V03X_H-5; i++) {
        centerline2[i] = (rightfollowline[i] + leftfollowline[i]) / 2;
    }
}


void element_check(void) {    
    // �������Ҹ�����
    memcpy(leftfollowline, leftline, sizeof(leftline));
    memcpy(rightfollowline, rightline, sizeof(rightline));
    centerline2_change();
    printf("carstatus%d\n",carstatus_now);
    /*---------- ֱ��״̬��� ----------*/
    if(carstatus_now == straight) {
        // ʮ��·�ڼ�����������ұ߽��������
        if(continuity_left(30, MT9V03X_H-5) > 0 && 
           continuity_right(30, MT9V03X_H-5) > 0) {
            
            // ���ұ߽�ͻ��㣨��������ɨ�裩
            Find_Up_Point(MT9V03X_H-5, 10);
            
            // δ�ҵ�ͻ������˳�
            if(Left_Up_Find == 0 && Right_Up_Find == 0) return;
            
            // ͬʱ��⵽����ͻ������ж�Ϊʮ��·��
            if(Left_Up_Find != 0 && Right_Up_Find != 0) {
                carstatus_now = crossroad;
                BUZZ_cycle();
                return;
            }
        }
        
        // Բ��Ԥ�����������߽粻����,���´��ڹյ�,���ϴ��ڲ�������
        if(continuity_left(5, MT9V03X_H-5) > 0 && Find_Right_Down_Point(MT9V03X_H-5, 5)&&montonicity_right(5,MT9V03X_H-5)) {
            carstatus_now = round_1;
            return;
        }
    }

    /*---------- ʮ��·��״̬���� ----------*/
    if(carstatus_now == crossroad) {
//        // ����ɨ��߽�ͻ��㣨�������ϣ�
        Find_Up_Point(5, MT9V03X_H-5);
//        
//        // ȷ��ɨ����㣨ȡ����ͻ���Ľϸ�λ�ã�
        int start_down_point = (Right_Up_Find < Left_Up_Find) ? Right_Up_Find : Left_Up_Find;
//        
//        // �����°�α߽��
        Find_Down_Point(MT9V03X_H-6, start_down_point);
//        
//        // У���µ�λ����Ч�ԣ���������ϵ㣩
        if(Left_Down_Find <= Left_Up_Find) Left_Down_Find = 0;
        if(Right_Down_Find <= Right_Up_Find) Right_Down_Find = 0;

        /* �߽�����ϲ��� */
        if(Left_Down_Find != 0 && Right_Down_Find != 0) {
            // ���1�������µ����Ч �� ˫�߽�ֱ�����
            add_Rline_k(rightline[Right_Down_Find], Right_Down_Find, 
                       Right_Up_Find, rightline[Right_Up_Find]);
            add_Lline_k(leftline[Left_Down_Find], Left_Down_Find, 
                       Left_Up_Find, leftline[Left_Up_Find]);
        }
        else if(Left_Down_Find == 0 && Right_Down_Find != 0) {
            // ���2�������µ���Ч �� �ұ߽����+��߽��ӳ�
            add_Rline_k(rightline[Right_Down_Find], Right_Down_Find, 
                       Right_Up_Find, rightline[Right_Up_Find]);
            lenthen_Left_bondarise(Left_Up_Find);
        }
        else if(Left_Down_Find != 0 && Right_Down_Find == 0) {
            // ���3�������µ���Ч �� ��߽����+�ұ߽��ӳ�
            lenthen_Right_bondarise(Right_Up_Find);
            add_Lline_k(leftline[Left_Down_Find], Left_Down_Find, 
                       Left_Up_Find, leftline[Left_Up_Find]);
        }
        else {
            // ���4������Ч�µ� �� ˫�߽��ӳ�
            lenthen_Right_bondarise(Right_Up_Find);
            lenthen_Left_bondarise(Left_Up_Find);
        }

        // �쳣����ͻ���ʧЧʱ�ָ�ԭʼ�߽�
        if(Right_Up_Find == 0) memcpy(rightfollowline, rightline, sizeof(rightline));
        if(Left_Up_Find == 0) memcpy(leftfollowline, leftline, sizeof(leftline));
        centerline2_change();

        // ͻ���ȫ��ʧЧʱ����ֱ��״̬
        if(Right_Up_Find == 0 && Left_Up_Find == 0) {
            carstatus_now = straight;
            return;
        }
        
    }

    /*---------- Բ��Ԥʶ��״̬���� ----------*/
//    if(carstatus_now == round_1) 
//    {
//        right_down_guai = Find_Right_Down_Point(MT9V03X_H-5, 80);
//        int zhongduandianzuo = continuity_left(MT9V03X_H-5, 80);
//        int bulianxudian = montonicity_right(MT9V03X_H-5, 80);
//        
//        // Բ��ȷ����������߽��ж�+���¹յ�+�ұ߽�ǵ���
//        if(zhongduandianzuo == 0 && right_down_guai && bulianxudian) {
//            // �ұ߽�յ����
//            add_Rline_k(rightfollowline[bulianxudian], bulianxudian, 
//                       right_down_guai, rightfollowline[right_down_guai]);
//        }
//        if(right_down_guai==0)
//        {
//             
//        }
//    }
    
} 


void choose_tracktype(void) {
    // ��ʵ�ֹ��ܣ�������������ѡ����ٲ���
    // track_type = TRACK_LEFT;
}