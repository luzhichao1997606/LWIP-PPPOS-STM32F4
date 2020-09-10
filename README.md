<!--
 * @FileName: 
 * @Description: 
 * @Version: 
 * @Author: lzc
 * @Date: 2020-09-10 10:50:39
 * @LastEditors: lzc
 * @LastEditTime: 2020-09-10 11:26:15
-->
# LWIP-PPPOS-STM32F4
LWIP+STM32F429+PPPOS
***
#### 平  台: STM32F429VET6
#### 编译器: KEIL5
#### 协议栈：LWIP
#### 模块：4G移远EC200S-CN
#### 配置：串口6为连接到模块，串口4为调试输出
#### 效果
>
```
status_cb: Connected
   our_ipaddr  = 10.189.82.15
   his_ipaddr  = 10.64.64.64
   netmask     = 255.255.255.255
connect to TCPsend:21 45 00 00 2c 00 00 00 00 ff 06 56 6f 0a bd 52 0f 7d 5d 7b 8b 15 cb 40 04 3a 00 00 19 6d 00 00 00 00 60 02 08 60 45 1e 00 00 02 04 02 18 64 b4 7e 
```
```

[11:25:52.347]收←◆------------------------------TCP STATE:4----------------
12


[11:25:52.394]收←◆send:7e 21 45 00 00 51 00 10 00 00 ff 06 56 3a 0a bd 52 0f 7d 5d 7b 8b 15 cb 40 04 3a 00 00 1b 50 6c 25 97 50 50 18 08 60 65 f9 00 00 31 32 0d 0a 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 0d 0a c6 8e 7e 

[11:25:52.675]收←◆recv:7e 21 45 04 00 28 f3 a3 40 00 73 06 ae cb 7d 5d 7b 8b 15 0a bd 52 0f 04 3a cb 40 6c 25 97 50 00 00 1b 79 50 10 fd 65 5e a8 00 00 5e a1 7e 

```

TCP接收端收到数据 `123456789abcdefghijklmnopqrstuvwxyz`