/*
 * @FileName:
 * @Description:
 * @Version:
 * @Author: lzc
 * @Date: 2020-09-12 10:17:21
 * @LastEditors: lzc
 * @LastEditTime: 2020-09-15 10:22:56
 */
#include "test_http.h"
#define TEST_START_BIT 0
#define TEST_STOP_BIT 511

struct tcp_pcb* tcp_client_pcb;
u16_t           All_Data_lens      = 0;
u16_t           start_bits_of_HTTP = 0;
u16_t           stop_bits_of_HTTP  = 511;
bool            http_get_lens_flag = false;
char            url[100]           = "/upgrade_firmware/test.bin_20200914091821368896";
bool            packet_over_flag   = true;
bool            ota_over_flag      = false;
//合宙的服务器的路径
char* true_server_name = "openluat-backend.oss-cn-hangzhou.aliyuncs.com";

static void HTTP_Client_Initialization(void);
// http处理函数
static err_t HttpClientGetFileReceive(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err)
{
    err_t r;
    printf("hello\r\n");
    printf("***************************************************************\r\n");
    printf("********************      len is %d     ************************\r\n", p->len);
    printf("********************    payload is %s   ************************\r\n", p->payload);
    printf("***************************************************************\r\n");

    mem_free(my_connection);
    my_connection = NULL;
    if (tpcb != NULL)
    {
        err_t r;
        altcp_arg(tpcb, NULL);
        altcp_recv(tpcb, NULL);
        altcp_err(tpcb, NULL);
        altcp_poll(tpcb, NULL, 0);
        altcp_sent(tpcb, NULL);
        r = altcp_close(tpcb);
        if (r != ERR_OK)
        {
            altcp_abort(tpcb);
            return ERR_ABRT;
        }
    }
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
    err_t     ErrorStatus_http;
    IP4_ADDR(&server_addr, 47, 110, 177, 54);  //合宙IP
    // IP4_ADDR(&server_addr, 125, 123, 142, 71);  //本机查看IP（DEBUG）
    char domain_name[100] = "http://openluat-backend.oss-cn-hangzhou.aliyuncs.com";
    domain_name[17]       = '\0';

    u16_t               portnum = 80;
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
/*****************************************************************************************************************************************/
/***********************************************************
  函数名称：Find_string(char *pcBuf,char*left,char*right, char *pcRes)
  函数功能：寻找特定字符串
  入口参数：
           char *pcBuf 为传入的字符串
           char*left   为搜索字符的左边标识符  例如："["
           char*right  为搜索字符的右边标识符  例如："]"
                     char *pcRes 为输出转存的字符串数组
  返回值：用来校验是否成功，无所谓的。
  备注： left字符需要唯一，right字符从left后面开始唯一即可
 服务器下发命令举例：+MQTTPUBLISH: 0,0,0,0,/device/NB/zx99999999999999_back,6,[reastrobot]
***********************************************************/
int find_string(char* pcBuf, char* left, char* right, char* pcRes)
{
    char* pcBegin = NULL;
    char* pcEnd   = NULL;
    //   int left_len = 0;
    pcBegin = strstr(pcBuf, left);  //取出左边数据
    if (pcBegin == NULL)
    {
        printf("left string name not found!\n");
        return 0;
    }
    //   left_len = strlen(left);
    pcEnd = strstr(pcBegin + strlen(left), right);  //扫描右边标识
    // pcEnd = strstr(pcBegin + left_len, right);  //扫描右边标识
    if (pcEnd == NULL || pcBegin > pcEnd)
    {
        printf("right string name not found!\n");
        return 0;
    }
    else
    {
        pcBegin += strlen(left);
        memcpy(pcRes, pcBegin, pcEnd - pcBegin);
        return 1;
    }
}
/**
 * @name:
 * @brief:
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
//例子
/*
HTTP/1.1 206 Partial Content
Server: AliyunOSS
Date: Mon, 14 Sep 2020 07:58:55 GMT
Content-Type: application/octet-stream
Content-Length: 13
Connection: keep-alive
x-oss-request-id: 5F5F22BF4CE483343930BFB9
Content-Range: bytes 0-12/19299
Accept-Ranges: bytes
ETag: "5E2963B27E8C3284FC8CEC532B49155D"
Last-Modified: Mon, 14 Sep 2020 01:18:21 GMT
x-oss-object-type: Normal
x-oss-hash-crc64ecma: 14901171418814596131
x-oss-storage-class: Standard
x-oss-server-time: 1

hello world!~	0
*/
u16_t temp_num               = 0;
u16_t Packet_Num             = 0;
char  http_first_packet_len  = 0;
u16_t http_second_packet_len = 0;
char* http_get_data_string(char* http_data_Payload)
{
    char file_bin_data[512]  = { 0 };
    char file_temp_data[600] = { 0 };
    char Range_data[20]      = { 0 };
    bool http_head_flag      = false;

    memset(file_bin_data, 0, strlen(file_bin_data));
    memset(file_temp_data, 0, strlen(file_temp_data));
    memset(Range_data, 0, strlen(Range_data));
    //判断是否有HTTP报文头
    if (strstr(http_data_Payload, "HTTP/1.1") == NULL)
    {
        http_head_flag = false;
    }
    else
    {
        http_head_flag = true;
    }
    //获取长度
    if (!http_get_lens_flag)
    {
        http_get_lens_flag = true;
        find_string(strstr(http_data_Payload, "Content-Range"), "/", "\r\n", Range_data);
        All_Data_lens = atoi(Range_data);
        printf("%d", All_Data_lens);
        Packet_Num = All_Data_lens / 512;
        if (All_Data_lens % 512)
        {
            Packet_Num++;
        }
        printf("!!!!!!!!!!!!!!!!!!!!!!!  %d  !!!!!!!!!!!!!!!!!!!!!!! %d \r\n", atoi(Range_data), Packet_Num);
    }
    //有报文头时
    if (http_head_flag)
    {

        memset(file_bin_data, 0, strlen(file_bin_data));
        memset(file_temp_data, 0, strlen(file_temp_data));
        strstr(http_data_Payload, "x-oss-server-time");
        memcpy(file_temp_data, strstr(http_data_Payload, "x-oss-server-time"), strlen(strstr(http_data_Payload, "x-oss-server-time")));
        http_first_packet_len = strlen(strstr(file_temp_data, "\r\n") + 4) - 2;  //需要去除结尾符
        memcpy(file_bin_data, strstr(file_temp_data, "\r\n") + 4, http_first_packet_len);
        printf("!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!! \r\n");
        printf("file_bin_data \r\n%s\r\n", file_bin_data);
        printf("!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!! \r\n");
        printf("\r\nhttp_first_packet_len %d\r\n", http_first_packet_len);
        printf("!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!! \r\n");
    }
    else
    {
        printf("Second packet\r\n");
        memset(file_bin_data, 0, strlen(file_bin_data));
        memset(file_temp_data, 0, strlen(file_temp_data));
        http_second_packet_len = 512 - http_first_packet_len;
        memcpy(file_bin_data, http_data_Payload, http_second_packet_len);
        printf("http_second_packet_len \r\n%d\r\n", http_second_packet_len);
        printf("!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!! \r\n");
        printf("file_bin_data \r\n%s\r\n", file_bin_data);
        printf("!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!! \r\n");
        temp_num++;
        printf("!!!!!!!!!!!!!!!!!!!!!!!%d!!!!!!!!!!!!!!!!!!!!!!! \r\n", temp_num);
        //更新起始和结束位置
        start_bits_of_HTTP = stop_bits_of_HTTP + 1;
        stop_bits_of_HTTP  = stop_bits_of_HTTP + 512;
        //接收完毕
        if (stop_bits_of_HTTP >= All_Data_lens || temp_num >= Packet_Num)
        {
            //删除线程或者置位标志位
            ota_over_flag = true;
        }
    }
    //返回bin文件的数据
    return file_bin_data;
}

/**
 * @name:
 * @brief:
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
char* composite_send_string(char* buff, u16_t Start_bit, u16_t Stop_bit)
{
    //合成发送字符串
    /* 创建一个建立连接的问候字符串*/
    sprintf(buff, HTTPC_REQ_10_HOST, url, "lwIP/", true_server_name, Start_bit, Stop_bit);
    return buff;
}
/**
 * @name:
 * @brief:
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
/* TCP客户端连接服务器错误回调函数 */
static void HTTPClientConnectError(void* arg, err_t err)
{
    /* 重新启动连接 */
    HTTP_Client_Initialization();
}
/**
 * @name:
 * @brief:
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
/* TCP客户端接收到数据后的数据处理回调函数 */
static err_t TCPClientCallback(void* arg, struct tcp_pcb* pcb, struct pbuf* tcp_recv_pbuf, err_t err)
{
    struct pbuf* tcp_send_pbuf;
    char         echoString[] = "This is the server content echo:\r\n";

    if (tcp_recv_pbuf != NULL)
    {
        /* 更新接收窗口 */
        tcp_recved(pcb, tcp_recv_pbuf->tot_len);
        /* 将接收到的服务器内容回显*/
        printf("***************************************************************\r\n");
        printf("%s\r\n", echoString);
        printf("***************************************************************\r\n");

        tcp_send_pbuf = tcp_recv_pbuf;
        printf("***************************************************************\r\n");
        printf("%s\r\n", tcp_send_pbuf->payload);
        printf("***************************************************************\r\n");

        http_get_data_string(tcp_send_pbuf->payload);

        //存储
        packet_over_flag = true;  //数据包存储完成
        if (ota_over_flag)
        {
            packet_over_flag = false;
        }
        pbuf_free(tcp_recv_pbuf);
    }
    else if (err == ERR_OK)
    {
        tcp_close(pcb);
        HTTP_Client_Initialization();
        return ERR_OK;
    }

    return ERR_OK;
}
/**
 * @name:
 * @brief:
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
static err_t http_Client_connected(void* arg, struct tcp_pcb* pcb, err_t err)
{
    char send_data[600];
    char clientString[] = "This is a new client connection.\r\n";

    /* 配置接收回调函数 */
    tcp_recv(pcb, TCPClientCallback);
    memset(send_data, 0, strlen(send_data));
    printf("***************************************************************\r\n");
    /* 发送一个建立连接的问候字符串*/
    // sprintf(send_data, HTTPC_REQ_10_HOST, url, "lwIP/", true_server_name, TEST_START_BIT, TEST_STOP_BIT);
    // printf("Send Data %s", send_data);
    printf("***************************************************************\r\n");

    printf("connect success!!\r\n");

    return ERR_OK;
}
/**
 * @name:create_http_connect_httpway
 * @brief:
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
/* TCP客户端初始化 */
static void HTTP_Client_Initialization(void)
{

    ip_addr_t ipaddr;
    /* 将目标服务器的IP写入一个结构体，为pc机本地连接IP地址 */
    IP4_ADDR(&ipaddr, 47, 110, 177, 54);
    /* 为tcp客户端分配一个tcp_pcb结构体    */
    tcp_client_pcb = tcp_new();
    /* 绑定本地端号和IP地址 */
    tcp_bind(tcp_client_pcb, IP_ADDR_ANY, 4000);
    if (tcp_client_pcb != NULL)
    {
        /* 与目标服务器进行连接，参数包括了目标端口和目标IP */
        tcp_connect(tcp_client_pcb, &ipaddr, 80, http_Client_connected);
        tcp_err(tcp_client_pcb, HTTPClientConnectError);
        for (uint16_t i = 0; i < 5; i++)
        {
            if (tcp_client_pcb->state == 4)
            {
                break;
            }
            osDelay(1000);
        }
    }
}
/**
 * @name:http_init
 * @brief:
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
void http_init(void)
{
    HTTP_Client_Initialization();
}
/**
 * @name:http_poll
 * @brief:数据处理
 * @author: lzc
 * @param {type} None
 * @return {type} None
 * @note: 修改记录：初次创建
 */
void http_poll(void)
{
    char send_data[600];
    memset(send_data, 0, strlen(send_data));
    // tcp_write(pcb, clientString, strlen(clientString), 0);
    if (tcp_client_pcb != NULL && tcp_client_pcb->state == 4 && packet_over_flag)
    {
        packet_over_flag = false;
        printf("***************************************************************\r\n");
        // sprintf(send_data, HTTPC_REQ_10_HOST, url, "lwIP/", true_server_name, 0, 12);
        composite_send_string(send_data, start_bits_of_HTTP, stop_bits_of_HTTP);
        while (ERR_OK != tcp_write(tcp_client_pcb, send_data, strlen(send_data), TCP_WRITE_FLAG_COPY))
            osDelay(10);
        printf("%s\r\n", composite_send_string(send_data, start_bits_of_HTTP, stop_bits_of_HTTP));
        printf("***************************************************************\r\n");
        osDelay(100);
    }
    osDelay(1000);
}