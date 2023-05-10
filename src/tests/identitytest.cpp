#include <sparsdr.h>
#include <iface.h>

void create_test_file(const char *filename)
{
    const size_t big_num = 1024 * 64;
    const float big_num_float = 1024 * 64;
    std::ofstream file(filename, std::ios::binary);
    std::complex<float> *buffer = new std::complex<float>[big_num];

    size_t period = 1024 * 2;
    for (unsigned int i = 0; i < big_num; i++)
    {
        float sign = (i % period) < (period / 2) ? 1.0 : -1.0;
        buffer[i] = std::complex<float>(sign * 0.1f, sign * 0.1f);
    }
    file.write((char *)buffer, big_num * sizeof(std::complex<float>));
}

void run_compress()
{
    unsigned int fft_size = 1024;
    unsigned int start_bin = 0;
    unsigned int stop_bin = fft_size;

    FileInterface *interface = new FileInterface("./testfile", "./interimfile");
    SparSDRCompressor sparsdr(fft_size, start_bin, stop_bin, interface);
    sparsdr.compress();
}

void run_reconstruct()
{
    unsigned int fft_size = 1024;
    FileInterface *interface2 = new FileInterface("./interimfile", "./testfile2");
    SparSDRReconstructor sparsdr2(fft_size, interface2, false);
    sparsdr2.reconstruct();
}

int main(int argc, char **argv)
{

    // This test creates a file, and then compresses and uncompresses it
    // Create a file and load it with 1024*64 complex samples
    create_test_file("./testfile");

    run_compress();
    run_reconstruct();

    // Compare the two files
    std::ifstream file1("./testfile", std::ios::binary);
    std::ifstream file2("./testfile2", std::ios::binary);
    std::complex<float> *buffer1 = new std::complex<float>[1024 * 64];
    std::complex<float> *buffer2 = new std::complex<float>[1024 * 64];
    file1.read((char *)buffer1, 1024 * 64 * sizeof(std::complex<float>));
    file2.read((char *)buffer2, 1024 * 64 * sizeof(std::complex<float>));

    size_t nfft = 1024;

    for (unsigned int i = 0; i < 1024 * 64 - 1024; i++)
    {
        std::complex<float> diff = buffer1[i] - buffer2[i + nfft];
        float diff_mag = std::abs(diff);

        if (diff_mag > 0.0001 * std::abs(buffer1[i]))
        {
            std::cout << "Error at index " << i << std::endl;
            std::cout << buffer1[i] << " != " << buffer2[i + nfft] << std::endl;
            return 1;
        }
    }

    return 0;
}