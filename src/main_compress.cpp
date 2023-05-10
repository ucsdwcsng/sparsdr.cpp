#include <sparsdr.h>

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <fft_size> <start_bin> <stop_bin>" << std::endl;
        return 1;
    }

    unsigned int fft_size = std::atoi(argv[1]);
    unsigned int start_bin = std::atoi(argv[2]);
    unsigned int stop_bin = std::atoi(argv[3]);

    if (stop_bin >= fft_size || start_bin >= stop_bin)
    {
        std::cerr << "Invalid bin range" << std::endl;
        return 1;
    }

    // Set stdin and stdout to binary mode
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    freopen(nullptr, "rb", stdin);
    freopen(nullptr, "wb", stdout);

    // Process the input
    SparSDRCompressor sparsdr(fft_size, start_bin, stop_bin);
    sparsdr.compress();

    return 0;
}
