#ifndef _APP_AUDIO_H_
#define _APP_AUDIO_H_

void app_adc_init();
int set_samples(int s);
int set_filter_limit(int f);
int set_interval(int i);
void audio_fft_task(void *ignore);

#endif /* _APP_AUDIO_H_ */