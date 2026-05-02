
# Motor
 **Questions**  
 * 电机编码器读过来的位置范围同样是-3.14~3.14，为什么有的电机要转4圈，有的只要转一圈？
# Ozone
**Questions**  
* 烧录代码时，读条后显示部分代码未成功烧录：  
1.关闭其他烧录软件，比如你用ozone同时开了两个工程，或者另外一个类似于ozone的软件正在占用资源。如果还是没有用的话就把jlink拔插一下
# SystemView
**Questions**  
* 硬件连接问题，软件连接不上芯片：  
1.确保芯片内部烧录了正确的代码  
2.在j_linkRTT模式下面，cores_number不能选择AUTO，手动输入数量
3.查看是否代码里面是否正确开启了sysview（SEGGER_SYSVIEW_Conf();）
* sysview软件里面可以看到有文件信息（说明正确连接到了芯片和文件），但是没有进入task和结束task的信息
1.  SEGGER_SYSVIEW_Conf();  
  // SEGGER_SYSVIEW_Start();  
  SystemView_Register_ISRs();  
  可能是因为加了中间这行命令