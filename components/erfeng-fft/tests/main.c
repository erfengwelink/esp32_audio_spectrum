#include "audio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main()
{
    app_adc_init();
    xTaskCreate(audio_fft_task,"audio_fft_task", 10*2048, NULL, 5, NULL);
}


