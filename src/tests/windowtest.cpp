#include <sparsdr.h>
#include <window.h>

void make_one_by_sqrt2(std::complex<float> *buffer, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        buffer[i] = std::complex<float>(1, 1) / std::complex<float>(sqrt(2), 0);
    }
}

int main(int argc, char **argv)
{

    // Create a vector of 64 complex samples all 1
    size_t buf_len = 64;
    std::complex<float> *buffer = new std::complex<float>[buf_len];
    make_one_by_sqrt2(buffer, buf_len);

    // Create a hann window
    float *window = new float[buf_len];
    make_hann_window(window, buf_len);

    // Apply the window
    vector_window_cf(buffer, window, buf_len, buffer);

    // Test COLA property
    float max_diff = 0.000001;
    for (int i = 0; i < buf_len / 2; i++)
    {
        if (std::abs(window[i] + window[buf_len / 2 + i] - 1) > max_diff)
        {
            std::cout << "COLA property violated at index " << i << std::endl;
            std::cout << window[i] + window[buf_len / 2 + i] << std::endl;
            return 1;
        }
    }

    return 0;
}