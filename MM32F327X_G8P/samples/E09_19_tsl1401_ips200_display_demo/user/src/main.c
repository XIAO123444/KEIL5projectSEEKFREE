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
// 接入红孩儿 CCD 模块
//      模块管脚            单片机管脚
//      CLK                 查看 zf_device_tsl1401.h 中 TSL1401_CLK_PIN 宏定义
//      SI                  查看 zf_device_tsl1401.h 中 TSL1401_SI_PIN 宏定义
//      AO[x]               查看 zf_device_tsl1401.h 中 TSL1401_AO_PINx 宏定义
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
// 
// 接入2寸IPS模块
//      模块管脚            单片机管脚
//      双排排针 并口两寸屏 硬件引脚
//      RD                  查看 zf_device_ips200.h 中 IPS200_RD_PIN_PARALLEL8     宏定义 A5
//      WR                  查看 zf_device_ips200.h 中 IPS200_WR_PIN_PARALLEL8     宏定义 A7
//      RS                  查看 zf_device_ips200.h 中 IPS200_RS_PIN_PARALLEL8     宏定义 A6
//      RST                 查看 zf_device_ips200.h 中 IPS200_RST_PIN_PARALLEL8    宏定义 D0
//      CS                  查看 zf_device_ips200.h 中 IPS200_CS_PIN_PARALLEL8     宏定义 A4
//      BL                  查看 zf_device_ips200.h 中 IPS200_BL_PIN_PARALLEL8     宏定义 D1
//      D0-D7               查看 zf_device_ips200.h 中 IPS200_Dx_PIN_PARALLEL8     宏定义 D8/D9/D10/D11/D12/D13/D14/D15
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
//      单排排针 SPI 两寸屏 硬件引脚
//      SCL                 查看 zf_device_ips200.h 中 IPS200_SCL_PIN_SPI  宏定义 A5
//      SDA                 查看 zf_device_ips200.h 中 IPS200_SDA_PIN_SPI  宏定义 A7
//      RST                 查看 zf_device_ips200.h 中 IPS200_RST_PIN_SPI  宏定义 A6
//      DC                  查看 zf_device_ips200.h 中 IPS200_DC_PIN_SPI   宏定义 D0
//      CS                  查看 zf_device_ips200.h 中 IPS200_CS_PIN_SPI   宏定义 A4
//      BL                  查看 zf_device_ips200.h 中 IPS200_BLk_PIN_SPI  宏定义 D1
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源



// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 将核心板插在主板上 插到底
// 
// 2.2寸IPS模块插入主板屏幕接口 并连接好红孩儿 CCD 模块 在断电情况下完成连接
// 
// 3.主板上电 或者核心板链接完毕后上电 核心板按下复位按键
// 
// 4.屏幕会显示初始化信息然后显示摄像头图像
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
#define IPS200_TYPE     (IPS200_TYPE_PARALLEL8)                                 // 双排排针 并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8
                                                                                // 单排排针 SPI 两寸屏 这里宏定义填写 IPS200_TYPE_SPI

int main (void)
{
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART

    // 此处编写用户代码 例如外设初始化代码等
    ips200_init(IPS200_TYPE);
    ips200_show_string(0, 0, "tsl1401 init.");
    tsl1401_init();
    ips200_show_string(0, 1, "init success.");
    ips200_full(RGB565_66CCFF);
    // 此处编写用户代码 例如外设初始化代码等

    while(1)
    {
        // 此处编写需要循环执行的代码
        if(tsl1401_finish_flag)
        {
            ips200_show_wave(0,  0, (const uint16 *)tsl1401_data[0], 128, 255, 128, 64);
            ips200_show_wave(0, 64, (const uint16 *)tsl1401_data[1], 128, 255, 128, 64);
            tsl1401_finish_flag = 0;
        }
        // 此处编写需要循环执行的代码
    }
}
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：屏幕不显示
//      如果使用主板测试，主板必须要用电池供电 检查屏幕供电引脚电压
//      检查屏幕是不是插错位置了 检查引脚对应关系
//      如果对应引脚都正确 检查一下是否有引脚波形不对 需要有示波器
//      无法完成波形测试则复制一个GPIO例程将屏幕所有IO初始化为GPIO翻转电平 看看是否受控
// 
// 问题2：CCD图像不正确
//      检查CCD线是否正确连接
//      调试程序查看是否正常触发采集中断
//      调整曝光值
