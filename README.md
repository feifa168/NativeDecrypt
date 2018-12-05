## 简介
> NativeDecrypt用于加载class时解密java字节码文件。配合xml使用，使用动态库pugixml处理xml。

## 依赖
* pugixml动态库
* enc_config.xml文件

## 用法
> libNativeEncrypt.dll用于加密，放到java工程encrypt模块中。
* 加密
>java -jar NativeEncrypt.jar [-src xxx.jar -dst xxx_encrypt.jar] 参数可有可无，配合enc_config.xml使用。
 enc_config.xml格式如下
 ```xml
 <?xml version="1.0" encoding="utf-8"?>
 
 <encrypt>
     <src>src.jar</src>
     <dst>src_encrypt.jar</dst>
     <files>
         <file type="package">com.shell.run</file>
         <file type="package">com.ft.config</file>
     </files>
 </encrypt>
 ```
* 解密
>java -agentlib:libNativeDecrypt xxx_encrypt.jar paramers...

## 原理
> java agent 有三种方法加载动态库
* agentlib
    > 使用java参数 java -agentlib:libxxx，实现jvmti.h文件中Agent_OnLoad函数，在jvm启动时加载；
    Agent_OnAttach在附加到java进程时加载。设置回调函数解密class文件。
* agentpath
    > 等同于agentlib，agentpath是文件全名，带扩展名，agentlib不带扩展名。
* javaagent java实现
    > javaagent 是java的一种实现方式，agentlib和agentpath是native实现方式。功能类似于Agent_OnLoad和Agent_OnAttach。

