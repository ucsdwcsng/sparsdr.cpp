#pragma once

#include <util.h>

class Interface
{
public:
    Interface(){};
    ~Interface(){};
    virtual size_t read_samples(std::complex<float> *block, size_t block_size) = 0;
    virtual size_t write_samples(std::complex<float> *block, size_t block_size) = 0;
};

class StdInterface : public Interface
{
public:
    StdInterface();
    ~StdInterface();
    size_t read_samples(std::complex<float> *block, size_t block_size);
    size_t write_samples(std::complex<float> *block, size_t block_size);

private:
    bool init = false;
};

class FileInterface : public Interface
{
public:
    FileInterface(const char *ifilename, const char *ofilename);
    ~FileInterface();
    size_t read_samples(std::complex<float> *block, size_t block_size);
    size_t write_samples(std::complex<float> *block, size_t block_size);

private:
    FILE *istream = nullptr;
    FILE *ostream = nullptr;
};