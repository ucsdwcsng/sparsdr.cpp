#pragma once

#include <util.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <fftw3.h>
#include <fftengine.h>
#include <iface.h>
#include <window.h>

class SparSDRCompressor
{
public:
    SparSDRCompressor(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin, Interface *interface);
    ~SparSDRCompressor();
    void compress();

private:
    void threshold_block(std::complex<float> *block);

    unsigned int fft_size;
    unsigned int start_bin;
    unsigned int stop_bin;
    FFTEngine *fft0 = nullptr;
    FFTEngine *fft1 = nullptr;

    float *window = nullptr;

    StaggeredBuffer *staggered_buffer = nullptr;

    // Interface
    Interface *interface = nullptr;
};

class SparSDRReconstructor
{
public:
    SparSDRReconstructor(unsigned int fft_size, Interface *interface);
    ~SparSDRReconstructor();
    void reconstruct();

private:
    unsigned int fft_size;
    FFTEngine *ifft0 = nullptr;
    FFTEngine *ifft1 = nullptr;

    Interface *interface = nullptr;
};