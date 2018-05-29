#include "time_domain.h"
#include "app_adc.h"
#include "soc/timer_group_struct.h"
#include "driver/timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define TIMER_INTR_SEL TIMER_INTR_LEVEL                  // Timer level interrupt
#define TIMER_GROUP TIMER_GROUP_0                        // Test on timer group 0
#define TIMER_DIVIDER 2                                  // Hardware timer clock divider
#define TIMER_INTERVAL0_SEC (0.000001)                    // [50us interval] test interval for timer 0 [1000usec/1.0msec(0.001),100usec/0.1msec(0.0001),8.5usec/0.0085msec(0.00001)]
#define TIMER_CPU_CLOCK 160000000L                       // CPU Clock 160Mhz
#define TIMER_SCALE (TIMER_BASE_CLK / TIMER_DIVIDER)     // used to calculate counter value
#define TIMER_SCALE160 (TIMER_CPU_CLOCK / TIMER_DIVIDER) // used to calculate counter value

typedef int timer_event_t;
static int s_cur_index = 0;
xQueueHandle timer_queue;
static TYPE_FFT sam[2 * SAMPLE_SIZE];

int get_sample_page()
{
    return (s_cur_index > (SAMPLE_SIZE - 1) ? 1 : 0);
}

static void timer_expire_isr()
{
    TIMERG0.hw_timer[0].update = 1;
    TIMERG0.int_clr_timers.t0 = 1;
    
    sam[s_cur_index].real = app_get_adc();
    sam[s_cur_index].imag = 0.0f;

    TIMERG0.hw_timer[0].config.alarm_en = 1;

    if(!cb_is_null())
    {
        if(s_cur_index == 0 || s_cur_index == 1024)
        {
            timer_event_t evt;
            int page = get_sample_page();
            evt = page;
            xQueueSendFromISR(timer_queue, &evt, NULL);
        }
    }

    if (s_cur_index > SAMPLE_SIZE * 2 - 1)
    {
        s_cur_index = 0;
    }
    else
    {
        s_cur_index++;
    }  
}

void init_timer(int Nus, fft_func func)
{
    if(!cb_is_null())
    {
        printf("already have fft handle function!!!\n");
        return ;
    }
    else{
        hanler_cb_set(func);
    }
    timer_queue = xQueueCreate(10, sizeof(timer_event_t));

    int timer_group = TIMER_GROUP;
    int timer_idx = TIMER_0;

    // Configure timer
    timer_config_t config;
    config.alarm_en = 1;
    config.auto_reload = 1;
    config.counter_dir = TIMER_COUNT_UP;
    config.divider = TIMER_DIVIDER;
    config.intr_type = TIMER_INTR_SEL;
    config.counter_en = TIMER_PAUSE;
    timer_init(timer_group, timer_idx, &config);

    timer_pause(timer_group, timer_idx);
    timer_set_alarm_value(timer_group, timer_idx, Nus * TIMER_INTERVAL0_SEC * TIMER_SCALE);

    timer_enable_intr(timer_group, timer_idx);
    timer_isr_register(timer_group, timer_idx,
                       timer_expire_isr, (void *)timer_idx, ESP_INTR_FLAG_LOWMED, NULL);
    timer_start(timer_group, timer_idx);
}

void timer_evt_task(void *arg)
{
    int page = 0;
    while (1) {
        timer_event_t evt;
        BaseType_t type = xQueueReceive(timer_queue, &evt, portMAX_DELAY);
        
        if(pdTRUE == type)
        {
            page = evt;
            //printf("page:%d \n", page);
            if(!cb_is_null())
            {
                app_do_fft(page, sam);
            }
        }
    }

}