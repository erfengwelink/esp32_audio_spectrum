#include "freq_domain.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define amplitude 100//150

static uint32_t amp[SAMPLE_SIZE];

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
    int dbfs = 0;
    char str[10];
    for(i = 3; i < n; i++)//3
    {
        itoa(i, str, 10);
        fft_display(amp[i], str);
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