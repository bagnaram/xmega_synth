#define F_CPU 32000000UL
 
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
//#include <ioavr.h>

 
 
#define INTERRUPT_PERIOD 512
#define FINT (F_CPU / INTERRUPT_PERIOD) // 16kHz?
#define FS (FINT)


const unsigned char sinetable[256] PROGMEM = { 
128,131,134,137,140,143,146,149,152,156,159,162,165,168,171,174, 
176,179,182,185,188,191,193,196,199,201,204,206,209,211,213,216, 
218,220,222,224,226,228,230,232,234,236,237,239,240,242,243,245, 
246,247,248,249,250,251,252,252,253,254,254,255,255,255,255,255, 
255,255,255,255,255,255,254,254,253,252,252,251,250,249,248,247, 
246,245,243,242,240,239,237,236,234,232,230,228,226,224,222,220, 
218,216,213,211,209,206,204,201,199,196,193,191,188,185,182,179, 
176,174,171,168,165,162,159,156,152,149,146,143,140,137,134,131, 
128,124,121,118,115,112,109,106,103,99, 96, 93, 90, 87, 84, 81,  
79, 76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39,  
37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 18, 16, 15, 13, 12, 10,  
9,  8,  7,  6,  5,  4,  3,  3,  2,  1,  1,  0,  0,  0,  0,  0,   
0,  0,  0,  0,  0,  0,  1,  1,  2,  3,  3,  4,  5,  6,  7,  8,   
9,  10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 27, 29, 31, 33, 35,  
37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76,  
79, 81, 84, 87, 90, 93, 96, 99, 103,106,109,112,115,118,121,124 
};

//lookup table for wavetable;
unsigned char wavetable[256];


unsigned int frequencyCoef=100;
unsigned int frequencyCoef2=100;
char soundEnabled=1;
char soundPWM=1;
char soundOn=0;

int currentVoice=0;

//function definations
void pwmSet(char channel, int value) ;
void sineWave();
//void noiseWave();
void setFrequency2();
void setFrequency();
 
int main() 
{
  //RC oscillator at 32MHZ
  clock_init();

  //initialize DAC
  pwmInit();

  setFrequency(440);
  setFrequency2(80);
  //setFrequency(10);
  PORTA.DIRSET=0xFF;
  PORTC.DIRSET=0xFF;
  PORTD.DIRSET=0xFF;
  
  
  while(1)
  {
    //noiseWave();
  }
}

ISR(TCC0_CCA_vect)
{

  static unsigned int phase0;
  static unsigned int sig0;
  static unsigned char flag=0;
  static unsigned int tempphase;

  //TCD0_CNT=0x0000;
  if(soundPWM)
  {
    tempphase=phase0+frequencyCoef;
    sig0=wavetable[phase0>>8];
    phase0=tempphase;
    //output the sample;
    pwmSet(0,sig0);
    flag^=1;
    PORTA.OUT=(flag<<4);
  }
  else//square wave
  {
    flag^=1;
    PORTA.OUT=(flag<<4);
  }
}

ISR(TCC0_CCB_vect)
{

  static unsigned int phase0;
  static unsigned int sig0;
  static unsigned char flag=0;
  static unsigned int tempphase;

  //TCC0_CNT=0x0000;
  if(soundPWM)
  {
    tempphase=phase0+frequencyCoef2;
    sig0=wavetable[phase0>>8];
    phase0=tempphase;
    //output the sample;
    pwmSet(1,sig0);
  }
  else//square wave
  {
  }
}


void sineWave()
{
  int i=0;
  for(i=0;i<256;++i)
  {
    wavetable[i]=pgm_read_byte_near(sinetable+i);
  }
}

void noiseWave()
{
  int i=0;
  for(i=0;i<256;++i)
  {
    wavetable[i]=(char)rand();
  }
}

void pwmInit()
{
  sineWave();//load sinetable into wavetable;
  
  // Setup timer0 to send a sample every interrupt 
  //
  TCC0_CTRLA = 0b00000001; // divide clock by 1 
  TCC0_PER=0x00FF;//8 bit resolution
  TCC0.INTCTRLB=0b00000101;//enable compare match interrupt 
  // Setup timer0 to send a sample every interrupt 
  //
  //TCC0_CTRLA = 0b00000001; // divide clock by 1 
  //TCC0_PER=0x00FF;//8 bit resolution
  //TCC0.INTCTRLB=0b00000001; //enable compare match interrupt


  //enable low level interrupt levels
  PMIC.CTRL |= PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm;
  sei();


  // Setup timer0 B-D to do pulse width modulation on speaker pin 
  //
  TCD0_CTRLA = 0b00000001; // divide clock by 1 
  TCD0_CTRLB = 0b00010011;//compare channel A & waveform generation mode

  TCD0_PER = 0x00FF; // set resolution to 12 bits 
}

void pwmSet(char channel, int value) 
{ 

  switch(channel){ 
    case 0: 
      TCD0_CCA = value; // duty cycle A 
      break; 
    case 1:    
      TCD0_CCB = value;   // B 
      break; 
    case 2:    
      TCD0_CCC = value;   // C 
      break; 
    case 3:    
      TCD0_CCD = value;   // D 
  }    
  TCD0_CTRLB = 0xF3; /* enable frequency generation for all 4 Channels */ 
} 

void setFrequency(unsigned int freq)
{
  if(soundPWM)
  {
    unsigned long templong=freq;
      frequencyCoef=templong*65536/FS;
  }
  else
  {
    unsigned long period=F_CPU/(2*freq);
    cli();
    TCC0_CCA=period;

  }
}
void setFrequency2(unsigned int freq)
{
  if(soundPWM)
  {
    unsigned long templong=freq;
      frequencyCoef2=templong*65536/FS;
  }
  else
  {
    unsigned long period=F_CPU/(2*freq);
    cli();
    TCD0_CCA=period;

  }
}

void clock_init (void)
{
  // Set internal oscillator to 32MHz
  OSC.CTRL = OSC_RC32MEN_bm;
  //Wait until oscillator is stable
  while ((OSC.STATUS & OSC_RC32MRDY_bm) == 0);
  // System clock selection
  CCP = CCP_IOREG_gc;
  CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
  // DFLL (Auto Calibration)
  DFLLRC32M.CTRL = DFLL_ENABLE_bm;
}
