/*
 * @FileName:
 * @Description:
 * @Version:
 * @Author: fwz
 * @Date: 2020-08-26 17:37:26
 * @LastEditors: lzc
 * @LastEditTime: 2020-09-15 14:47:29
 */

/******************************************************************************/
/***        头文件  Include files                                            ***/
/******************************************************************************/
/* 升级头文件 */
#include "UpdateIAP.h"
/* 系统头文件 */
#include "MCU_Config.h"
/******************************************************************************/
/***        宏定义  Macro Definitions                                        ***/
/******************************************************************************/

/******************************************************************************/
/***        数据类型定义  Type Definitions                                   ***/
/******************************************************************************/

/******************************************************************************/
/***        静态函数声明  Local Function Prototypes                          ***/
/******************************************************************************/
static uint32_t get_sector(uint32_t Address);
/******************************************************************************/
/***        外部变量定义  Exported Variables                                 ***/
/******************************************************************************/
iapfun jump2app;
/******************************************************************************/
/***        静态变量定义  Local Variables                                    ***/
/******************************************************************************/

/******************************************************************************/
/***        外部函数定义  Exported Functions                                 ***/
/******************************************************************************/

void MSR_MSP(unsigned int addr);  //设置堆栈地址

/**
 * @name:void mcu_reset(void)
 * @brief:设备重启
 * @author: fwz
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
void mcu_reset(void)
{
    //关闭总中断
    __disable_irq();
    NVIC_SystemReset();
}

//设置栈顶地址
// addr:栈顶地址
__asm void MSR_MSP(unsigned int addr)
{
    MSR             MSP, r0  // set Main Stack value
                 BX r14
}

/**
 * @name:void iap_load_app(uint32_t appxaddr)
 * @brief:跳转到应用程序段
 * @author: fwz
 * @param {uint32_t} appxaddr:用户代码起始地址.
 * @return {type} None
 * @note: 修改记录：初次创建
 */
void iap_load_app(uint32_t appxaddr)
{
    if (((*( __IO uint32_t* )appxaddr) & 0x2FFE0000) == 0x20000000)  //检查栈顶地址是否合法.
    {
        jump2app = (iapfun) * ( __IO uint32_t* )(appxaddr + 4);  //用户代码区第二个字为程序开始地址(复位地址)
        MSR_MSP(*( __IO uint32_t* )appxaddr);                    //初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
        jump2app();                                              //跳转到APP.
    }
}

/**
 * @name: uint32_t FlashReadWord(uint32_t addr)
 * @brief: 读取指定地址的字(32位数据)
 * @author: fwz
 * @param {：uint32_t} addr	,	写入的地址
 * @return {uint32_t} 返回读到的32位数据
 * @note: 修改记录：初次创建
 */
uint32_t flash_read_word(uint32_t addr)
{
    return *( uint32_t* )addr;
}

/**
 * @name:bool FlashWriteWord(uint32_t WriteAddr, uint32_t data)
 * @brief:不做检查的写入数据（32位）
 * @author: fwz
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
bool flash_write_word(uint32_t WriteAddr, uint32_t data)
{
    HAL_FLASH_Unlock();

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr, data) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return false;
    }
    HAL_FLASH_Lock();
    return true;
}

/**
 * @name:bool FlashErase(uint32_t StartAddress, uint16_t ErasePageNum)
 * @brief:
 * @author: fwz
 * @param {uint32_t}StartAddress:要擦除扇区的开始地址
 * @param {uint16_t}ErasePageNum:要擦除扇区个数
 * @return {type} None
 * @note: 修改记录：初次创建
 */
bool FlashErase(uint32_t StartAddress, uint16_t ErasePageNum)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t               PAGEError = 0;
    uint32_t               sector_adder;

    //地址转换为扇区
    sector_adder = get_sector(StartAddress);
    HAL_FLASH_Unlock();
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    // EraseInitStruct.Banks        = FLASH_BANK_1;
    EraseInitStruct.Sector       = sector_adder;
    EraseInitStruct.NbSectors    = ErasePageNum;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return false;
    }
    else
    {
        HAL_FLASH_Lock();
        return true;
    }
}

/**
 * @name: bool check_update_flag(void)
 * @brief: 检查更新标志位
 * @author: fwz
 * @param {type} None
 * @return {bool} false：表示无更新标志；true:表示有更新标志位
 * @note: 修改记录：初次创建
 */
bool check_update_flag(void)
{
    bool status = false;
    if (flash_read_word(UPDATE_FLASH_FLAG) == 0x55555555)
    {
        status = true;
    }
    return status;
}

/**
 * @name: bool CheckAppExisted(void)
 * @brief:检查APP是否存在
 * @author: xxx
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
bool check_app1_existed(void)
{
    uint8_t i = 0;
    //检测APP1地址是都是0XFF
    for (i = 0; i < 8; i++)
    {
        // 从第5K开始
        if (flash_read_word(BLOCK_APP1_START + MCU_FLASH_PAGE_SIZE * 1 + i * 4) != FLASH_APP_DEFAULT_VALUE)
        {
            return true;  //ËµÃ÷ÒÑ´æÔÚAPP
        }
    }
    return false;  //²»´æÔÚAPP
}
/**
 * @name:void updateInterface_CopyCode_FromApp2ToApp1(void)
 * @brief:将APP2代码复制到APP1中
 * @author: fwz
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
void update_app2_to_app1(void)
{
    uint32_t i = 0;

    for (i = 0; i < BLCOK_APP1_SPACE; i += 4)
    {
        flash_write_word(BLOCK_APP1_START + i, *( uint32_t* )(BLOCK_APP2_START + i));
    }
}
/**
 * @name:void update_app1_to_app2(void)
 * @brief:
 * @author: fwz
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
void update_app1_to_app2(void)
{
    uint32_t i = 0;

    for (i = 0; i < BLCOK_APP1_SPACE; i += 4)
    {
        flash_write_word(BLOCK_APP2_START + i, *( uint32_t* )(BLOCK_APP1_START + i));
    }
}

/******************************************************************************/
/***        静态函数定义  Local Functions                                    ***/
/******************************************************************************/
/**
 * @name:static uint32_t GetSector(uint32_t Address)
 * @brief:地址匹配扇区
 * @author: fwz
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
static uint32_t get_sector(uint32_t Address)
{
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_SECTOR_7;
    }
    else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_SECTOR_8;
    }
    else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_SECTOR_9;
    }
    else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_SECTOR_10;
    }
    else /*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
    {
        sector = FLASH_SECTOR_11;
    }

    return sector;
}
/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/
