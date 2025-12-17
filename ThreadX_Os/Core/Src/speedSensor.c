#include "app_threadx.h"

#define WHEEL_DIAMETER  0.212f
#define PPR             20.0f

#ifndef TX_TIMER_TICKS_PER_SECOND
    #define TX_TIMER_TICKS_PER_SECOND 1000
#endif

// tire has the diameter of 0.21 m
// RPM = pulse / PPR * (60 / dt_seconds)

static float convertValuesRPM(void)
{
    static ULONG last_time_ticks = 0;
    static ULONG last_count = 0;
    static UINT is_first_run = 1;

    ULONG current_time_ticks = tx_time_get();
    ULONG current_count = htim1.Instance->CNT;

    if (is_first_run)
    {
        last_count = current_count;
        last_time_ticks = current_time_ticks;
        is_first_run = 0;
        return 0.0f;
    }

    ULONG delta_ticks = current_time_ticks - last_time_ticks;
    float dt = (float)delta_ticks / (float)TX_TIMER_TICKS_PER_SECOND;

    ULONG pulses;
    if (current_count >= last_count)
        pulses = current_count - last_count;
    else
        pulses = (htim1.Init.Period - last_count) + current_count + 1;

    last_count = current_count;
    last_time_ticks = current_time_ticks;

    // Cálculo do RPM
    float rpm = ((float)pulses / PPR) * (60.0f / dt);
    return rpm;

}

VOID thread_SensorSpeed(ULONG thread_input)
{
    char msg[32];

    HAL_TIM_Base_Stop(&htim1);
    HAL_TIM_Base_Start(&htim1);

    while (1)
    {
        ULONG cr1_reg = htim1.Instance->CR1;
        ULONG cnt_reg = htim1.Instance->CNT;

        snprintf(msg, sizeof(msg), "DEBUG: CR1=%lu | CNT=%lu\r\n", cr1_reg, cnt_reg);
        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);

        tx_thread_sleep(500);

        g_speed = convertValuesRPM();
    }
}
