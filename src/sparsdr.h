#pragma once

#include <util.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <fftw3.h>
#include <fftengine.h>

class SparSDRCompressor
{
public:
    SparSDRCompressor(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin);
    ~SparSDRCompressor();
    void compress();

private:
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

class SparSDRReconstructor
{
public:
    SparSDRReconstructor(unsigned int fft_size);
    ~SparSDRReconstructor();
    void reconstruct();

private:
    unsigned int read_samples(std::complex<float> *block, size_t block_size)
    {
        return fread(block, sizeof(std::complex<float>), block_size, stdin);
    }

    unsigned int fft_size;
    FFTEngine *ifft0 = nullptr;
    FFTEngine *ifft1 = nullptr;
};