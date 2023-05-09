#include <fftengine.h>
#include "string.h"

FFTEngine::FFTEngine(unsigned int fft_size, int sign = FFTW_FORWARD, unsigned int flags = FFTW_MEASURE)
{
    this->fft_size = fft_size;
    this->fft_input = reinterpret_cast<fftwf_complex *>(fftwf_malloc(sizeof(fftwf_complex) * this->fft_size));
    this->fft_output = reinterpret_cast<fftwf_complex *>(fftwf_malloc(sizeof(fftwf_complex) * this->fft_size));
    this->fft = fftwf_plan_dft_1d(this->fft_size, this->fft_input, this->fft_output, sign, flags);
}

FFTEngine::~FFTEngine()
{
    fftwf_destroy_plan(this->fft);
    fftwf_free(this->fft_input);
    fftwf_free(this->fft_output);
}

void FFTEngine::execute_fft(void *input, void *output)
{
    memcpy(this->fft_input, input, sizeof(fftwf_complex) * this->fft_size);
    fftwf_execute(this->fft);
    memcpy(output, this->fft_output, sizeof(fftwf_complex) * this->fft_size);
}

unsigned int FFTEngine::get_fft_size()
{
    return this->fft_size;
}