#include <util.h>

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

    std::copy(block, block + this->buffer_size, this->buffer0);
    std::copy(this->buffer1 + this->stagger_size, this->buffer1 + this->buffer_size, this->buffer1);
    std::copy(block + this->stagger_size, block + this->buffer_size, this->buffer1 + this->stagger_size);

    this->push_count++;
}