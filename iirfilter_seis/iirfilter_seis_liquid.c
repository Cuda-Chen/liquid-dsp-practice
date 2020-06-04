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

    // output filtered result to script
    FILE *fptr = fopen(outputfile, "w");
    fprintf(fptr, "%% %s: auto-generated file. DO NOT EDIT.\n", outputfile);
    fprintf(fptr, "clear all;\n");
    fprintf(fptr, "close all;\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "order=%d;\n", order);
    fprintf(fptr, "n=%d;\n", dataSize);
    fprintf(fptr, "x=zeros(1,n);\n");
    fprintf(fptr, "y=zeros(1,n);\n");
    for(index = 0; index != dataSize; index++)
    {
        fprintf(fptr, "x(%d) = %e + j*%e;\n", index + 1, crealf(data[index]), cimagf(data[index]));
        fprintf(fptr, "y(%d) = %e + j*%e;\n", index + 1, crealf(result[index]), cimagf(result[index]));
    }
    // plot output
    fprintf(fptr, "t=0:(n-1);\n");
    fprintf(fptr, "figure;\n");
    fprintf(fptr, "subplot(2,1,1);\n");
    fprintf(fptr, "  plot(t,real(x),'-','Color',[1 1 1]*0.5,'LineWidth',1,...\n");
    fprintf(fptr, "       t,real(y),'-','Color',[0 0.5 0.25],'LineWidth',2);\n");
    fprintf(fptr, "  xlabel('time');\n");
    fprintf(fptr, "  ylabel('real');\n");
    fprintf(fptr, "  legend('input','filtered output','location','northeast');\n");
    fprintf(fptr, "  grid on;\n");
    fprintf(fptr, "subplot(2,1,2);\n");
    fprintf(fptr, "  plot(t,imag(x),'-','Color',[1 1 1]*0.5,'LineWidth',1,...\n");
    fprintf(fptr, "       t,imag(y),'-','Color',[0 0.25 0.5],'LineWidth',2);\n");
    fprintf(fptr, "  xlabel('time');\n");
    fprintf(fptr, "  ylabel('imag');\n");
    fprintf(fptr, "  legend('input','filtered output','location','northeast');\n");
    fprintf(fptr, "  grid on;\n");
    fclose(fptr);

    free(data);
    free(result);
    return 0;
}
