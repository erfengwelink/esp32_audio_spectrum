#ifndef _HANDLER_H_
#define _HANDLER_H_
#include <stdbool.h>
#include "freq_domain.h"

bool cb_is_null();
void hanler_cb_set(fft_func cb);
void app_do_fft(int page, TYPE_FFT sample[]);

#endif /* _HANDLER_H_ */