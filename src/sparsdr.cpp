#include <sparsdr.h>

SparSDR::SparSDR(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin)
{
    this->fft_size = fft_size;
    this->start_bin = start_bin;
    this->stop_bin = stop_bin;
}

SparSDR::~SparSDR()
{
}

void SparSDR::process_block()
{
    // Create FFT and iFFT plans
    std::complex<float> block[this->fft_size];
    std::complex<float> interblock[this->fft_size];
    std::complex<float> output_block[this->fft_size];
    int flags = 0;
    fftwf_plan fft = fftwf_plan_dft_1d(this->fft_size,
                                       reinterpret_cast<fftwf_complex *>(block),
                                       reinterpret_cast<fftwf_complex *>(interblock),
                                       FFTW_FORWARD, FFTW_MEASURE);

    fftwf_plan ifft = fftwf_plan_dft_1d(this->fft_size,
                                        reinterpret_cast<fftwf_complex *>(interblock),
                                        reinterpret_cast<fftwf_complex *>(output_block),
                                        FFTW_BACKWARD, FFTW_MEASURE);

    while (true)
    {
        size_t read_size = fread(block, sizeof(std::complex<float>), this->fft_size, stdin);
        if (read_size == 0 || read_size < this->fft_size)
        {
            break;
        }
        // Perform FFT
        fftwf_execute(fft);

        // Zero out specified bins
        for (unsigned int bin = this->start_bin; bin <= this->stop_bin; ++bin)
        {
            interblock[bin] = 0.0f;
        }

        // Perform iFFT
        fftwf_execute(ifft);

        // Normalize and output the block
        for (size_t j = 0; j < read_size; ++j)
        {
            output_block[j] /= static_cast<float>(this->fft_size);
        }
        fwrite(output_block, sizeof(std::complex<float>), read_size, stdout);
    }

    // Clean up
    fftwf_destroy_plan(fft);
    fftwf_destroy_plan(ifft);
}