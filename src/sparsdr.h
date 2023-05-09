#pragma once

#include <util.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <fftw3.h>

class SparSDR
{
public:
    SparSDR(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin);
    ~SparSDR();
    void process_block();

private:
    unsigned int fft_size;
    unsigned int start_bin;
    unsigned int stop_bin;
};