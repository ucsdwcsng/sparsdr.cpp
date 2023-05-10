#include <sparsdr.h>

SparSDRCompressor::SparSDRCompressor(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin, Interface *interface)
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

    // Create window
    this->window = new float[this->fft_size];
    make_hann_window(this->window, this->fft_size);

    this->staggered_buffer = new StaggeredBuffer(this->fft_size, this->fft_size / 2);

    this->interface = interface;
}

SparSDRCompressor::~SparSDRCompressor()
{
    // Destroy FFT plans
    delete this->fft0;
    delete this->fft1;

    // Destroy window
    delete this->window;

    // Destroy buffers
    delete this->staggered_buffer;

    // Destroy interface
    delete this->interface;
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

        size_t read_size = this->interface->read_samples(block, this->fft_size);
        if (read_size == 0 || read_size < this->fft_size)
        {
            break;
        }

        // Perform operation on buffer0
        vector_window_cf((cf_t *)this->staggered_buffer->get_buffer0(), this->window, this->fft_size, interblock);
        fft0->execute_fft(interblock, interblock);
        this->threshold_block(interblock);
        this->interface->write_samples(interblock, this->fft_size);

        // Push block into staggered buffer
        this->staggered_buffer->push(block);

        // Perform operation on buffer1
        vector_window_cf((cf_t *)this->staggered_buffer->get_buffer1(), this->window, this->fft_size, interblock);
        fft1->execute_fft(interblock, interblock);
        this->threshold_block(interblock);
        this->interface->write_samples(interblock, this->fft_size);
    }
}

//---- Reconstruct ---//
SparSDRReconstructor::SparSDRReconstructor(unsigned int fft_size, Interface *interface)
{
    this->fft_size = fft_size;

    // if fft size is not a multiple of 2, throw Runtime error
    if (this->fft_size % 2 != 0)
    {
        throw std::runtime_error("FFT size must be a multiple of 2");
    }

    this->ifft0 = new FFTEngine(this->fft_size, FFTW_BACKWARD, FFTW_ESTIMATE);
    this->ifft1 = new FFTEngine(this->fft_size, FFTW_BACKWARD, FFTW_ESTIMATE);

    this->interface = interface;
}

SparSDRReconstructor::~SparSDRReconstructor()
{
    // Destroy FFT plans
    delete this->ifft0;
    delete this->ifft1;

    // Destroy interface
    delete this->interface;
}

void SparSDRReconstructor::reconstruct()
{
    // Create FFT and iFFT plans
    std::complex<float> block0[this->fft_size];
    std::complex<float> block1[this->fft_size];

    std::complex<float> evenw[this->fft_size];
    std::complex<float> oddw[this->fft_size];

    while (true)
    {

        size_t read_size = this->interface->read_samples(block0, this->fft_size);
        read_size += this->interface->read_samples(block1, this->fft_size);
        if (read_size < 2 * this->fft_size)
        {
            break;
        }
        // Perform IFFT on both blocks
        ifft0->execute_fft(block0, evenw);
        scale_block(evenw, this->fft_size);

        // add last half of odd window to first half of even window
        for (unsigned int i = 0; i < this->fft_size / 2; ++i)
        {
            evenw[i] += oddw[i + this->fft_size / 2];
        }

        ifft1->execute_fft(block1, oddw);
        scale_block(oddw, this->fft_size);

        // add last half of even window to first half of odd window
        for (unsigned int i = 0; i < this->fft_size / 2; ++i)
        {
            evenw[i + this->fft_size / 2] += oddw[i];
        }

        // Write only one of the blocks to output
        size_t write_size = this->interface->write_samples(evenw, this->fft_size);
    }
}