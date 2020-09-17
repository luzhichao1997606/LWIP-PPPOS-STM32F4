/*
 * @FileName:
 * @Description:
 * @Version:
 * @Author: fwz
 * @Date: 2020-08-26 18:11:55
 * @LastEditors: fwz
 * @LastEditTime: 2020-09-08 14:15:58
 */
#ifndef __Mcu_Config_h
#define __Mcu_Config_h

/************************** IAP 数据相关参数 **************************/

// 系统默认的起始ROM和RAM地址
#define BASE_OF_ROM (0x08000000)
#define BASE_OF_RAM (0x20000000)

//后面扇大小128k
#define MCU_FLASH_PAGE_SIZE 128 * 1024
//总共页数：32*1024/128=256

/************************************************
 *应用程序由Bootloader引导再启动
 *------------------------------------------------
 *内部Flash分区:
 * | 62kB boot | 50kB param | 200kB app1 | 200kB app2
 * 目前Bootloader程序rom用了8.47kB，剩余53.53kb
 *------------------------------------------------
 *RAM分区:
 * | 6K boot  |  58K app1
 * 目前Bootloader程序ram用了1.48kB，剩余1.02kB
 ************************************************/
// Bootloader代码区域
#define BLOCK_BOOTLOADER_START (BASE_OF_ROM)  // 0x08000000
#define BLOCK_BOOTLOADER_SECTOR (16 * 1024)   // 16K
#define BLOCK_BOOTLOADER_SPACE (BLOCK_BOOTLOADER_SECTOR)

// 系统参数区域
#define BLCOK_SYS_PARAM_START (BLOCK_BOOTLOADER_START + BLOCK_BOOTLOADER_SPACE)  ////0x08004000
#define BLCOK_SYS_PARAM_SECTOR (16 * 1024)
#define BLCOK_SYS_PARAM_SPACE (BLCOK_SYS_PARAM_SECTOR)

// APP1代码区域
#define BLOCK_APP1_START (BLCOK_SYS_PARAM_START + BLCOK_SYS_PARAM_SPACE)  // 0x8008000
#define BLOCK_APP1_NUM (4)
#define BLCOK_APP1_SPACE (MCU_FLASH_PAGE_SIZE * 1 + 16 * 1024 * 2 + 64 * 1024 * 1)

// APP2代码区域
#define BLOCK_APP2_START (BLOCK_APP1_START + BLCOK_APP1_SPACE)  // 0x08040000
#define BLOCK_APP2_NUM (2)                                      //
#define BLCOK_APP2_SPACE (MCU_FLASH_PAGE_SIZE * BLOCK_APP2_NUM)
// // Bootloader代码区域
// #define BLOCK_BOOTLOADER_START (BASE_OF_ROM)  // 0x08000000
// #define BLOCK_BOOTLOADER_NUM (20)             // 1页 20*128空间
// #define BLOCK_BOOTLOADER_SPACE (MCU_FLASH_PAGE_SIZE * BLOCK_BOOTLOADER_NUM)

// // 系统参数区域
// #define BLCOK_SYS_PARAM_START (BLOCK_BOOTLOADER_START + BLOCK_BOOTLOADER_SPACE)  ////0x08000A00
// #define BLCOK_SYS_PARAM_NUM (2)                                                  // 1页 2*128=256空间
// #define BLCOK_SYS_PARAM_SPACE (MCU_FLASH_PAGE_SIZE * BLCOK_SYS_PARAM_NUM)

// // APP1代码区域
// #define BLOCK_APP1_START (BLCOK_SYS_PARAM_START + BLCOK_SYS_PARAM_SPACE)  // 0x08000B00
// #define BLOCK_APP1_NUM (117)                                              // 117*128=  空间
// #define BLCOK_APP1_SPACE (MCU_FLASH_PAGE_SIZE * BLOCK_APP1_NUM)

// // APP2代码区域
// #define BLOCK_APP2_START (BLOCK_APP1_START + BLCOK_APP1_SPACE)  // 0x08004580
// #define BLOCK_APP2_NUM (117)                                    // 117*128=  空
// #define BLCOK_APP2_SPACE (MCU_FLASH_PAGE_SIZE * BLOCK_APP2_NUM)
/*********************************************************************/

///* Bootloader程序版本号 */
////主版本编号
//#define Bootloader_Software_Version_Major (3u)
////子版本编号
//#define Bootloader_Software_Version_Child (1u)

//程序更新标志位地址，更新之后变成0x55555555，目前只设置1个标志位
#define UPDATE_FLASH_FLAG BLCOK_SYS_PARAM_START  // 4 byte
//本次更新数据内容长度的地址
#define UPDATE_DATA_LEN UPDATE_FLASH_FLAG + 4  // 4 byte
//本次更新数据内容CRC32
#define UPDATE_DATA_CRC32 UPDATE_DATA_LEN + 4  // 4 byte

#endif
