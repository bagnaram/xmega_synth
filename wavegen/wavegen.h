#ifndef WAVEGEN_H_
#define WAVEGEN_H_

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
 
 
#define INTERRUPT_PERIOD 512
#define FINT (F_CPU / INTERRUPT_PERIOD) 
#define FS (FINT)


extern const unsigned int a_major[8];


/**
 * Sets up the hardware PWM in the XMEGA
 */
void wavegen_pwmInit();
/**
 * Sets up the internal oscillator in the XMEGA.
 * Sets the internal oscillator to 32Mhz and waits 
 * until the ooscillator is stable.
 */
void wavegen_clock_init (void);
/**
 *  Sets the PWM frequency per channel. Channels are 0,1,2.
 */
void wavegen_pwmSet(char channel, int value) ;
/**
 *  Copies a sinewave from the program memory into the buffer
 */
void wavegen_sineWave(char wavetableNum);
/**
 *  Copies a noise sample into the buffer
 */
void wavegen_noiseWave(char wavetableNum);
void wavegen_setFrequency3(unsigned int freq);
void wavegen_setFrequency2(unsigned int freq);
void wavegen_setFrequency(unsigned int freq);
/**
 *  Will set the specific bank (1,2,3) to be enabled or disabled.
 *  startStop designates enabled.
 */
void wavegen_setSound(char startStop, char bankNum);
/**
 *  Copies a noise sample into the buffer
 */
void wavegen_disableSound(char bankNum);




#endif 
