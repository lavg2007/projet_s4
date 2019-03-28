// Chebyshev type I bandstop IIR filter [900,1100] Hz in SOS format, sections as coeffs (b0, b1, b2, a0, a1, a2)
// w[n] = a0x[n] - a1w[n - 1] - a2w[n - 2]
// y[n] = b0w[n] + b1w[n - 1] + b2w[n - 2]
// Storage of of w[n-1] and w[n-2] in IIRw
#define IIR_FILTER_COEFFS_SCALE_NBITS 13
#define N_SOS_SECTIONS 2
int32_t IIRCoeffs[N_SOS_SECTIONS][6] = 
//{{7201,-14253,7201,8192,-16071,8071}, 
//{ 8192, -16225, 8192, 8192,-16143, 8089}};  //900-1100
//{{7291,-14433,7291,8192,-16204,8180}, 
//{ 8192, -16218, 8192, 8192,-16209, 8180}};  //990-1010
{{687,677,687,8192,-8067,3386},
{8192,-2816,8191,8192,-6336,7045}};
static int32_t IIRw[N_SOS_SECTIONS][3] = {0};
