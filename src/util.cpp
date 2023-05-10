#include <util.h>

// Simple tools
void scale_block(std::complex<float> *block, size_t block_size)
{
    for (size_t i = 0; i < block_size; ++i)
    {
        block[i] /= static_cast<float>(block_size);
    }
}

// StaggeredBuffer
StaggeredBuffer::StaggeredBuffer(unsigned int buffer_size, unsigned int stagger_size)
{
    this->buffer_size = buffer_size;
    this->stagger_size = stagger_size;

    this->buffer0 = new std::complex<float>[this->buffer_size];
    this->buffer1 = new std::complex<float>[this->buffer_size];
}

StaggeredBuffer::~StaggeredBuffer()
{
    delete[] this->buffer0;
    delete[] this->buffer1;
}

void StaggeredBuffer::push(std::complex<float> *block)
{

    // Copy last half of buffer0 to first half of buffer1
    std::copy(this->buffer0 + this->stagger_size, this->buffer0 + this->buffer_size, this->buffer1);

    // Copy input to buffer0
    std::copy(block, block + this->buffer_size, this->buffer0);

    // Copy first half of input to second half of buffer1
    std::copy(block, block + this->stagger_size, this->buffer1 + this->stagger_size);

    this->push_count++;
}
