#include "config.h"
 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>

#include "lcd8bit.h" 

int main()
{
  // Initialise the LCD Display
  lcd_init();
  // clear display
  lcd_clear_and_home();
   
   
  // display "Hello World" on LCD first line
  lcd_line_one();
  lcd_write_string_p(PSTR("TROLOLO! \0"));    // message from flash space
   
   
  // display "Currently: xxx" with the variable i on LCD second line
  uint32_t i;
  char MsgTemp[25];
   
  i = 14;
   
  lcd_line_two();
  sprintf_P(MsgTemp, PSTR("Currently: %d"), i);
  lcd_write_string_0(MsgTemp);
  while(1)
  {
  }
}
