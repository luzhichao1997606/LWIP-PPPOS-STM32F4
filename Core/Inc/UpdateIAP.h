/*
 * @FileName:
 * @Description:
 * @Version:
 * @Author: xxx
 * @Date: 2020-08-26 18:52:46
 * @LastEditors: fwz
 * @LastEditTime: 2020-09-08 14:12:43
 */

#ifndef _UPDATE_IAP_H_
#define _UPDATE_IAP_H_

#if defined __cplusplus
extern "C" {
#endif

/******************************************************************************/
/***        头文件  Include files                                            ***/
/******************************************************************************/
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <string.h>
/******************************************************************************/
/***        宏定义  Macro Definitions                                        ***/
/******************************************************************************/
/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0 (( uint32_t )0x08000000)  /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1 (( uint32_t )0x08004000)  /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2 (( uint32_t )0x08008000)  /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3 (( uint32_t )0x0800C000)  /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4 (( uint32_t )0x08010000)  /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5 (( uint32_t )0x08020000)  /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6 (( uint32_t )0x08040000)  /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7 (( uint32_t )0x08060000)  /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8 (( uint32_t )0x08080000)  /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9 (( uint32_t )0x080A0000)  /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10 (( uint32_t )0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11 (( uint32_t )0x080E0000) /* Base @ of Sector 11, 128 Kbytes */
// APP³ÌÐò×´Ì¬±êÖ¾
//当内部flash没有被升级过则数值应为0xffffffff
#define FLASH_APP_DEFAULT_VALUE 0xFFFFFFFF

/******************************************************************************/
/***        数据类型定义  Type Definitions                                   ***/
/******************************************************************************/
typedef signed char      int8_t;
typedef signed short int int16_t;
typedef signed int       int32_t;

/* exact-width unsigned integer types */
typedef unsigned char      uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int       uint32_t;

/******************************************************************************/
/***        外部变量声明  Exported Variables                                 ***/
/******************************************************************************/

/******************************************************************************/
/***        外部函数声明  Exported Functions                                 ***/
/******************************************************************************/
typedef void (*iapfun)(void);          //定义一个函数类型的参数.
void iap_load_app(uint32_t appxaddr);  //执行flash里面的app程序

bool check_update_flag(void);
// bool check_app1_flag(void);
// bool check_app2_flag(void);
bool     check_app1_existed(void);
uint32_t flash_read_word(uint32_t addr);
bool     FlashErase(uint32_t StartAddress, uint16_t ErasePageNum);
bool     flash_write_word(uint32_t WriteAddr, uint32_t data);
void     update_app2_to_app1(void);
void     update_app1_to_app2(void);
void     mcu_reset(void);

#if defined __cplusplus
}
#endif

#endif /* _UPDATE_IAP_H_ */
/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/
