/*
 * erfeng_fft.h
 *
 *  Created on: 24/May/2018
 *      Author: erfeng
 */

#ifndef _ERFENG_FFT_H
#define _ERFENG_FFT_H

#include "Config.h"

#define TYPE_FFT_E float /* Type is the same with COMPLEX member */

#ifndef PI
#define PI (3.14159265f)
#endif

typedef COMPLEX TYPE_FFT; /* Define COMPLEX in Config.h */

int fft(TYPE_FFT *x, uint32_t N);
int fft_real(TYPE_FFT *x, uint32_t N);
int ifft(TYPE_FFT *x, uint32_t N);
int ifft_real(TYPE_FFT *x, uint32_t N);
void get_magnitude(TYPE_FFT *x, uint32_t out[], int n);

#endif /* _ERFENG_FFT_H */
