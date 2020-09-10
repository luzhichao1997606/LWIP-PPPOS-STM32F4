#ifndef __PPP_APP_H__
#define __PPP_APP_H__

#include "cycle_queue.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "netif/ppp/pppapi.h"
#include "uart_driver.h"
#include <string.h>

extern UART_HandleTypeDef huart6;

uint8_t lwip_comm_init(void);
void    tcp_connect_init(void);
#endif
