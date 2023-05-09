#include <sparsdr.h>

SparSDR::SparSDR(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin)
{
    this->fft_size = fft_size;
    this->start_bin = start_bin;
    this->stop_bin = stop_bin;

    this->fft0 = new FFTEngine(this->fft_size, FFTW_FORWARD, FFTW_ESTIMATE);
    this->fft1 = new FFTEngine(this->fft_size, FFTW_BACKWARD, FFTW_ESTIMATE);
}

SparSDR::~SparSDR()
{
    // Destroy FFT plans
    delete this->fft0;
    delete this->fft1;
}

void SparSDR::threshold_block(std::complex<float> *block)
{
    for (unsigned int bin = this->start_bin; bin <= this->stop_bin; ++bin)
    {
        block[bin] = 0.0f;
    }
}

void SparSDR::process_block()
{
    // Create FFT and iFFT plans
    std::complex<float> block[this->fft_size];
    std::complex<float> interblock[this->fft_size];

    while (true)
    {

        size_t read_size = this->read_samples(block, this->fft_size);
        if (read_size == 0 || read_size < this->fft_size)
        {
            break;
        }
        // Perform FFT
        fft0->execute_fft(block, interblock);

        // Zero out specified bins
        this->threshold_block(interblock);

        // Perform iFFT
        fft1->execute_fft(interblock, block);

        // Normalize and output the block
        this->scale_block(block, read_size);
        fwrite(block, sizeof(std::complex<float>), read_size, stdout);
    }
}