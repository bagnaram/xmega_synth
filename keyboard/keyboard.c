// keyboard.c
// for NerdKits with ATmega168
// hevans@nerdkits.com
//
// Designed for use with the USB NerdKit running with ATMega168. Datasheet page
// numbers refer to ATMega168 datasheet.

#include "keyboard.h"

//PIN configuration
//PF2 (PCINT12) -  CLK pc4
//PF1           -  DATA pc2

    char st[20]="                ";
volatile uint8_t kbd_data;
volatile uint8_t char_waiting;
uint8_t started;
uint8_t bit_count;
uint8_t shift;
uint8_t caps_lock;
uint8_t extended;
uint8_t release;

/**
  * include for PS/2 to ASCII mapping
  */
const char keymap[128] PROGMEM = {'?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','q','1','?','?','?','z','s','a','w','2','?','?','c','x','d','e','4','3','?','?',' ','v','f','t','r','5','?','?','n','b','h','g','y','6','?','?','?','m','j','u','7','8','?','?','?','k','i','o','0','9','?','?','?','?','l','?','p','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?'};
/**
  * Interrupt service routine for PORTF. Triggered when PORTF pin 2
  * transitions from high to low. Handles PS/2 clock input and reads
  * data in bit by bit.
  */
ISR(PORTF_INT1_vect)
{
  //make sure clock line is low, if not ignore this transition
if(KB_PORT.IN & (1<<KB_CLK)){
    return;
}
  //if we have not started, check for start bit on DATA line
  if(!started){
    if ( (KB_PORT.IN & (1<<KB_DATA)) == 0 ) {
      started = 1;
      bit_count = 0;
      kbd_data = 0;
      //printf_P(PSTR("%d"),started);
      return;
    }
  } else if(bit_count < 8) { //we started, read in the new bit
    //put a 1 in the right place of kdb_data if PC2 is high, leave
    //a 0 otherwise
    if((KB_PORT.IN & KB_DATA) == KB_DATA){
      kbd_data |= (1<<bit_count);
    }
    bit_count++;
    return;
  } else if(bit_count == 8){ //pairty bit 
    //not implemented
    bit_count++;
    return;
  } else {  //stop bit
    //should check to make sure DATA line is high, what to do if not?
    started = 0;
    bit_count = 0;
//
  }
static char last_key=0;

  if(kbd_data == 0xF0){ //release code
    release = 1;
    //kbd_data = 0;
    return;
  }
  else if(kbd_data == 0xE0)//extended code
  { 
    return; //do nothing
  }
  else if (kbd_data == 0x12 ||kbd_data == 0x59 ) { //hanlde shift key
    if(release == 0){
      shift = 1;
    } else {
      shift = 0;
      release = 0;
    }
    return;
  } else { //not a special character
      if(release==2)
        release=0;
      char_waiting = 1;
  }
}

/**
  * Initialiaze the clock to be 32Mhz
  */
void keyboard_clock_init(void)
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

char keyboard_render_scan_code(uint8_t data)
{
  volatile static char last_key=0;
  char to_ret = pgm_read_byte(&(keymap[data])); //grab character from array
  if(shift){
    to_ret -= 0x20;
  }
    last_key=to_ret;
    return to_ret;
}

uint8_t keyboard_read_char(){
  while(!char_waiting){
     //wait for a character
  }
  if(release==1)
    release=2;
  char_waiting = 0;
  return kbd_data;
}

/**
  * This function initialiazes the ports on the PS/2 port
  * Sets up timer interrupts for PORTF.
  */
void keyboard_init()
{

  started = 0;
  kbd_data = 0;
  bit_count = 0;


  //make PC4 input pin
  //DDRC &= ~(1<<PC4);
  //turn on pullup resistor
  KB_PORT.DIRSET = ~(KB_CLK | KB_DATA);
  KB_PORT.PIN1CTRL=PORT_OPC_PULLUP_gc;
  KB_PORT.PIN2CTRL=PORT_OPC_PULLUP_gc |  PORT_ISC_FALLING_gc;

  //Enable PIN Change Interrupt 1 - This enables interrupts on pins
  //PCINT14...8 see p70 of datasheet


  KB_PORT.INT1MASK=(KB_CLK);

  /**
    * Make this interrupt medium level so it is higher
    * priority than the timer interrupts!
    */
  KB_PORT.INTCTRL=PORT_INT1LVL_MED_gc;


  //enable low level interrupts
  PMIC.CTRL |=PMIC_LOLVLEN_bm;

  
  //PCICR |= (1<<PCIE1);
  
  //Set the mask on Pin change interrupt 1 so that only PCINT12 (PC4) triggers
  //the interrupt. see p71 of datasheet
  //PCMSK1 |= (1<<PCINT12);
}


