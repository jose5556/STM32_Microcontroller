#include "app_threadx.h"

// tire has the diameter of 0.21 m
// RPM = pulse / PPR * (60 / dt_seconds)

// TODO UPDATE THIS FUNCTION TO BETTER CALCULATE SPEED
static uint16_t convertValuesRPM(void)
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
        return 0;
    }

    ULONG delta_ticks = current_time_ticks - last_time_ticks;
    if (delta_ticks == 0)
        return 0;

    float dt = (float)delta_ticks / (float)TX_TIMER_TICKS_PER_SECOND;

    ULONG pulses;
    if (current_count >= last_count)
        pulses = current_count - last_count;
    else
        pulses = (htim1.Init.Period - last_count) + current_count + 1;

    last_count = current_count;
    last_time_ticks = current_time_ticks;

    float rpm_f = ((float)pulses / PPW) * (60.0f / dt);

    if (rpm_f < 0.0f)
        rpm_f = 0.0f;
    if (rpm_f > 65535.0f)
        rpm_f = 65535.0f;

    return (uint16_t)rpm_f;
}

// Thread responsible for reading speed sensor and sending RPM via CAN
VOID thread_SensorSpeed(ULONG thread_input)
{
    char debug[32];
    uint16_t rpm;
    t_can_msg msg;
    UINT ret;

    memset(&msg, 0, sizeof(t_can_msg));
    msg.type = CAN_MSG_SPEED;

    // Reset / start timer, responsible to control STM32 timer
    HAL_TIM_Base_Stop(&htim1);
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    HAL_TIM_Base_Start(&htim1);

    while (1)
    {
        rpm = convertValuesRPM();

        msg.data[0] = (rpm >> 8) & 0xFF;
        msg.data[1] = rpm & 0xFF;

        // Sends data to CAN thread
        ret = tx_queue_send(&can_tx_queue, &msg, TX_NO_WAIT);
        if (ret == TX_QUEUE_ERROR)
        {
            uart_send("CAN TX is giving this specific error!\r\n");
            tx_thread_sleep(500);
            continue ;
        }

        // Debug
        ULONG cr1_reg = htim1.Instance->CR1;
        ULONG cnt_reg = htim1.Instance->CNT;

        int len = snprintf(
            debug,
            sizeof(debug),
            "RPM=%u | CR1=%lu | CNT=%lu\r\n",
            rpm,
            cr1_reg,
            cnt_reg
        );

        if (len > 0 && (size_t)len < sizeof(debug))
            HAL_UART_Transmit(&huart1, (uint8_t *)debug, len, 100);

        tx_thread_sleep(500);
    }
}
