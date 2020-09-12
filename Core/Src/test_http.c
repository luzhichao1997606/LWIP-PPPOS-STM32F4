/*
 * @FileName:
 * @Description:
 * @Version:
 * @Author: lzc
 * @Date: 2020-09-12 10:17:21
 * @LastEditors: lzc
 * @LastEditTime: 2020-09-12 19:07:53
 */
#include "test_http.h"

// http处理函数
static err_t HttpClientGetFileReceive(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err)
{
    printf("hello\r\n");
    printf("***************************************************************\r\n");
    printf("********************      len is %d     ************************\r\n", p->len);
    printf("********************    payload is %s   ************************\r\n", p->payload);
    printf("***************************************************************\r\n");
    return ERR_OK;
}
/**
 * @name: http_get_url
 * @brief: 获取URL
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
void http_get_url()
{
    ip_addr_t server_addr;

    err_t ErrorStatus_http;
    IP4_ADDR(&server_addr, 47, 110, 177, 54);  //合宙IP
    // IP4_ADDR(&server_addr, 125, 123, 142, 71);   //本机查看IP（DEBUG）
    char domain_name[100] = "http://openluat-backend.oss-cn-hangzhou.aliyuncs.com";
    domain_name[17]       = '\0';

    char                url[100] = "/upgrade_firmware/test.bin_20200912142838784755";
    u16_t               portnum  = 80;
    httpc_connection_t* conn_settings_try;
    conn_settings_try->use_proxy = 0;  //不使用代理
    ErrorStatus_http             = httpc_get_file(&server_addr, portnum, url, conn_settings_try, HttpClientGetFileReceive, NULL, NULL);
    printf("ErrorStatus_http is %d      \r\n  ", ErrorStatus_http);
}
void http_result(void* arg, httpc_result_t httpc_result, u32_t rx_content_len, u32_t srv_res, err_t err)
{
    printf("result %d , len is %d", httpc_result, rx_content_len);
}
/**
 * @name: http_connect
 * @brief: 连接以获取数据
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */

void http_connect()
{
    ip_addr_t server_addr;

    err_t ErrorStatus_http;
    // IP4_ADDR(&server_addr, 47, 110, 177, 54);  //合宙IP
    IP4_ADDR(&server_addr, 125, 123, 142, 71);  //本机查看IP（DEBUG）
    char domain_name[100] = "http://openluat-backend.oss-cn-hangzhou.aliyuncs.com";
    domain_name[17]       = '\0';
    char url[100]         = "/upgrade_firmware/test.bin_20200912154515808854";

    u16_t               portnum = 2080;
    httpc_connection_t* conn_settings_try;
    conn_settings_try->use_proxy = 0;  //不使用代理
    conn_settings_try->result_fn = http_result;
    ErrorStatus_http             = httpc_get_file(&server_addr, portnum, url, conn_settings_try, HttpClientGetFileReceive, NULL, NULL);
    if (ErrorStatus_http == HTTPC_RESULT_OK)
    {
        printf("SUCCESS !!");
    }
    else
    {
        printf("ErrorStatus_http is %d      \r\n  ", ErrorStatus_http);
    }
}
