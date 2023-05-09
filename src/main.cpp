#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <fftw3.h>
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
    // fftplan fft = fft_create_plan(fft_size, block, interblock, LIQUID_FFT_FORWARD, flags);
    // fftplan ifft = fft_create_plan(fft_size, interblock, output_block, LIQUID_FFT_BACKWARD, flags);
    fftwf_plan fft = fftwf_plan_dft_1d(fft_size,
                                       reinterpret_cast<fftwf_complex *>(block),
                                       reinterpret_cast<fftwf_complex *>(interblock),
                                       FFTW_FORWARD, FFTW_ESTIMATE);

    fftwf_plan ifft = fftwf_plan_dft_1d(fft_size,
                                        reinterpret_cast<fftwf_complex *>(interblock),
                                        reinterpret_cast<fftwf_complex *>(output_block),
                                        FFTW_BACKWARD, FFTW_ESTIMATE);

    while (true)
    {
        size_t read_size = fread(block, sizeof(std::complex<float>), fft_size, stdin);
        if (read_size == 0 || read_size < fft_size)
        {
            break;
        }
        // Perform FFT
        fftwf_execute(fft);

        // Zero out specified bins
        for (unsigned int bin = start_bin; bin <= stop_bin; ++bin)
        {
            interblock[bin] = 0.0f;
        }

        // Perform iFFT
        fftwf_execute(ifft);

        // Normalize and output the block
        for (size_t j = 0; j < read_size; ++j)
        {
            output_block[j] /= static_cast<float>(fft_size);
        }
        fwrite(output_block, sizeof(std::complex<float>), read_size, stdout);
    }

    // Clean up
    fftwf_destroy_plan(fft);
    fftwf_destroy_plan(ifft);
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

    // Set stdin and stdout to binary mode
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    freopen(nullptr, "rb", stdin);
    freopen(nullptr, "wb", stdout);

    // Process the input
    process_block(fft_size, start_bin, stop_bin);

    return 0;
}
