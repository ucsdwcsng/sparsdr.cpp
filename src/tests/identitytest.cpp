#include <sparsdr.h>
#include <iface.h>

int main(int argc, char **argv)
{

    unsigned int fft_size = 1024;
    unsigned int start_bin = 0;
    unsigned int stop_bin = 512;

    // Set stdin and stdout to binary mode
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    freopen(nullptr, "rb", stdin);
    freopen(nullptr, "wb", stdout);

    // Process the input
    StdInterface *interface = new StdInterface();
    SparSDRCompressor sparsdr(fft_size, start_bin, stop_bin, interface);

    return 0;
}