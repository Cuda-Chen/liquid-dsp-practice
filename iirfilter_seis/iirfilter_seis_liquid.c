// create a bandpass filter then use it

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "liquid/liquid.h"

int dataSize = 365896; // input data length
float sampleRate = 100.0f; // input data sampling rate
float lowcut = 0.05f; // desired low cut frequency
float highcut = 0.1f; // desired high cut frequency
char const *outputfile = "liquid_result.m"; // visualization use

int main(void)
{
    // get input data
    FILE *input;
    float complex *data = (float complex *)malloc(sizeof(float complex) * dataSize);
    int index;
    input = fopen("test_wave.txt", "r");
    for(index = 0; index != dataSize; index++)
    {
        fscanf(input, "%f", &data[index]);
    }
    fclose(input);

    // filter design
    liquid_iirdes_filtertype ftype = LIQUID_IIRDES_BUTTER;
    liquid_iirdes_bandtype btype   = LIQUID_IIRDES_BANDPASS;
    liquid_iirdes_format format    = LIQUID_IIRDES_SOS;
    unsigned int order = 4;
    float fc = lowcut / sampleRate;
    float f0 = sqrt(lowcut * highcut) / sampleRate;
    float Ap = 1.0f;
    float As = 40.0f;
    // design filter from prototype
    iirfilt_crcf q = iirfilt_crcf_create_prototype(
                        ftype, btype, format, order, fc, f0, Ap, As);
    iirfilt_crcf_print(q);

    float complex *result = (float complex *)malloc(sizeof(float complex) * dataSize);

    for(index = 0; index != dataSize; index++)
    {
        iirfilt_crcf_execute(q, data[index], &result[index]);
    }

    // compute frequency response
    int nfft = 5000;
    float complex H[nfft];
    int i;
    for(i = 0; i < nfft; i++)
    {
        float freq = 0.5f * (float)i / (float)nfft;
        iirfilt_crcf_freqresponse(q, freq, &H[i]);
    }

    // output filtered result to script
    FILE *fptr = fopen(outputfile, "w");
    fprintf(fptr, "%% %s: auto-generated file. DO NOT EDIT.\n", outputfile);
    fprintf(fptr, "clear all;\n");
    fprintf(fptr, "close all;\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "order=%d;\n", order);
    fprintf(fptr, "n=%d;\n", dataSize);
    fprintf(fptr, "nfft=%u\n", nfft);
    fprintf(fptr, "x=zeros(1,n);\n");
    fprintf(fptr, "y=zeros(1,n);\n");
    fprintf(fptr, "H=zeros(1,nfft);\n");
    for(index = 0; index != dataSize; index++)
    {
        fprintf(fptr, "x(%d) = %e + j*%e;\n", index + 1, crealf(data[index]), cimagf(data[index]));
        fprintf(fptr, "y(%d) = %e + j*%e;\n", index + 1, crealf(result[index]), cimagf(result[index]));
    }
    for (i=0; i<nfft; i++)
        fprintf(fptr,"H(%4u) = %12.8f + j*%12.8f;\n", i+1, crealf(H[i]), cimagf(H[i]));
    // plot output
    fprintf(fptr, "t=0:(n-1);\n");
    fprintf(fptr, "figure;\n");
    fprintf(fptr, "subplot(2,1,1);\n");
    fprintf(fptr, "  plot(t,real(x),'-','Color',[0 0.5 0.25],'LineWidth',1);\n");
    fprintf(fptr, "  xlabel('time');\n");
    fprintf(fptr, "  ylabel('amplitude');\n");
    fprintf(fptr, "  legend('input','location','northeast');\n");
    fprintf(fptr, "  grid on;\n");
    fprintf(fptr, "subplot(2,1,2);\n");
    fprintf(fptr, "  plot(t,real(y),'-','Color',[0 0.25 0.5],'LineWidth',1)\n");
    fprintf(fptr, "  xlabel('time');\n");
    fprintf(fptr, "  ylabel('amplitude');\n");
    fprintf(fptr, "  legend('filtered output','location','northeast');\n");
    fprintf(fptr, "  grid on;\n");

    // plot frequency response
    fprintf(fptr,"f=0.5*[0:(nfft-1)]/nfft;\n");
    fprintf(fptr,"figure;\n");
    fprintf(fptr,"subplot(3,1,1);\n");
    fprintf(fptr,"  plot(f,20*log10(abs(H)),'Color',[0 0.25 0.5],'LineWidth',2);\n");
    fprintf(fptr,"  axis([0 0.005 -3 0]);\n");
    fprintf(fptr,"  grid on;\n");
    fprintf(fptr,"  ylabel('Pass band [dB]');\n");
    fprintf(fptr,"subplot(3,1,2);\n");
    fprintf(fptr,"  plot(f,20*log10(abs(H)),'Color',[0 0.25 0.5],'LineWidth',2);\n");
    fprintf(fptr,"  axis([0 0.005 -100 0]);\n");
    fprintf(fptr,"  grid on;\n");
    fprintf(fptr,"  ylabel('Stop band [dB]');\n");
    fprintf(fptr,"subplot(3,1,3);\n");
    fprintf(fptr,"  plot(f,180/pi*arg(H),'Color',[0 0.25 0.5],'LineWidth',2);\n");
    fprintf(fptr,"  axis([0 0.005 -180 180]);\n");
    fprintf(fptr,"  grid on;\n");
    fprintf(fptr,"  ylabel('Phase [degrees]');\n");
    fprintf(fptr,"  xlabel('Normalized Frequency [f/F_s]');\n");

    fclose(fptr);

    free(data);
    free(result);
    iirfilt_crcf_destroy(q);
    return 0;
}
