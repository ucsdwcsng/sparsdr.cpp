#include <sparsdr.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <fft_size>" << std::endl;
        return 1;
    }

    unsigned int fft_size = std::atoi(argv[1]);

    // Set stdin and stdout to binary mode
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    FILE *f1 = freopen(nullptr, "rb", stdin);
    FILE *f2 = freopen(nullptr, "wb", stdout);

    // Process the input
    StdInterface *interface = new StdInterface();
    SparSDRReconstructor sparsdr(fft_size, interface, false);
    sparsdr.reconstruct();

    return 0;
}
