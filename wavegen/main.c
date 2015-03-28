#include "../config.h"
#include "wavegen.h"
#define INTERRUPT_PERIOD 512
#define FINT (F_CPU / INTERRUPT_PERIOD) // 16kHz?
#define FS (FINT)
int main()
{
  wavegen_noiseWave(0);
  wavegen_sineWave(1);

  wavegen_clock_init();

  wavegen_pwmInit();

  PORTA.DIRSET=0xFF;
  PORTC.DIRSET=0xFF;
  PORTD.DIRSET=0xFF;

  wavegen_setSound(1,1);
  wavegen_setSound(1,2);
  wavegen_setFrequency(80);
  wavegen_setFrequency2(40);
  extern int soundPWM;
  soundPWM=1;
  while(1)
  {
  }
}
