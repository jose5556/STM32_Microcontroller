#include "app_threadx.h"

VOID thread_tx_can(UINT thread_input)
{
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &canFrames.TxHeader_Speed, thread_input) == HAL_OK)
        uart_send("CAN enviado!\r\n");
    else
        uart_send("Erro ao enviar CAN!\r\n");
}
