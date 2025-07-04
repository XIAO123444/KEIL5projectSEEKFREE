#include "menu.h"
#include "pid_v.h"
#include "encoder.h"
#include "key.h"
#include "photo_chuli.h"




#define ips200_x_max 240
#define ips200_y_max 320

int current_state=1;
int p=0;//��¼��ǰָ��
int p_nearby=0;//��¼������ָ��
int input;
extern int status;
extern uint16 w_step,h_step,K,limit;



typedef struct 
{
    unsigned char priority;
    char str[40];
    uint16 x;
    uint16 y;
    float value_f;
    int value_i;
    char int_float;
    void (*Operate_DOWN)();
    void (*Operate_UP)();
    void (*Operate_default)();

}MENU;

void nfunc(void){
    ips200_show_string(0,180,"nofunc");

}

MENU menu[]={
    {1,"pid_param",0,20,0,0,0},
        {2,"p",      ips200_x_max-10 * 7, 20,  0,0,1,  pid_sub_p,           pid_add_p,          nfunc},  
        {2,"i",      ips200_x_max-10 * 7, 40,  0,0,1,  pid_sub_i,           pid_add_i,          nfunc},  
        {2,"d",      ips200_x_max-10 * 7, 60,  0,0,1,  pid_sub_d,           pid_add_d,          nfunc},  
        {2,"i_max",  ips200_x_max-10 * 7, 80,  0,0,1,  pid_sub_i_max,       pid_add_i_max,      nfunc},  
        {2,"d_max",  ips200_x_max-10 * 7, 100, 0,0,1,  pid_sub_d_max,       pid_add_d_max,      nfunc},  
        {2,"output", ips200_x_max-10 * 7, 120, 0,0,1,  pid_sub_output_max,  pid_add_output_max, nfunc},
    {1,"carstatue",0,40,0,0,0},
        {2,"v_left"         ,ips200_x_max-10*7,20,0,0,1,                nfunc,nfunc,nfunc},
        {2,"v_right"        ,ips200_x_max -10*7,40,0,0,1,                nfunc,nfunc,nfunc},
        {2,"encoder_right"  ,ips200_x_max-10*7,60,0,0,0,                nfunc,nfunc,nfunc},
        {2,"encoder_left"   ,ips200_x_max-10*7,80,0,0,0,                nfunc,nfunc,nfunc},
        {2,"angle"          ,ips200_x_max-10*7,100,0,0,0,               nfunc,nfunc,nfunc},
        {2,"angle2"         ,ips200_x_max-10*7,120,0,0,0,               nfunc,nfunc,nfunc},
    {1,"graph_param",0,60,0,0,0},
        {2,"w_step"         ,ips200_x_max-10*7,20,0,0,0,                w_step_sub, w_step_add, nfunc},
        {2,"h_step"         ,ips200_x_max-10*7,40,0,0,0,                h_step_sub, h_step_add, nfunc},
        {2,"K"              ,ips200_x_max-10*7,60,0,0,0,                K_sub,      K_add,      nfunc},
        {2,"limit"          ,ips200_x_max-10*7,80,0,0,0,                limit_sub,  limit_add,  nfunc},
    {1,"graph_show",0,80,0,0,0},
        {2,"graph"          ,ips200_x_max-10*7,20,0,0,0,                nfunc,      nfunc,      nfunc},

    {1,"end",0,0,0,0,0}//����ɾȥ
};

enum condition{
    NOACTION,
    L,
    R,
    UP,
    DOWN,
    CONFIRM,
    BACK

}condition;  
    




/*
------------------------------------------------------------------------------------------------------------------
�������    ��ʼ����Ļ 
����˵��     ��
���ز���     ��
ʹ��ʾ��     ֱ�ӵ���
��ע��Ϣ     ��
-------------------------------------------------------------------------------------------------------------------
*/
void Menu_Screen_Init(void)
{
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);    //����Ϊ�׵׺���
    ips200_init(IPS200_TYPE_SPI);    //����ͨ��ģʽΪSPIͨ��
}
/*
------------------------------------------------------------------------------------------------------------------
�������     �������ݣ���pid����������
����˵��     ��
���ز���     ��
ʹ��ʾ��     ֱ�ӵ���
��ע��Ϣ     �Ѽ������ұ�����������
-------------------------------------------------------------------------------------------------------------------
*/
void update(void)
{
    for(int i=0;strcmp(menu[i].str, "end") != 0;i++)
        {
            if(strcmp(menu[i].str, "encoder_right")==0)
            {
                menu[i].value_i=Encoder_GetInfo_R();
            }
            if(strcmp(menu[i].str, "encoder_left")==0)
            {
                menu[i].value_i=Encoder_GetInfo_L();
            }
            struct pid_v *pid_ptr = PID_vget_param();
            
            
            //PID����
            if(strcmp(menu[i].str, "p")==0)
            {
                menu[i].value_f=pid_ptr->p;
            }
            if(strcmp(menu[i].str, "i")==0)
            {
                menu[i].value_f=pid_ptr->i;
            }
            if(strcmp(menu[i].str, "d")==0)
            {
                menu[i].value_f=pid_ptr->d;
            }           
            if(strcmp(menu[i].str, "i_max")==0)
            {
                menu[i].value_f=pid_ptr->i_max;
            }
            if(strcmp(menu[i].str, "d_max")==0)
            {
                menu[i].value_f=pid_ptr->d_max;
            }
            if(strcmp(menu[i].str, "output")==0)
            {
                menu[i].value_f=pid_ptr->output_max;
            }
            
            //ͼ����
            if(strcmp(menu[i].str, "w_step")==0)
            {
                menu[i].value_i=w_step;
            }
            if(strcmp(menu[i].str, "h_step")==0)
            {
                menu[i].value_i=h_step;
            }
            if(strcmp(menu[i].str, "K")==0)
            {
                menu[i].value_i=K;
            }
            if(strcmp(menu[i].str, "limit")==0)
            {
                menu[i].value_i=limit;
            }
            //ͼ����ʾ
        }
}


