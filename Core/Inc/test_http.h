/*
 * @FileName:
 * @Description:
 * @Version:
 * @Author: lzc
 * @Date: 2020-09-12 10:17:34
 * @LastEditors: lzc
 * @LastEditTime: 2020-09-14 14:57:46
 */
#ifndef _TEST_HTTP_H__
#define _TEST_HTTP_H__
#include "lwip/apps/http_client.h"
#include "lwip/apps/httpd.h"
#include "lwip/apps/httpd_opts.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "pppapp.h"
#include "uart_driver.h"
#include <string.h>
/* GET request with host */
#define HTTPC_REQ_10_HOST                                                           \
    "GET %s HTTP/1.1\r\n" /* URI */                                                 \
    "User-Agent: %s\r\n"  /* User-Agent */                                          \
    "Accept: */*\r\n"                                                               \
    "Host: %s\r\n" /* server name */                                                \
    "Range:bytes=%d-%d\r\n"                                                         \
    "Connection: keep-alive\r\n" /* we don't support persistent connections, yet */ \
    "\r\n"
#define HTTPC_REQ_10_HOST_FORMAT(uri, srv_name, start_bit, stop_bit) HTTPC_REQ_10_HOST, uri, HTTPC_CLIENT_AGENT, srv_name, start_bit, stop_bit

void http_connect();
void http_init(void);
void http_poll(void);
#endif
