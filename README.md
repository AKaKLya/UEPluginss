# SingleInstanceLock

![SS](https://github.com/user-attachments/assets/c8224baf-0a88-4968-853b-17f519e012f9)

使用Windows的mutex检测工程多开的情况，

工程A的路径 D:\ProjectA\A.uproject .

A第一次启动， 创建互斥对象 并命名为 Global\UE4_SingleInstance_D__ProjectA_A.project。 

使用具体路径当做名字，确保mutex的命名唯一，下面简称mutex-A

A第二次被启动，创建互斥对象 并命名为 Global\UE4_SingleInstance_D__ProjectA_A.project。，创建失败，原因:互斥对象已经存在 (A已经被启动)

此时询问是否继续启动，如果继续启动 mutex-A 的引用计数+1.

两个A被启动了，关闭一个 mutex-A 的引用计数-1，再关闭一个 再-1，引用计数清零 Windows销毁这个mutex。

即便是引擎进程崩溃，Windows也会负责清理这个进程创建的mutex.

# AutoImport
对特定的文件夹进行监控，如果文件夹发生变动 则遍历文件夹中的资产文件 并导入到引擎，

自动导入 .png .TGA .fbx等格式，可以自定义扩展其它格式的对应操作

系统会监控以下事件：

文件创建/删除/重命名、目录创建/删除/重命名、文件内容修改、创建时间变更

导入流程:
0. 引擎启动时，注册监控事件，监控D:\UEImport文件夹，如果文件夹不存在 则自动创建这个文件夹
1. 从PS中导出 T.png 到 D:\UEImport 文件夹中，文件夹发生变化，触发插件的监控事件
2. 插件判断文件名T.png的后缀 选择贴图导入模式，自动导入贴图
3. 贴图导入完成后，删除D:\UEImport\T.png文件
