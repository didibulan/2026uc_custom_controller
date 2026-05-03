
# Motor
## DM
 **Questions**  
 * 电机编码器读过来的位置范围同样是-3.14~3.14，为什么有的电机要转4圈，有的只要转一圈？
 ## Dji
# Ozone
**Questions**  
* 烧录代码时，读条后显示部分代码未成功烧录：  
1. 关闭其他烧录软件，比如你用ozone同时开了两个工程，或者另外一个类似于ozone的软件正在占用资源。如果还是没有用的话就把jlink拔插一下
# SystemView
**Questions**  
* **软件连接不上芯片：**
1. 确保芯片内部烧录了正确的代码  
1. 在j_linkRTT模式下面，cores_number不能选择AUTO，手动输入数量
1. Target->Recoder Configuration中没有jlink编号没关系的（多插jlink未尝试），只要jlink-commander识别到了就行
1. 查看是否代码里面是否正确开启了sysview（SEGGER_SYSVIEW_Conf();）
* **sysview软件里面可以看到有文件（说明正确连接到了芯片和文件），但是没有进入task和结束task的信息**
1. 可能是因为加了中间这行命令，Sysview大多不需要显式启动命令
```
SEGGER_SYSVIEW_Conf();  
// SEGGER_SYSVIEW_Start();  
SystemView_Register_ISRs();  
```
* **如何修改system工程名字？**
1. SEGGER_SYSVIEW_Config_FreeRTOS.c文件
```
// The application name to be displayed in SystemViewer
#define SYSVIEW_APP_NAME        "9Axis_Arm_MC02_H7"     <-
```
* **Could not find systemview buffer:**
1. 检查STM32H723XG_FLASH.ld文件，大概率是因为程序摆放的位置不对，导致sysview找不到正确的读取位置
```
/* Specify the memory areas */
MEMORY
{
DTCMRAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 128K
RAM_D1 (xrw)       : ORIGIN = 0x24000000, LENGTH = 320K
RAM_D2 (xrw)       : ORIGIN = 0x30000000, LENGTH = 32K
RAM_D3 (xrw)       : ORIGIN = 0x38000000, LENGTH = 16K
ITCMRAM (xrw)      : ORIGIN = 0x00000000, LENGTH = 64K
FLASH (rx)         : ORIGIN = 0x8000000, LENGTH = 1024K
}
```
比如这个地方，一开始是写在DTCMRAM这里的，但是sysview没法读取，后来改成RAM_D1，才能读取成功，这里记得同步SEGGER_SYSVIEW_Config_FreeRTOS.c、SEGGER_SYSVIEW_Conf.h文件里的相应代码，同时修改STM32H723XG_FLASH.ld文件里面的所有相关代码
```
// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE        (0x24000000)           <-
```
```
#define SEGGER_SYSVIEW_RTT_BUFFER_SIZE      4096
#define SEGGER_SYSVIEW_ID_BASE              0x24000000 <-
#define SEGGER_SYSVIEW_ID_SHIFT             2
```
STM32H723XG_FLASH.ld文件是干嘛的？这是GCC工具链脚本，用来规定程序在flash/RAM里面如何摆放。后缀.ld：GNU Linker Script指的是GCC连接器的配置文件
