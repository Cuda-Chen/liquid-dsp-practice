#include <liquid/liquid.h>
#include <math.h>
#include <stdio.h>

// https://liquidsdr.org/doc/iirdes/

int main() {
    unsigned int order = 4;
    float lowcut = 0.05, highcut = 0.1, sampleRate = 100.0;
    float fc = lowcut / 100.0;
    float f0 = sqrt(lowcut * highcut) / sampleRate;
    float As = 60.0f;
    float Ap = 1.0f;

    unsigned int N = 2 * order;
    unsigned int r = N % 2;
    unsigned int L = (N - r) / 2;

    float B[3 * (L + r)];
    float A[3 * (L + r)];

    liquid_iirdes(LIQUID_IIRDES_BUTTER,
                  LIQUID_IIRDES_BANDPASS,
                  LIQUID_IIRDES_SOS,
                  order,
                  fc, f0, Ap, As,
                  B, A);

    unsigned int i;
    printf("B [%u x 3] :\n", L+r);
    for (i=0; i<L+r; i++)
        printf("  %12.8f %12.8f %12.8f\n", B[3*i+0], B[3*i+1], B[3*i+2]);
    printf("A [%u x 3] :\n", L+r);
    for (i=0; i<L+r; i++)
        printf("  %12.8f %12.8f %12.8f\n", A[3*i+0], A[3*i+1], A[3*i+2]);

    return 0;
}
