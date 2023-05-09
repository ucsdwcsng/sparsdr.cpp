
#include <util.h>
#include <liquid/liquid.h>

int main()
{
    unsigned int M = 4;  // interpolation factor
    unsigned int m = 12; // filter delay [symbols]
    float As = 60.0f;    // filter stop-band attenuation [dB]

    // create interpolator from prototype
    firinterp_crcf interp = firinterp_crcf_create_kaiser(M, m, As);

    std::complex<float> x = 1.0f; // input sample

    std::complex<float> y[M]; // interpolated output buffer

    // repeat on input sample data as needed
    {
        firinterp_crcf_execute(interp, x, y);
    }

    // destroy interpolator object
    firinterp_crcf_destroy(interp);
    return 0;
}