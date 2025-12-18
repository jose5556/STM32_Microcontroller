#include "app_threadx.h"

UINT    init_threads(VOID) 
{
    UINT ret =  TX_SUCCESS;

    ret = tx_thread_create(&threads[0].thread, "CANThread", thread_SensorSpeed, 0,
                                  threads[0].stack, 1024,
                                  THREAD_0_PRIO, THREAD_0_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS)
        uart_send("ERROR! Speed sensor thread creation failed!\r\n");

    ret = tx_thread_create(&threads[1].thread, "TxCanThread", thread_tx_can, 0,
                                  threads[1].stack, 1024,
                                  THREAD_0_PRIO, THREAD_0_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS)
        uart_send("ERROR! CAN TX thread creation failed!\r\n");

    return (ret);
}