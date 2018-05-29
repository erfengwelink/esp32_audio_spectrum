#include "erfeng_fft.h"
#include "audio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define ADC_WIDTH ADC_WIDTH_BIT_10


#define SAMPLES 1024             // Must be a power of 2
#define FILTER_LIMIT 500
#define SAMPLE_INTERVAL 50

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

static int s_samples = SAMPLES;
static int s_filter_limit = FILTER_LIMIT;
static int s_interval = SAMPLE_INTERVAL;
static int out[s_samples];
static TYPE_FFT in[s_samples];

static void check_efuse()
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

void app_adc_init()
{
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();
    //Configure ADC
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH);
        adc1_config_channel_atten(channel, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);
}

int set_samples(int s)
{
    s_samples = s;
    return 0;
}

int set_filter_limit(int f)
{
    s_filter_limit = f;
    return 0;
}

int set_interval(int i)
{
    s_interval = i;
    return 0;
}

static void fft_display(int dsize, char* str)
{
    int dmax = 500;
    dsize /= amplitude;
    if(dsize > dmax)
    {
        dsize = dmax;
    }
    if(dsize > 40){
        for (int s = 0; s <= dsize; s = s + 2) {
            printf("*");
        }
        printf("    %s strength:%d\n", str, dsize);
    }
}

void frequence_domain_shown()
{
#if 1
        for(int i=2; i<s_samples/2; i++)
        {
            if (out[i] > FILTER_CNT)
            {
                if(i<=2){
                    //fft_display(out[i], " [125HZ]");
                }
                else if(i > 2   && i <= 4 )
                {
                    fft_display(out[i], " [250HZ]");
                }else if (i > 4   && i <= 7 ) {
                    fft_display(out[i], " [500HZ]");
                }else if (i > 7   && i <= 15 ) {
                    fft_display(out[i], " [1000HZ]");
                }else if (i > 15  && i <= 40 ) {
                    fft_display(out[i], " [2000HZ]");
                }else if (i > 40  && i <= 70 ) {
                    fft_display(out[i], " [4000HZ]");
                }else if (i > 70  && i <= 288 ) {
                    fft_display(out[i], " [8000HZ]");
                }else if (i > 288           ) {
                    fft_display(out[i], " [16000HZ]");
                }
            }
        }
#endif
}

void audio_fft_task(void *ignore)
{
    do{
        //temp_t = xTaskGetTickCount();
        for (int i = 0; i < s_samples; i++) {
            in[i].real = adc1_get_raw((adc1_channel_t)channel);
            in[i].imag  = 0.0f; 
        }

        fft(in, s_samples);
        get_magnitude(in, out, s_samples);
        frequence_domain_shown();
        vTaskDelay(pdMS_TO_TICKS(s_interval)); 
    }while(0);
}

void app_main()
{
    app_adc_init();


}