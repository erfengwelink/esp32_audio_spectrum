#ifndef _TIME_DOMAIN_H_
#define _TIME_DOMAIN_H_
#include "handler.h"

void init_timer(int Nus, fft_func func);
void timer_evt_task(void *arg);

#endif
