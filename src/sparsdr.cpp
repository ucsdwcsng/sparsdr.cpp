#include <sparsdr.h>

SparSDR::SparSDR(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin)
{
    this->fft_size = fft_size;
    this->start_bin = start_bin;
    this->stop_bin = stop_bin;

    // if fft size is not a multiple of 2, throw Runtime error
    if (this->fft_size % 2 != 0)
    {
        throw std::runtime_error("FFT size must be a multiple of 2");
    }

    this->fft0 = new FFTEngine(this->fft_size, FFTW_FORWARD, FFTW_ESTIMATE);
    this->fft1 = new FFTEngine(this->fft_size, FFTW_BACKWARD, FFTW_ESTIMATE);

    this->staggered_buffer = new StaggeredBuffer(this->fft_size, this->fft_size / 2);
}

SparSDR::~SparSDR()
{
    // Destroy FFT plans
    delete this->fft0;
    delete this->fft1;

    // Destroy buffers
    delete this->staggered_buffer;
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
        this->staggered_buffer->push(block);
        if (read_size == 0 || read_size < this->fft_size)
        {
            break;
        }
        // Perform FFT
        fft0->execute_fft(this->staggered_buffer->get_buffer0(), interblock);

        // Zero out specified bins
        this->threshold_block(interblock);

        // Perform iFFT
        fft1->execute_fft(interblock, block);

        // Normalize and output the block
        this->scale_block(block, read_size);
        fwrite(block, sizeof(std::complex<float>), read_size, stdout);
    }
}