#include <iface.h>

StdInterface::StdInterface()
{
    this->init = true;
}

StdInterface::~StdInterface()
{
    this->init = false;
}

size_t StdInterface::read_samples(std::complex<float> *block, size_t block_size)
{
    size_t read_count = fread(block, sizeof(std::complex<float>), block_size, stdin);
    return read_count;
}

size_t StdInterface::write_samples(std::complex<float> *block, size_t block_size)
{
    size_t write_count = fwrite(block, sizeof(std::complex<float>), block_size, stdout);
    return write_count;
}

FileInterface::FileInterface(const char *ifilename, const char *ofilename)
{
    this->istream = fopen(ifilename, "rb");
    this->ostream = fopen(ofilename, "wb");
}

FileInterface::~FileInterface()
{
    if (this->ostream != nullptr)
        fclose(this->ostream);
    if (this->istream != nullptr)
        fclose(this->istream);
}

size_t FileInterface::read_samples(std::complex<float> *block, size_t block_size)
{
    size_t read_count = fread(block, sizeof(std::complex<float>), block_size, this->istream);
    return read_count;
}

size_t FileInterface::write_samples(std::complex<float> *block, size_t block_size)
{
    size_t write_count = fwrite(block, sizeof(std::complex<float>), block_size, this->ostream);
    return write_count;
}