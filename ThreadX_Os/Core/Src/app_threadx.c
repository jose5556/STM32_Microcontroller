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
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_THREAD thread_0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void thread_0_entry(ULONG thread_input);
void uart_send(const char *msg);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  
  UINT status = tx_thread_create(&thread_0, "Thread 0", thread_0_entry, 0,
                                  memory_ptr, 1024,
                                  1, 1,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);

  /* USER CODE END App_ThreadX_MEM_POOL */
  /* USER CODE BEGIN App_ThreadX_Init */
  /* USER CODE END App_ThreadX_Init */
  return status;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
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

// UART helper function
extern UART_HandleTypeDef huart1;

void uart_send(const char *msg) {
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
}

// Thread 0 entry function
void thread_0_entry(ULONG thread_input) {
    ULONG counter = 0;
    char buffer[64];
    
    uart_send("\r\n=== ThreadX Started! ===\r\n");
    uart_send("Thread 0 running...\r\n");
    
    while(1) {
        // Debug message
        snprintf(buffer, sizeof(buffer), "Thread 0 alive: %lu\r\n", counter++);
        uart_send(buffer);
        
        // Sleep for 1 second
        tx_thread_sleep(1000);
        
        // Delete after 5 iterations
        if (counter >= 5) {
            uart_send("\r\nThread 0 deleting itself...\r\n");
            tx_thread_delete(&thread_0);
            // Não executa daqui para frente
        }
    }
}

/* USER CODE END 1 */
