#include <stdio.h>
#include <math.h>
#include <complex.h>

#include "liquid.h"

#define OUTPUT_FILENAME "iirfilter_example.m"

int main()
{
    // options
    liquid_iirdes_filtertype ftype = LIQUID_IIRDES_BUTTER;
    liquid_iirdes_bandtype btype = LIQUID_IIRDES_LOWPASS;
    liquid_iirdes_format     format = LIQUID_IIRDES_SOS;
    unsigned int order = 4; // filter order
    float fc = 0.1f; // cutoff frequency
    float f0 = 0.0f; // center frequency
    float Ap = 1.0f; // pass-band ripple
    float As = 40.0f; // stop-band attenuation
    unsigned int n = 128; // number of samples

    // design filter from prototype
    iirfilt_crcf q = iirfilt_crcf_create_prototype(
                        ftype, btype, format, order, fc, f0, Ap, As);
    iirfilt_crcf_print(q);

    return 0;
}
