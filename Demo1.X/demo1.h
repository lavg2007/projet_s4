/* 
 * File:   demo1.h
 * Author: Gabriel
 *
 * Created on March 23, 2019, 6:27 PM
 */

#ifndef DEMO1_H
#define	DEMO1_H

#ifdef	__cplusplus
extern "C" {
#endif

#define fftc            fft32c1024
#define LOG2FFTLEN      10
#define FFT_LEN         1024
#define SIG_LEN         64
#define H_LEN           256
#define SINE_LOG        12
#define SINE_RANGE      2048
    
extern int32_t inBuffer1[], inBuffer2[], outBuffer1[], outBuffer2[], *currentInBuffer, *currentOutBuffer, bufferCount;
extern bool swapBuffers;
extern int phase, sinOut;
extern const unsigned int freqSampling;
extern const unsigned int freqTest;
extern int32_t stabValue1;
extern int16_t buffer69;
extern bool flagCap;

#ifdef	__cplusplus
}
#endif

#endif	/* DEMO1_H */

