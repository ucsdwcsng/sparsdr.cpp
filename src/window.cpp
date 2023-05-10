#include <window.h>
void make_hann_window(float *window, unsigned int win_len)
{
    for (unsigned int i = 0; i < win_len; i++)
    {
        window[i] = liquid_hann(i, win_len + 1);
    }
}

void vector_window_cf(cf_t *buffer, float *window, unsigned int win_len, cf_t *obuffer)
{

    unsigned int t = (win_len >> 2) << 2;
    unsigned int i;

    for (i = 0; i < t; i += 4)
    {
        obuffer[i] = buffer[i] * window[i];
        obuffer[i + 1] = buffer[i + 1] * window[i + 1];
        obuffer[i + 2] = buffer[i + 2] * window[i + 2];
        obuffer[i + 3] = buffer[i + 3] * window[i + 3];
    }

    for (; i < win_len; i++)
    {
        obuffer[i] = buffer[i] * window[i];
    }
}
