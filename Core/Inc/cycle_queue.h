/*
 * @FileName:
 * @Description:
 * @Version:
 * @Author: lzc
 * @Date: 2020-09-15 17:50:27
 * @LastEditors: lzc
 * @LastEditTime: 2020-09-17 13:29:18
 */
#ifndef __CYCLEQUEUE__
#define __CYCLEQUEUE__
#include <stdbool.h>
#include <stdint.h>

#define USART_TCPSENDING 1
#define USART_TCPSENDOVER 0

#define MaxQueueSize 25     // 25
#define LEFTRAMSIZE 2048    // 640
#define RECEIVEBUFLEN 4096  // 2048
typedef struct DataType_t
{
    uint8_t* index;
    int16_t  size;
} DataType;

typedef struct seq
{
    DataType queue[MaxQueueSize];
    uint8_t* heapcache;
    uint8_t* currentCache;
    int8_t   rear;
    int8_t   front;
    int8_t   count;
    int16_t  leftram;
    bool     reConnectState;
} SeqCQueue;

typedef struct uUSART_RECEIVETYPE_t
{
    uint8_t Send_flag : 1;
    uint8_t usartDMA_rxBuf[RECEIVEBUFLEN];
} USART_RECEIVETYPE;

int              QueueNotEmpty(SeqCQueue* Q);
void             QueueInitiate(SeqCQueue* Q);
int              QueueAppend(SeqCQueue* Q, DataType x);
int              QueueDelete(SeqCQueue* Q, DataType* d);
extern SeqCQueue seqCQueue;
void             GetQueueFront(void);

extern USART_RECEIVETYPE UsartType;

extern SeqCQueue seqCQueue;
#endif
