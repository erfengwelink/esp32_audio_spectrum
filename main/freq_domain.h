#ifndef _FREQ_H_
#define _FREQ_H_

#include "erfeng_fft.h"

typedef void (*fft_func)(int page, TYPE_FFT *sample);
typedef void (*trigger_func)(void);

void low_limit_set(uint32_t l);
void high_limit_set(uint32_t h);
void set_sound_trigger_cb(trigger_func cb);
void handle(int page, TYPE_FFT sample[]);

#endif
