/*********************************************************************************************************************
* MM32F327X-G8P Opensourec Library 即（MM32F327X-G8P 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MM32F327X-G8P 开源库的一部分
* 
* MM32F327X-G8P 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.37
* 适用平台          MM32F327X_G8P
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完


// *************************** 例程硬件连接说明 ***************************
// 接入小钻风硬件二值化摄像头 对应主板摄像头接口 请注意线序
//      模块管脚            单片机管脚
//      TXD/SCL             查看 zf_device_ov7725.h 中 OV7725_COF_UART_TX 或 OV7725_COF_IIC_SCL 宏定义
//      RXD/SDA             查看 zf_device_ov7725.h 中 OV7725_COF_UART_RX 或 OV7725_COF_IIC_SDA 宏定义
//      PCLK                查看 zf_device_ov7725.h 中 OV7725_PCLK_PIN 宏定义
//      VSY                 查看 zf_device_ov7725.h 中 OV7725_VSYNC_PIN 宏定义
//      D0-D7               查看 zf_device_ov7725.h 中 OV7725_DATA_PIN 宏定义 从该定义开始的连续八个引脚
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
// 
// 接入无线转串口模块
//      模块管脚            单片机管脚
//      RX                  查看 zf_device_wireless_uart.h 中 WIRELESS_UART_RX_PINx 宏定义 默认 C6
//      TX                  查看 zf_device_wireless_uart.h 中 WIRELESS_UART_TX_PINx 宏定义 默认 C7
//      RTS                 查看 zf_device_wireless_uart.h 中 WIRELESS_UART_RTS_PINx 宏定义 默认 C13
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源



// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 将核心板插在主板上 插到底 摄像头接在主板的摄像头接口 注意线序 无线模块插入主板无线模块接口
// 
// 2.主板上电 或者核心板链接完毕后上电
// 
// 3.电脑上使用摄像头的传感器调试助手 串口波特率为 zf_device_wireless_uart.h 文件中 WIRELESS_UART_BUAD_RATE 宏定义 默认 115200，核心板按下复位按键
// 
// 4.调试助手选择二值化图像 分辨率填写宽160 高120 勾选开始采集再打开对应的串口
// 
// 5.上位机将会显示图像
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
#define LED1                    ( H2 )

int main (void)
{
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART

    // 此处编写用户代码 例如外设初始化代码等
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式

    if(wireless_uart_init())                                                    // 判断是否通过初始化
    {
        while(1)                                                                // 初始化失败就在这进入死循环
        {
            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭
            system_delay_ms(100);                                               // 短延时快速闪灯表示异常
        }
    }

    while(1)
    {
        if(ov7725_init())
        {
            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
        }
        else
        {
            break;
        }
        system_delay_ms(1000);                                                  // 闪灯表示异常
    }
    // 此处编写用户代码 例如外设初始化代码等

    while(1)
    {
        // 此处编写需要循环执行的代码
        if(ov7725_finish_flag)
        {
            wireless_uart_send_image((const uint8 *)ov7725_image_binary, OV7725_IMAGE_SIZE);
            ov7725_finish_flag = 0;
        }
        // 此处编写需要循环执行的代码
    }
}
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：上位机没有图像
//      查看串口助手打开的是否是正确的串口，检查打开的 COM 口是否对应的是调试下载器或者 USB-TTL 模块的 COM 口
//      如果是使用逐飞科技 CMSIS-DAP | ARM 调试下载器连接，那么检查下载器线是否松动，检查核心板串口跳线是否已经焊接，串口跳线查看核心板原理图即可找到
//      如果是使用 USB-TTL 模块连接，那么检查连线是否正常是否松动，模块 TX 是否连接的核心板的 RX，模块 RX 是否连接的核心板的 TX
//      如果串口和接线都正常 查看LED灯是否闪烁 如果闪烁证明初始化失败 检查摄像头接线是否正常 线序是否正确
//      如果摄像头接线都正常 LED灯不闪烁 联系技术客服即可
// 
// 问题2：图像乱码、错位、不正常
//      检查是否选择的二值化图像选项
//      检查是否正确设置了图像宽度和高度 宽度和高度查看 zf_device_ov7725.h 中 OV7725_W 和 OV7725_H 宏定义
