#include <sparsdr.h>

void compress_wrapper(unsigned int fft_size, unsigned int start_bin, unsigned int stop_bin, const char *filename)
{
    FileInterface *interface = new FileInterface(filename, "/tmp/interimfile");
    SparSDRCompressor sparsdr_c(fft_size, start_bin, stop_bin, interface);
    sparsdr_c.compress();
}

void reconstruct_wrapper(unsigned int fft_size, const char *filename)
{
    FileInterface *interface = new FileInterface("/tmp/interimfile", filename);
    SparSDRReconstructor sparsdr_r(fft_size, interface, true);
    sparsdr_r.reconstruct();
}

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0] << " <fft_size> <start_bin> <stop_bin> <input_file> <output_file>" << std::endl;
        return 1;
    }

    unsigned int fft_size = std::atoi(argv[1]);
    unsigned int start_bin = std::atoi(argv[2]);
    unsigned int stop_bin = std::atoi(argv[3]);

    if (stop_bin > fft_size || start_bin >= stop_bin)
    {
        std::cerr << "Invalid bin range" << std::endl;
        return 1;
    }

    // Make sure bins are all on one side of the window
    if (start_bin < fft_size / 2 && stop_bin > fft_size / 2)
    {
        std::cerr << "Bins must be on one side of the window" << std::endl;
        return 1;
    }

    // Compress
    compress_wrapper(fft_size, start_bin, stop_bin, argv[4]);

    // Reconstruct
    reconstruct_wrapper(stop_bin - start_bin, argv[5]);

    return 0;
}
