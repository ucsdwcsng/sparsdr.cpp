#include <sparsdr.h>

SparSDRCompressor::SparSDRCompressor(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin)
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
    this->fft1 = new FFTEngine(this->fft_size, FFTW_FORWARD, FFTW_ESTIMATE);

    this->staggered_buffer = new StaggeredBuffer(this->fft_size, this->fft_size / 2);
}

SparSDRCompressor::~SparSDRCompressor()
{
    // Destroy FFT plans
    delete this->fft0;
    delete this->fft1;

    // Destroy buffers
    delete this->staggered_buffer;
}

void SparSDRCompressor::threshold_block(std::complex<float> *block)
{
    for (unsigned int bin = this->start_bin; bin <= this->stop_bin; ++bin)
    {
        block[bin] = 0.0f;
    }
}

void SparSDRCompressor::compress()
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
        // Push block into staggered buffer
        this->staggered_buffer->push(block);

        // Perform FFT
        fft0->execute_fft(this->staggered_buffer->get_buffer0(), block);
        fft1->execute_fft(this->staggered_buffer->get_buffer1(), interblock);

        // Zero out specified bins
        this->threshold_block(block);
        this->threshold_block(interblock);

        // Write both windows to stdout
        fwrite(block, sizeof(std::complex<float>), this->fft_size, stdout);
        fwrite(interblock, sizeof(std::complex<float>), this->fft_size, stdout);
    }
}

//---- Reconstruct ---//
SparSDRReconstructor::SparSDRReconstructor(unsigned int fft_size)
{
    this->fft_size = fft_size;

    // if fft size is not a multiple of 2, throw Runtime error
    if (this->fft_size % 2 != 0)
    {
        throw std::runtime_error("FFT size must be a multiple of 2");
    }

    this->ifft0 = new FFTEngine(this->fft_size, FFTW_BACKWARD, FFTW_ESTIMATE);
    this->ifft1 = new FFTEngine(this->fft_size, FFTW_BACKWARD, FFTW_ESTIMATE);
}

SparSDRReconstructor::~SparSDRReconstructor()
{
    // Destroy FFT plans
    delete this->ifft0;
    delete this->ifft1;
}

void SparSDRReconstructor::reconstruct()
{
    // Create FFT and iFFT plans
    std::complex<float> block0[this->fft_size];
    std::complex<float> block1[this->fft_size];

    while (true)
    {

        size_t read_size = this->read_samples(block0, this->fft_size);
        read_size += this->read_samples(block1, this->fft_size);
        if (read_size < 2 * this->fft_size)
        {
            break;
        }
        // Perform IFFT on both blocks
        ifft0->execute_fft(block0, block0);
        ifft1->execute_fft(block1, block1);

        // Normalize to preserve power
        scale_block(block0, this->fft_size);
        scale_block(block1, this->fft_size);

        // Write only one of the blocks to output
        fwrite(block0, sizeof(std::complex<float>), this->fft_size, stdout);
    }
}