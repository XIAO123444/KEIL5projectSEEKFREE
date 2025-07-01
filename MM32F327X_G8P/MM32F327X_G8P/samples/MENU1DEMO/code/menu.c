#include "menu.h"
#define ips200_x_max 240
#define ips200_y_max 320

int current_state=1;
int p=0;//记录当前指针
int p_nearby=0;//记录所属的指针
int input;

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

}MENU;


//菜单编写规范
MENU menu[]={
    {1,"pid",0,20,0,0,0},
        {2,"p",ips200_x_max-10*6,20,0,0,1},
        {2,"i",ips200_x_max-10*6,40,0,0,1},
        {2,"d",ips200_x_max-10*6,60,0,0,1},
    {1,"carstatue",0,40,0,0,0},
        {2,"v_left",ips200_x_max-10*6,20,0,0,1},
        {2,"v_right",ips200_x_max-10*6,40,0,0,1},
        {2,"encoder_right",ips200_x_max-10*6,60,0,0,1},
        {2,"encoder_left",ips200_x_max-10*6,80,0,0,1},
        {2,"angle",ips200_x_max-10*6,100,0,0,0},           
    {1,"end",0,0,0,0,0}//不可删去
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
    


//调试屏幕
void IPS200_Show_Init(void)
{
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);    //设置为白底黑字
    ips200_init(IPS200_TYPE_SPI);    //设置通信模式为SPI通信
}




void output(void) 
{
    int target_priority=current_state-1;
    if (target_priority==0)
    {
    ips200_show_string(0,0,"menu");//输出标题字符
        int count=1;
        for(int i=0;strcmp(menu[i].str, "end") != 0;i++)
        {
            if(menu[i].priority==1)
            {
                if(i==p)
                {
                    ips200_show_string(0,menu[i].y,"->");//输出指向字符
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
     ips200_show_string(0,0,menu[p_nearby].str);//输出上级字符
        for(int i=p_nearby+1;menu[i].priority!=target_priority;i++)
        {
            if(menu[i].priority==current_state)
            {
                if(i==p)
                {
                    ips200_show_string(0,menu[i].y,"->");//输出指向字符
                    ips200_show_string(20,menu[i].y,menu[i].str);
                    if(menu[i].int_float)
                    {
                       ips200_show_float(ips200_x_max-70,menu[i].y,menu[i].value_f,3,3);
                    }
                    else
                    {
                       ips200_show_int(ips200_x_max-70,menu[i].y,menu[i].value_i,5);
                    }
                }
                else
                {
                    ips200_show_string(20,menu[i].y,menu[i].str);
                    if(menu[i].int_float)
                    {
                       ips200_show_float(ips200_x_max-70,menu[i].y,menu[i].value_f,3,3);
                    }
                    else
                    {
                        ips200_show_int(ips200_x_max-70,menu[i].y,menu[i].value_i,5);
                    }
                }
            } 
        }
    }
}


//注：这里为了防止占用，将其写进一个TIM中断函数里备用。
void Menu_control(void)
{
        output();
//      scanf("%d", &input);等待修改
        condition = (enum condition)input; 
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
}