#pragma once

#include <util.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <fftw3.h>
#include <fftengine.h>

class SparSDR
{
public:
    SparSDR(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin);
    ~SparSDR();
    void process_block();

private:
    void scale_block(std::complex<float> *block, size_t block_size)
    {
        for (size_t i = 0; i < block_size; ++i)
        {
            block[i] /= static_cast<float>(this->fft_size);
        }
    }
    unsigned int read_samples(std::complex<float> *block, size_t block_size)
    {
        return fread(block, sizeof(std::complex<float>), block_size, stdin);
    }
    void threshold_block(std::complex<float> *block);

    unsigned int fft_size;
    unsigned int start_bin;
    unsigned int stop_bin;
    FFTEngine *fft0 = nullptr;
    FFTEngine *fft1 = nullptr;

    StaggeredBuffer *staggered_buffer = nullptr;
};