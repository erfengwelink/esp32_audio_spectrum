#include "freq_domain.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define amplitude 100//150

static uint32_t amp[SAMPLE_SIZE];
static uint32_t l_amp_limit = 0;
static uint32_t h_amp_limit = 0;

static trigger_func sound_trigger = NULL;

void set_sound_trigger_cb(trigger_func cb)
{
    sound_trigger = cb;
}

void high_limit_set(uint32_t h)
{
    h_amp_limit = h;
}

void low_limit_set(uint32_t l)
{
    l_amp_limit = l;
}

static void fft_display(uint32_t dsize, char* str)
{
    int dmax = 100;
    dsize /= amplitude;
    if(dsize > dmax)
    {
        dsize = 50;
    }
    for (int s = 0; s <= dsize; s = s + 2) 
    {
        printf("*");
    }
    printf(" < %s > strength:%d\n", str, dsize);
}

void spectrum_dump(uint32_t amp[], int n)
{
    int i = 0;

    //char str[10];
    for(i = 3; i < n; i++)//3
    {
        if(amp[i] > l_amp_limit && amp[i] < h_amp_limit)
        {
            if(sound_trigger)
            {
                sound_trigger();
                printf("amp:%u\n", amp[i]);
                //return ;
            }
        }

#if 0
        itoa(i, str, 10);
        fft_display(amp[i], str);
#endif

    }
    printf("\n\n\n\n______________________________ %d \n\n\n\n", i);
}

void handle(int page, TYPE_FFT sample[])
{
    memset(amp, 0, sizeof(amp)/sizeof(uint32_t));
    TYPE_FFT *p = &sample[0];
    if(page)
    {
        p = &sample[SAMPLE_SIZE];
    }
    fft(p, SAMPLE_SIZE);
    get_magnitude(p, amp, SAMPLE_SIZE);
    spectrum_dump(amp, sizeof(amp)/(2 * sizeof(int)));
}