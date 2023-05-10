#include <util.h>
#include <iostream>

void make_idx_vec(std::complex<float> *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        buffer[i] = std::complex<float>(i, 0);
    }
}

void print_vec(std::complex<float> *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        std::cout << buffer[i] << ",";
    }
    std::cout << std::endl;
}

int check_equal(std::complex<float> *buffer1, std::complex<float> *buffer2, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (buffer1[i] != buffer2[i])
        {
            std::cout << "Buffers not equal at index: " << i << std::endl;
            std::cout << "Buffer 1: " << buffer1[i] << "Buffer 2: " << buffer2[i] << std::endl;
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{

    // Create a vector of 64 samples increasing
    size_t buf_len = 1024;
    std::complex<float> *buffer = new std::complex<float>[buf_len];
    make_idx_vec(buffer, buf_len);

    // Create staggered buffer
    unsigned int window_size = 4;
    unsigned int stagger_size = window_size / 2;
    StaggeredBuffer *staggered_buffer = new StaggeredBuffer(window_size, stagger_size);

    // Do push and read 4 times
    unsigned int num_pushes = buf_len / window_size;
    for (unsigned int i = 0; i < num_pushes; i++)
    {
        staggered_buffer->push(buffer + i * window_size);
        if (check_equal((std::complex<float> *)staggered_buffer->get_buffer0(), buffer + i * window_size, window_size) != 0)
        {
            std::cout << "Test failed at push index " << i << std::endl;
            return -1;
        }
        if (i >= 1)
        {
            if (check_equal((std::complex<float> *)staggered_buffer->get_buffer1(), buffer + (i - 1) * window_size + stagger_size, window_size) != 0)
            {
                std::cout << "Test failed at push index " << i << std::endl;
                return -1;
            }
        }
    }
    return 0;
}