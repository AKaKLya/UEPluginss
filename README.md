# SingleInstanceLock
使用Windows的mutex检测工程多开的情况，

工程A的路径 D:\ProjectA\A.uproject .

A第一次启动， 创建互斥对象 并命名为 Global\UE4_SingleInstance_D__ProjectA_A.project。 

使用具体路径当做名字，确保mutex的命名唯一，下面简称mutex-A

A第二次被启动，创建互斥对象 并命名为 Global\UE4_SingleInstance_D__ProjectA_A.project。，创建失败，原因:互斥对象已经存在 (A已经被启动)

此时询问是否继续启动，如果继续启动 mutex-A 的引用计数+1.

两个A被启动了，关闭一个 mutex-A 的引用计数-1，再关闭一个 再-1，引用计数清零 Windows销毁这个mutex。

即便是引擎进程崩溃，Windows也会负责清理这个进程创建的mutex.

