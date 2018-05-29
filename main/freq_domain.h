#ifndef _FREQ_H_
#define _FREQ_H_

#include "erfeng_fft.h"

typedef void (*fft_func)(int page, TYPE_FFT *sample);

void handle(int page, TYPE_FFT sample[]);

#endif
