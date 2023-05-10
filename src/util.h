#pragma once

#include <algorithm>
#include <complex>
#include <map>
#include <math.h>
#include <sstream>
#include <vector>

// Simple processing
void scale_block(std::complex<float> *block, size_t block_size);

// Staggered buffer
class StaggeredBuffer
{
public:
    StaggeredBuffer(unsigned int buffer_size, unsigned int stagger_size);
    ~StaggeredBuffer();
    void push(std::complex<float> *block);

    // Getters
    void *get_buffer0()
    {
        return this->buffer0;
    }
    void *get_buffer1()
    {
        return this->buffer1;
    }
    unsigned long long int get_push_count()
    {
        return this->push_count;
    }

private:
    unsigned int buffer_size;
    unsigned int stagger_size;
    unsigned long long int push_count = 0;
    std::complex<float> *buffer0 = nullptr;
    std::complex<float> *buffer1 = nullptr;
};
