/*
 * @FileName:
 * @Description:
 * @Version:
 * @Author: lzc
 * @Date: 2020-09-12 10:17:34
 * @LastEditors: lzc
 * @LastEditTime: 2020-09-17 15:24:32
 */
#ifndef _TEST_HTTP_H__
#define _TEST_HTTP_H__
#include "MCU_Config.h"
#include "UpdateIAP.h"
#include "lwip/apps/http_client.h"
#include "lwip/apps/httpd.h"
#include "lwip/apps/httpd_opts.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "pppapp.h"
#include "task.h"
//#include "uart_driver.h"
#include <string.h>
/* GET request with host */
// keep-alive
#define HTTPC_REQ_10_HOST                                                           \
    "GET %s HTTP/1.1\r\n" /* URI */                                                 \
    "User-Agent: %s\r\n"  /* User-Agent */                                          \
    "Accept: */*\r\n"                                                               \
    "Host: %s\r\n" /* server name */                                                \
    "Range:bytes=%d-%d\r\n"                                                         \
    "Connection: keep-alive\r\n" /* we don't support persistent connections, yet */ \
    "\r\n"
#define HTTPC_REQ_10_HOST_FORMAT(uri, srv_name, start_bit, stop_bit) HTTPC_REQ_10_HOST, uri, HTTPC_CLIENT_AGENT, srv_name, start_bit, stop_bit
//通过结构体实现U32和u8转化
typedef union
{
    struct
    {
        uint8_t u8_data[4];
    } CDATA;
    uint32_t u32_data;
} U32_U8_DATA_STRUCT;
extern bool ota_over_flag;

void http_connect();
void http_init(void);
void http_poll(void);
#endif
