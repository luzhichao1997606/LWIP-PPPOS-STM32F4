/*
 * @FileName:
 * @Description:
 * @Version:
 * @Author: lzc
 * @Date: 2020-09-08 09:05:13
 * @LastEditors: lzc
 * @LastEditTime: 2020-09-12 10:41:22
 */
#ifndef __PPP_APP_H__
#define __PPP_APP_H__

#include "cmsis_os.h"
#include "cycle_queue.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "mqtt.h"
#include "netif/ppp/pppapi.h"
#include "test_http.h"
#include "uart_driver.h"
#include <string.h>
extern UART_HandleTypeDef huart6;

uint8_t lwip_comm_init(void);
void    tcp_connect_init(void);
#endif
