#include "keyboard.h"



/**
  * include for PS/2 to ASCII mapping
  */


/**
 * Debug routine for keyboard unit test. Call this
 * main routine without linking.
 */
int main()
{
  // Initialise the LCD Display
  lcd_init();
  // clear display
  lcd_clear_and_home();
  lcd_line_one();

    lcd_write_string_0("ready");

  //fire up the LCD

  keyboard_init(); 
  
  //enable interrupts
  sei();

keyboard_clock_init();
  //debug LED - output
  //DDRC |= (1<<PC3);

  uint8_t key_code = 0;

  char str_buf[21];
  uint8_t buf_pos = 0;
  str_buf[0] = str_buf[1] = 0x00;

  char text[255];


  while(1) {
    if(char_waiting){
      key_code = keyboard_read_char();
      if(key_code == 0x5A){ // enter key, clear the line
        buf_pos = 0;
        str_buf[0] = str_buf[1] = 0x00;
      } else if (key_code == 0x66){ //backspace
        buf_pos--;
        str_buf[buf_pos] = 0x00;
      } else {
        str_buf[buf_pos] = keyboard_render_scan_code(key_code);
        if(str_buf[buf_pos]!=0)
          buf_pos++;
        str_buf[buf_pos] = 0x00;
      }
    } 
   
    lcd_line_one();
    sprintf(text,"%-20s",str_buf);
    //sprintf(text,"%X,%d",key_code,key_code);
    lcd_write_string_0(text);
    if(shift){
      lcd_line_two();
    sprintf(text,"SHIFT");
    lcd_write_string_0(text);
// fprintf_P(&lcd_stream,PSTR("SHIFT"));
    } else {
      lcd_line_two();
sprintf(text,"            ");
    lcd_write_string_0(text);
//fprintf_P(&lcd_stream,PSTR("          "));
    }
    
  }
  
  return 0;
}

