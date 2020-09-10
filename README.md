<!--
 * @FileName: 
 * @Description: 
 * @Version: 
 * @Author: lzc
 * @Date: 2020-09-10 10:50:39
 * @LastEditors: lzc
 * @LastEditTime: 2020-09-10 11:02:46
-->
# LWIP-PPPOS-STM32F4
LWIP+STM32F429+PPPOS
***
### 平  台: STM32F429VET6
### 编译器: KEIL5
### 协议栈：LWIP
### 模块：4G移远EC200S-CN
### 效果
>
```
status_cb: Connected
   our_ipaddr  = 10.189.82.15
   his_ipaddr  = 10.64.64.64
   netmask     = 255.255.255.255
connect to TCPsend:21 45 00 00 2c 00 00 00 00 ff 06 56 6f 0a bd 52 0f 7d 5d 7b 8b 15 cb 40 04 3a 00 00 19 6d 00 00 00 00 60 02 08 60 45 1e 00 00 02 04 02 18 64 b4 7e 
```
```
[10:59:42.811]收←◆------------------------------TCP STATE:4----------------
62


[10:59:43.126]收←◆send:7e 21 45 00 00 51 00 43 00 00 ff 06 56 07 0a bd 52 0f 7d 5d 7b 8b 15 cb 40 04 3a 00 00 23 52 63 8e ba f1 50 18 08 60 3d ed 00 00 36 32?d 0a 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 0d 0a bd ae 7e 

[10:59:43.299]收←◆QueueQueue initQueue Add2233recv:7e 21 45 04 00 28 f2 8e 40 00 73 06 af e0 7d 5d 7b 8b 15 0a bd 52 0f 04 3a cb 40 63 8e ba f1 00 00 23 7b 50 10 fe 51 3a b0 00 00 64 86 7e 
```

TCP接收端收到数据 `123456789abcdefghijklmnopqrstuvwxyz`