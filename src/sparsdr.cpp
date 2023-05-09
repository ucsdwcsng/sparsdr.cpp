#include <sparsdr.h>

SparSDR::SparSDR(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin)
{
    this->fft_size = fft_size;
    this->start_bin = start_bin;
    this->stop_bin = stop_bin;

    this->fft0 = new FFTEngine(this->fft_size, FFTW_FORWARD, FFTW_MEASURE);
    this->fft1 = new FFTEngine(this->fft_size, FFTW_BACKWARD, FFTW_MEASURE);
}

SparSDR::~SparSDR()
{
    // Destroy FFT plans
    delete this->fft0;
    delete this->fft1;
}

void SparSDR::process_block()
{
    // Create FFT and iFFT plans
    std::complex<float> block[this->fft_size];
    std::complex<float> interblock[this->fft_size];

    while (true)
    {
        size_t read_size = fread(block, sizeof(std::complex<float>), this->fft_size, stdin);
        if (read_size == 0 || read_size < this->fft_size)
        {
            break;
        }
        // Perform FFT
        fft0->execute_fft(block, interblock);

        // Zero out specified bins
        for (unsigned int bin = this->start_bin; bin <= this->stop_bin; ++bin)
        {
            interblock[bin] = 0.0f;
        }

        // Perform iFFT
        // fftwf_execute(ifft);
        fft1->execute_fft(interblock, block);

        // Normalize and output the block
        for (size_t j = 0; j < read_size; ++j)
        {
            block[j] /= static_cast<float>(this->fft_size);
        }
        fwrite(block, sizeof(std::complex<float>), read_size, stdout);
    }
}