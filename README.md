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

外部软件只需要导出资产到 D:\UEImport，剩下的交给插件.

系统会监控以下事件：

文件创建/删除/重命名、目录创建/删除/重命名、文件内容修改、创建时间变更

导入流程:
1. 引擎启动时，注册监控事件，监控D:\UEImport文件夹，如果文件夹不存在 则自动创建这个文件夹
2. 从PS中导出 T.png 到 D:\UEImport 文件夹中，文件夹发生变化，触发插件的监控事件
3. 插件判断文件名T.png的后缀 选择贴图导入模式，自动导入贴图
4. 贴图导入完成后，删除D:\UEImport\T.png文件

## AutoImport_Tool
提供了PS和3DMAX的插件，一键导出png fbx 到 D:\UEImport文件夹

### PS插件安装
把SaveToPSLog文件夹 放在 Adobe Photoshop 2020\Required\CEP\extensions

打开PS，在工具栏中的 窗口--->扩展功能--->PSLogExport 召唤插件按钮

SaveToPSLog\jsx\main.jsx 可以修改导出路径

### 3dmax插件安装
方法一:

DragAndDrop-Macro2.mcr 放到 C:\Users\Administrator\AppData\Local\Autodesk\3dsMax\2021 - 64bit\CHS\usermacros

Administrator是你的Windows系统使用的用户名，

![WinName](https://github.com/user-attachments/assets/9467c895-54f4-495c-8d63-5b88c03482ae)

方法二:
工具栏--->脚本--->打开脚本， 选择A.ms
![m1](https://github.com/user-attachments/assets/e79f90ee-f242-469a-9421-83bbc9781063)

![m2](https://github.com/user-attachments/assets/77e2b939-7e02-4826-9fd0-e4959dad5eb2)

![m3](https://github.com/user-attachments/assets/961f0c58-077e-4110-9efa-ded401334b02)

![m4](https://github.com/user-attachments/assets/2ecf96a9-09e3-4491-b17d-37ca77e101ca)


