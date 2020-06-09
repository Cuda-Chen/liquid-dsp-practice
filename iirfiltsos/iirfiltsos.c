#include <stdio.h>
#include <math.h>
#include <complex.h>

#include "liquid/liquid.h"

float sampleRate = 100.0f; // input data sampling rate
float lowcut = 0.05f; // desired low cut frequency
float highcut = 0.1f; // desired high cut frequency

int main(void)
{
    // filter design
    liquid_iirdes_filtertype ftype = LIQUID_IIRDES_BUTTER;
    liquid_iirdes_bandtype btype   = LIQUID_IIRDES_BANDPASS;
    liquid_iirdes_format format    = LIQUID_IIRDES_SOS;
    unsigned int order = 4;
    float fc = lowcut / sampleRate;
    float f0 = sqrt(lowcut * highcut) / sampleRate;
    float Ap = 1.0f;
    float As = 40.0f;

    // create and IIR filter
    unsigned int N = order * 2;
    unsigned int r = N % 2;
    unsigned int L = (N - r) / 2;
    unsigned int h_len = 3 * (L + r);
    float b[h_len];
    float a[h_len];
    liquid_iirdes(ftype, btype, format, order, fc, f0, Ap, As, b, a);
    iirfiltsos_crcf q = iirfiltsos_crcf_create(b, a);
    
    // print the coefficients
    iirfiltsos_crcf_print(q);

    int i;
    unsigned int n = 128;
    float complex x[n];
    float complex y[n];
    for(i = 0; i < n; i++) {
        x[i] = cexpf((2*M_PI*0.057f*_Complex_I - 0.04f)*i);
        x[i] += 0.1f*(randnf() + _Complex_I*randnf());

        // run filter with df1
        iirfiltsos_crcf_execute_df1(q, x[i], &y[i]);
    }

    iirfiltsos_crcf_destroy(q);

    return 0;
}
