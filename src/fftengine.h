#pragma once

#include <fftw3.h>

class FFTEngine
{
public:
    FFTEngine(unsigned int fft_size, int sign, unsigned int flags);
    ~FFTEngine();
    void execute_fft(void *input, void *output);
    unsigned int get_fft_size();

private:
    unsigned int fft_size;
    fftwf_plan fft = nullptr;
    fftwf_complex *fft_input = nullptr;
    fftwf_complex *fft_output = nullptr;
};