/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE END PTD */
/* USER CODE BEGIN PV */

/* USER CODE BEGIN PD */
t_canFrames canFrames; // Define the global variable here

/* USER CODE END PV */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

t_threads threads[1];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void thread_0_entry(ULONG thread_input);
void uart_send(const char *msg);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @param canFrames: pointer to CAN frames structure
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  
  uart_send("\r\n=== Initializing ThreadX ===\r\n");
  UINT status = tx_thread_create(&threads[0].thread, "CANThread", thread_0_entry, 0,
                                  threads[0].stack, 1024,
                                  THREAD_0_PRIO, THREAD_0_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);

  /* USER CODE END App_ThreadX_MEM_POOL */


  /* USER CODE BEGIN App_ThreadX_Init */
  /* USER CODE END App_ThreadX_Init */
  return (status);
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  canFrames: pointer to CAN frames structure
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN Before_Kernel_Start */
  /* USER CODE END Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN Kernel_Start_Error */

  /* USER CODE END Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

void    uart_send(const char *msg) 
{
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
}

// Thread 0 entry function
void thread_0_entry(ULONG input) 
{
    ULONG counter = 0;
    char buffer[64];
    uint8_t speedData[8] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
    
    uart_send("\r\n=== ThreadX Started! ===\r\n");
    uart_send("CAN Thread running...\r\n");

    while(1) {
        // Send CAN message with Speed header
        if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &canFrames.TxHeader_Speed, speedData) == HAL_OK) {
            snprintf(buffer, sizeof(buffer), "CAN Msg sent: %lu\r\n", counter++);
            uart_send(buffer);
        } else {
            uart_send("CAN send failed!\r\n");
            HAL_FDCAN_Stop(&hfdcan1);
            HAL_FDCAN_Start(&hfdcan1);
            tx_thread_sleep(100);
            continue ;
        }
        
        // Sleep for 1 second
        tx_thread_sleep(1000);
    }
}

/* USER CODE END 1 */
