# 工程组第二次组会temp（2026/6/13-6/21）
## 完成进度（截止6/21）
| 负责人 | 进度 | 存在缺陷/遇到问题 |
| :---: | :---: | :---: |
| SJH | 基本确定腿构型 | 连杆长度未确定，电机摆放位置有待考量 |
| CYF | 基本确定新车中心框架尺寸，较老车稍宽 | 腿部构型实现未确定 |
| ZXZ | 完成丝杆夹爪初步设计草图 | 暂未设计夹爪固定装置，未进行可行性验证，未确定各关节长度 |
| ZKN | 导出老车建模文件，mujoco适配，分离视觉模型和碰撞模型，增加按键控制暂停和重载模型功能，重补第一版验证基本可行 | mujoco：腿部电机控制未完成，考虑py为c提供接口避免相同功能重复实现未完成。复现：老车一侧腿部摇臂电机航插松动导致无法成功使能未更换，老车机械臂关节电机通讯失败 |
## 后续工作（主要来自胡哥提点）
- 清点工程电机
- 验证导轮上岛会不会被卡住，如果会被卡住，考虑其他方案
- 早点造原型车
## 资料
[机械设计杂谈/来源：B站/来源：胡哥分享](https://www.bilibili.com/video/BV1VLjh6HE9P?buvid=Y74D047B39E750FB43CCA11146D2F968507A&from_spmid=tm.recommend.0.0&is_story_h5=false&mid=4dpVbJXnrzM2X7l8s7o0tg%3D%3D&plat_id=116&share_from=ugc&share_medium=iphone&share_plat=ios&share_session_id=EA6F232E-8D49-4320-ADFD-50586F62273D&share_source=weixin&share_tag=s_i&spmid=united.player-video-detail.0.0&timestamp=1782052823&unique_k=TJXx5V6&up_id=1246247490)  
[从系统分析开始的控制笔记/来源：胡哥分享](https://liuskywalkerjskd.github.io/Control-Note/)  
[ACE战队 2024-2025赛季轮腿机器人技术文档-机械](https://bbs.robomaster.com/article/728195)