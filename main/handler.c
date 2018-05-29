#include "handler.h"

static fft_func hfft = NULL;

void hanler_cb_set(fft_func cb)
{
    hfft = cb;
}

bool cb_is_null()
{
    return (hfft == NULL) ? true : false;
}

void app_do_fft(int page, TYPE_FFT sample[])
{
    hfft(page, sample);
}