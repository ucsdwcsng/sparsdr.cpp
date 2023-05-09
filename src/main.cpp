#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <liquid/liquid.h>
#include <cstring>
#include <cstdlib>

void process_block(unsigned int fft_size,
                   unsigned int start_bin, unsigned int stop_bin)
{
    // Create FFT and iFFT plans
    std::complex<float> block[fft_size];
    std::complex<float> interblock[fft_size];
    std::complex<float> output_block[fft_size];
    int flags = 0;
    fftplan fft = fft_create_plan(fft_size, block, interblock, LIQUID_FFT_FORWARD, flags);
    fftplan ifft = fft_create_plan(fft_size, interblock, output_block, LIQUID_FFT_BACKWARD, flags);

    size_t block_size;
    while (true)
    {
        block_size = 0;
        for (; block_size < fft_size; ++block_size)
        {
            float real, imag;
            if (!std::cin.read(reinterpret_cast<char *>(&real), sizeof(float)) ||
                !std::cin.read(reinterpret_cast<char *>(&imag), sizeof(float)))
            {
                break;
            }
            block[block_size] = std::complex<float>(real, imag);
        }

        if (block_size == 0 || block_size < fft_size)
        {
            break;
        }
        // Perform FFT
        fft_execute(fft);

        // Zero out specified bins
        for (unsigned int bin = start_bin; bin <= stop_bin; ++bin)
        {
            interblock[bin] = 0.0f;
        }

        // Perform iFFT
        fft_execute(ifft);

        // Normalize and output the block
        for (size_t j = 0; j < block_size; ++j)
        {
            output_block[j] /= static_cast<float>(fft_size);
            float real_part = output_block[j].real();
            float imag_part = output_block[j].imag();
            std::cout.write(reinterpret_cast<const char *>(&real_part), sizeof(float));
            std::cout.write(reinterpret_cast<const char *>(&imag_part), sizeof(float));
        }
    }

    // Clean up
    fft_destroy_plan(fft);
    fft_destroy_plan(ifft);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <fft_size> <start_bin> <stop_bin>" << std::endl;
        return 1;
    }

    unsigned int fft_size = std::atoi(argv[1]);
    unsigned int start_bin = std::atoi(argv[2]);
    unsigned int stop_bin = std::atoi(argv[3]);

    if (stop_bin >= fft_size || start_bin >= stop_bin)
    {
        std::cerr << "Invalid bin range" << std::endl;
        return 1;
    }

    // Process the input
    process_block(fft_size, start_bin, stop_bin);

    return 0;
}
