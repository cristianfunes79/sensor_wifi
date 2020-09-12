//#define delay_ms(uint32_t time_ms) delay_us(uint32_t time_us*1000)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "xtensa/hal.h"
#define us_to_counts(x) (x)*CONFIG_ESP8266_DEFAULT_CPU_FREQ_MHZ
void delay_us(unsigned int time_us);