/*
------------------------------------------------------------------------------------------------------------------
�������     ��Ļ��ʾ
����˵��     ��
���ز���     ��
ʹ��ʾ��     ֱ�ӵ���
��ע��Ϣ     ��
-------------------------------------------------------------------------------------------------------------------
*/
void output(void) 
{
    update();
    int target_priority=current_state-1;
    if (target_priority==0)
    {
    ips200_show_string(0,0,"menu");//��������ַ�
        int count=1;
        for(int i=0;strcmp(menu[i].str, "end") != 0;i++)
        {
            if(menu[i].priority==1)
            {
                if(i==p)
                {
                    ips200_show_string(0,menu[i].y,"->");//���ָ���ַ�
                    ips200_show_string(20,menu[i].y,menu[i].str);
                }
                else
                {
                    ips200_show_string(20,menu[i].y,menu[i].str);
                }
                count++;
            }
        }
    }
    else if(target_priority!=0)
    {
     ips200_show_string(0,0,menu[p_nearby].str);//����ϼ��ַ�
        for(int i=p_nearby+1;menu[i].priority!=target_priority;i++)
        {
            if(menu[i].priority==current_state)
            {
                if(i==p)
                {
                    ips200_show_string(0,menu[i].y,"->");//���ָ���ַ�
                    ips200_show_string(20,menu[i].y,menu[i].str);
                    if(menu[i].int_float)
                    {
                       ips200_show_float(menu[i].x,menu[i].y,menu[i].value_f,3,3);
                    }
                    else
                    {
                       ips200_show_int(menu[i].x,menu[i].y,menu[i].value_i,5);
                    }
                }
                else
                {
                    ips200_show_string(20,menu[i].y,menu[i].str);
                    if(menu[i].int_float)
                    {
                       ips200_show_float(menu[i].x,menu[i].y,menu[i].value_f,3,3);
                    }
                    else
                    {
                        ips200_show_int(menu[i].x,menu[i].y,menu[i].value_i,5);
                    }
                }
            } 
        }
    }
    if(strcmp(menu[p_nearby].str,"graph_param")==0)
    {
        
    }
}

/*
------------------------------------------------------------------------------------------------------------------
�������     �˵�����
����˵��     ��
���ز���     ��
ʹ��ʾ��     ֱ�ӵ���
��ע��Ϣ     
-------------------------------------------------------------------------------------------------------------------
*/
void Menu_control(void)
{
        output();
//      scanf("%d", &input);�ȴ��޸�
        status=0;
        condition = (enum condition)input; 
        if(input)
        {
            ips200_clear();
        }
        switch (condition)
        {
            case NOACTION:
                break;
            
        case R:
            if (strcmp(menu[p].str, "end") != 0&&menu[p+1].priority>=menu[p].priority)
            {
                int temp=menu[p].priority;
                p++;
                while(menu[p].priority!=temp){p++;} 
            }
            else
            {
                ips200_show_string(0,180,"error");
            }
            break;
        case L:
            if(p!=0&&menu[p-1].priority>=menu[p].priority)
            {
                int temp=menu[p].priority;
                p--;
                while (menu[p].priority!=temp){p--;}
            }
            else
            {
                ips200_show_string(0,180,"error");
            }
             

            break;
        case UP:
            menu[p].Operate_UP();
            break;
        case DOWN:
            menu[p].Operate_DOWN();
            break;
        case CONFIRM:
            if(menu[p+1].priority==current_state+1&&strcmp(menu[p+1].str,"end")!=0)
            {
                current_state++;
                p_nearby=p;
                p++;
            }
            else
            {
                ips200_show_string(0,180,"error");
            }
         

            break;
        case BACK:
        if(menu[p].priority!=1)
        {
            current_state--;
            p=p_nearby;
            while (menu[p_nearby].priority!=current_state-1)
            {
                p_nearby--;
            }
        }
        else
        {
            ips200_show_string(0,180,"error");
        }

        default:
            break;
        }
        input=0;
        
}