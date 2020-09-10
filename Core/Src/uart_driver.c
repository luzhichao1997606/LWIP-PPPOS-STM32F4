#include "uart_driver.h"
#include "cmsis_os.h"
#include "cycle_queue.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
extern UART_HandleTypeDef huart4;
extern DMA_HandleTypeDef  hdma_uart4_rx;
extern DMA_HandleTypeDef  hdma_uart4_tx;
extern DMA_HandleTypeDef  hdma_usart6_rx;

SeqCQueue seqCQueue;
//#define DEBUG_INFO(fmt,args...)  printf(fmt, ##args)

/*-------------------private variable----------------------------------*/
DataType          cache;
USART_RECEIVETYPE UsartType;

/*-------------------public function----------------------------------*/

void MX_UART_Config(UART_HandleTypeDef* huart, int baud)
{
// DEBUG_INFO("Change the baud rate:%d\r\n", baud );
UART_REINIT:
    HAL_UART_DeInit(huart);
    huart->Init.BaudRate     = baud;
    huart->Init.WordLength   = UART_WORDLENGTH_8B;
    huart->Init.StopBits     = UART_STOPBITS_1;
    huart->Init.Parity       = UART_PARITY_NONE;
    huart->Init.Mode         = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(huart) != HAL_OK)
    {
        // DEBUG_INFO("initialization failed\r\n" );
        osDelay(1000);
        goto UART_REINIT;
    }

    HAL_UART_Receive_DMA(huart, seqCQueue.currentCache, LEFTRAMSIZE);

    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);

    if (huart->Instance == USART6)
    {

        /* UART3_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);  // add code
        HAL_NVIC_EnableIRQ(USART6_IRQn);

        USART6->SR;

        HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);  // add code
        HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

        HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 5, 0);  // add code
        HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
    }
    else
    {
        printf("ERROR PARA");
    }
}

void InitPeripherals(void)
{
    QueueInitiate(&seqCQueue);
}

void UartSendData(UART_HandleTypeDef* huart, uint8_t* pdata, uint16_t Length)
{

    if (HAL_UART_Transmit(huart, pdata, Length, 100) != HAL_OK)
    {
        HAL_UART_DMAResume(huart);
        HAL_UART_Receive_DMA(huart, seqCQueue.currentCache, LEFTRAMSIZE);
    }
}

void UartIdleReceiveData(UART_HandleTypeDef* huart, SeqCQueue* Queue)
{
    printf("Queue");
    if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
    {
        printf("Queue init");
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        HAL_UART_DMAStop(huart);
        uint32_t temp = hdma_usart6_rx.Instance->NDTR;
        if ((LEFTRAMSIZE - temp) != 0x00 && (LEFTRAMSIZE - temp) < LEFTRAMSIZE)
        {
            cache.size  = (LEFTRAMSIZE - temp);
            cache.index = Queue->currentCache;
            printf("Queue Add");
            if (QueueAppend(Queue, cache))
            {
                Queue->leftram += cache.size;
                if ((RECEIVEBUFLEN - Queue->leftram) >= LEFTRAMSIZE)
                {
                    Queue->currentCache += cache.size;
                }
                else
                {
                    Queue->leftram      = 0;
                    Queue->currentCache = Queue->heapcache;
                }
            }
        }
        else
        {
            HAL_UART_DMAResume(huart);
        }
        HAL_UART_Receive_DMA(huart, Queue->currentCache, LEFTRAMSIZE);
    }
}

/********************************************************************************/
