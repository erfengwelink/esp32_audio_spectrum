#include "app_adc.h"
#include "time_domain.h"
#include "handler.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SAMPLING_FREQUENCY 20000 //40000

static void speeker()
{
    printf("%s\n", __func__);
}

static int sround(double d)
{
    int i = (int)d;
    if(i - d > 0)
        i += 1;
    return i;
}

void app_main()
{
    int period = sround(1000000 * (1.0 / SAMPLING_FREQUENCY));
    printf("period:%d\n", period);
    app_adc_init();
    low_limit_set(10000);
    high_limit_set(20000);
    set_sound_trigger_cb(speeker);
    init_timer(period, handle);
    xTaskCreate(timer_evt_task, "timer_evt_task", 2048, NULL, 5, NULL);
}