#pragma once

#include <util.h>
#include <liquid/liquid.h>

typedef std::complex<float> cf_t;

void make_hann_window(float *window, unsigned int win_len);

void vector_window_cf(cf_t *buffer, float *window, unsigned int win_len, cf_t *obuffer);
