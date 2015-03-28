/*
 * \file main.c 
 * \version 1.12
 * \author matt bagnara
 * CONTRIBUTIONS BY:
 * PLATFORM: atxmega256D3
 * \brief avrsynth for senior project 2013
 */

#include "config.h"
 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
  *includes for linkedlist library
  */
#include "list/list.h" 

/**
  *includes for lcd display library
  */
#include "lcd8bit/lcd8bit.h" 

/**
  *Definations for SD CARD library
  */
#include "sd_card/ff.h"
#include "sd_card/integer.h"

FATFS FatFs;	//!< FatFs work area 
FIL *fp;		//!< fpe object 

/**
  * User Provided RTC Function called by FatFs module       
  * Used to provide a Timestamp for SDCard files and folders
  */
DWORD get_fattime (void)
{
	// Returns current time packed into a DWORD variable 
	return	  ((DWORD)(2014 - 1980) << 25)	// Year 2014 
	| ((DWORD)8 << 21)				// Month 7 
	| ((DWORD)2 << 16)				// Mday 28 
	| ((DWORD)20 << 11)				// Hour 0..24
	| ((DWORD)30 << 5)				// Min 0 
	| ((DWORD)0 >> 1);				// Sec 0
}


/**
  * Definations for wavegen library
  */
#include "wavegen/wavegen.h"

/**
  *includes for PS/2 keyboard interface library 
  */
#include "keyboard/keyboard.h" 
/**
  *qwerty key map for tone generation
  */
const char qwerty[26] = {'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m'};

/**
  * End execution and display an error message on LCD
  */
void die(char* message)
{
  lcd_clear_and_home();
  lcd_line_one();
  lcd_write_string_0("ERR:");    // message from file on SD
  lcd_line_two();
  lcd_write_string_0(message);    // message from file on SD

  //end execution!
  exit(1);
}
void set_bank(unsigned char bank)
{
  lcd_line_one();
  char selected=0;
  const  char CONF_BANK_MENU_SIZE=5;
  char *conf_bank_menu[5]={"->BACK        ","->Set Sine      ","->Set Square      ","->Set Triangle   ","->Set Noise      "};
  unsigned char menu_index=0;
  uint8_t key_code=0; //holds code from PS/2 input
  while(1)
  {

    for(menu_index=0;menu_index<CONF_BANK_MENU_SIZE;++menu_index)
    {
      if(menu_index % 2 ==0)
      {
        lcd_line_one();
      }
      else
      {
        lcd_line_two();
      }
      if(menu_index<= (selected/2) * 2+1 && menu_index >= (selected/2)*2 ) 
      {
      if(selected==menu_index)
      {
        lcd_write_string_0(conf_bank_menu[menu_index]);
      }
      else 
      {
        lcd_write_string_0(conf_bank_menu[menu_index]+2);
      }
      }
      if(selected==CONF_BANK_MENU_SIZE-1 && (menu_index+1) % 2 ==1)
      {
        lcd_line_two();
        lcd_write_string_0("                ");
      }
    }


      key_code=keyboard_read_char();
      if(release==0 && key_code==0x5A) //for return key pressed
      {
        key_code=0;
        if(selected==0)
        {
          return;
        }
        else if(selected==1)
        {
          wavegen_sineWave(bank);//load sinewave into wavetable 1 memory
          return;
        }
        else if(selected==2)
        {
          wavegen_squareWave(0);//load squarewave into wavetable 1 memory
          return;
        }
        else if(selected==3)
        {
          //wavegen_sineWave(0);//load triangle into wavetable 1 memory
          return;
        }
        else if(selected==4)
        {
          wavegen_noiseWave(bank);//load sinewave into wavetable 1 memory
          return;
        }

      }
      else if(release==0 && key_code==0x72) //for arrow up pressed
      {
        selected++;
        key_code=0;
        //_delay_ms(100);
      }
      else if(release==0 && key_code==0x75) //for arrow down pressed
      {
        selected--;
        key_code=0;
        //_delay_ms(100);
      }
    if(selected<=0)
    {
      selected=0;
    }
    if(selected>=CONF_BANK_MENU_SIZE)
    {
      selected=CONF_BANK_MENU_SIZE-1;
    }

  }

}

int main()
{
  // Initialise the LCD Display
  lcd_init();
  //keyboard initialization routine
  keyboard_init();
  wavegen_clock_init();
  wavegen_pwmInit();
  // clear display
  lcd_clear_and_home();

  /*
  lcd_line_one();
  lcd_write_string_0("Welcome to");
  _delay_ms(100);
  lcd_clear_and_home();
  lcd_write_string_0("   AVRSYNTH   ");
  _delay_ms(100);
  */

  /**
    * these functions are preppers for memory in
    * the routines
    */
  UINT bw;
  unsigned int count=0;
  wavegen_sineWave(0);//load sinewave into wavetable 1 memory
  wavegen_sineWave(1);//load sinewave into wavetable 2 memory
  wavegen_sineWave(2);//load sinewave into wavetable 2 memory
  wavegen_noiseWave(2);//load noise into wavetable 3 memory

  /**
    * Start our main menu routine
    */
main_menu:
  lcd_line_one();
  lcd_write_string_0("ready");
  char selected=0;
  const  char MENU_SIZE=4;
  char *menu[4]={"->Use Keyboard  ","->Play from SD  ","->Configure     ","->About Device  "};
  int menu_index=0;
  uint8_t key_code=0; //holds code from PS/2 input
  while(1)
  {

    for(menu_index=0;menu_index<MENU_SIZE;++menu_index)
    {
      if(menu_index % 2 ==0)
      {
        lcd_line_one();
      }
      else
      {
        lcd_line_two();
      }
      if(menu_index<= (selected/2) * 2+1 && menu_index >= (selected/2)*2 ) 
      {
      if(selected==menu_index)
      {
        lcd_write_string_0(menu[menu_index]);
      }
      else 
      {
        lcd_write_string_0(menu[menu_index]+2);
      }
      }
    }


      key_code=keyboard_read_char();
      if(release==0 && key_code==0x5A) //for return key pressed
      {
        key_code=0;
        if(selected==0)
        {
          goto play_wave;
        }
        else if(selected==1)
        {
          goto read_sd;

        }
        else if(selected==2)
        {
          goto config_menu;
        }
        else if(selected==3)
        {
          goto about;
        }

      }
      else if(release==0 && key_code==0x72) //for arrow up pressed
      {
        selected++;
        key_code=0;
        //_delay_ms(100);
      }
      else if(release==0 && key_code==0x75) //for arrow down pressed
      {
        selected--;
        key_code=0;
        //_delay_ms(100);
      }
    if(selected<=0)
    {
      selected=0;
    }
    if(selected>=MENU_SIZE)
    {
      selected=MENU_SIZE-1;
    }

  }

  /**
    * Start our config menu routine
    */
config_menu:
  lcd_line_one();
  selected=0;
  const  char CONF_MENU_SIZE=5;
  char *conf_menu[5]={"->BACK          ","->Set Bank1      ","->Set Bank2      ","->Set Bank3      ","->Debug Mode     "};
  menu_index=0;
  key_code=0; //holds code from PS/2 input
  while(1)
  {

    for(menu_index=0;menu_index<CONF_MENU_SIZE;++menu_index)
    {
      if(menu_index % 2 ==0)
      {
        lcd_line_one();
      }
      else
      {
        lcd_line_two();
      }
      if(menu_index<= (selected/2) * 2+1 && menu_index >= (selected/2)*2 ) 
      {
      if(selected==menu_index)
      {
        lcd_write_string_0(conf_menu[menu_index]);
      }
      else 
      {
        lcd_write_string_0(conf_menu[menu_index]+2);
      }
      }
      if(selected==CONF_MENU_SIZE-1 && (menu_index+1) % 2 ==1)
      {
        lcd_line_two();
        lcd_write_string_0("                ");
      }
    }


      key_code=keyboard_read_char();
      if(release==0 && key_code==0x5A) //for return key pressed
      {
        key_code=0;
        if(selected==0)
        {
          goto main_menu;
        }
        else if(selected==1)
        {
          set_bank(0);

        }
        else if(selected==2)
        {
          set_bank(1);
        }
        else if(selected==3)
        {
          set_bank(2);
        }
        else if(selected==4)
        {
          goto debug;
        }

      }
      else if(release==0 && key_code==0x72) //for arrow up pressed
      {
        selected++;
        key_code=0;
        //_delay_ms(100);
      }
      else if(release==0 && key_code==0x75) //for arrow down pressed
      {
        selected--;
        key_code=0;
        //_delay_ms(100);
      }
    if(selected<=0)
    {
      selected=0;
    }
    if(selected>=CONF_MENU_SIZE)
    {
      selected=CONF_MENU_SIZE-1;
    }

  }

/**
  * Routine for reading data off the sd card
  */
read_sd:
  f_mount(0, &FatFs);		// Give a work area to the FatFs module 
  // open file
  fp = (FIL *)malloc(sizeof (FIL));
  //create a file
  if (f_open(fp, "file.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) 
  {
    // MSG ID,[BANK 1]{ON/OFF,NOTE NUMBER(freq)},[BANK 2]{ON/OFF,NOTE NUMBER(freq)},[BANK 2]{ON/OFF,NOTE NUMBER(freq)}
    char *text = "0,1,215,0,0,0,0:5,0,0,1,242,0,0:10,1,215,0,0,0,0:14,0,0,0,0,0,0";
    f_write(fp, text, strlen(text), &bw);	// Write data to the file 
    f_close(fp);// Close the file 
    if (bw == strlen(text)) //when entire string is written
    { 
      // Lights LED if data written well
      //PORTC.OUT=0x00;
      //lcd.goto_xy(0,0); lcd.send_format_string("Created:%dB\n", f_size(fp));
    }
		//else led2.Set(1);
  }
	
  // get card volume
  char szCardLabel[12] = {0};
  DWORD sn = 0;
  if (f_getlabel("", szCardLabel, &sn) == FR_OK) 
  {
    char text[255]; 
    sprintf(text,"%s SN:%X",szCardLabel,(unsigned int)sn);
    lcd_line_one();
    lcd_write_string_0(text);    // message from file on SD
    sei();
    //lcd.send_format_string("%s SN:%X\n", szCardLabel, sn);
  }

  /*
   * Read from a file
  */
  char stream[128]="";
  if (f_open(fp, "file.txt", FA_READ ) == FR_OK) 
  {	// Create a file
    UINT br;
    f_read(fp, stream, 255, &br); //copy 255 characters from file to stream

    f_close(fp);// Close the file

    //text[16] = '\0'; //text only 16 characters wide
    //lcd_line_two();
    //lcd_write_string_0(text);    // message from file on SD
  }


  /*
   * Stream parsing from the file text stream
   */

  char* start_index=stream;
  char* end_index=stream;
  char stream_index=0;

  //message linked list data structure
  struct onode* head=malloc(sizeof(struct onode**));
  struct order newOrder; 

  do
  {
    int val=5; //each index value stored here after conversion to int
    end_index=start_index;
    do
    {
      end_index++;
    }while(*end_index!='\0' && *end_index!=',' && *end_index!=':');

    char field[20];
    strncpy(field,start_index,(end_index-start_index));
    field[end_index-start_index+0]='\0';

    val=atoi(field);
    /*
    sprintf(buf,"%d: %s %d ",stream_index,field,val);
  lcd_clear_and_home();
  lcd_line_one();
    lcd_write_string_0(buf);
*/
    if(head==NULL)
    {
      die("malloc fail!");
    }
    if(stream_index==0)
    {
      newOrder.id=val;
    }
    else if(stream_index==1)
    {
      newOrder.bank1_startstop=val;
    }
    else if(stream_index==2)
    {
      newOrder.bank1_note=val;
    }

    else if(stream_index==3)
    {
      newOrder.bank2_startstop=val;
    }
    else if(stream_index==4)
    {
      newOrder.bank2_note=val;
    }

    else if(stream_index==5)
    {
      newOrder.bank3_startstop=val;
    }
    else if(stream_index==6)
    {
      newOrder.bank3_note=val;
    }

    //create temporary new node to add to the list
    pushNode(&head,newNode(&newOrder));
    //printf("%X-%X:%d \n",start_index,end_index,val);

    start_index=end_index+1;
    stream_index++;


    if(*end_index==':')
    {
      //every message processed reset the stream index
      stream_index=0;
      //printf("next message\n");
    }
  }while(*end_index!='\0');

  sort(&head);//sort our list

  //now we loop through our data structure and play the music
  while(1)
  {
    if(getOrderNode(head,count)!=NULL)
    {
    //set the registers for each count
      struct order* gett = (getOrderNode(head,count)->data);
      wavegen_setSound(getOrderStartstop(gett,1),1);//toggle it
      wavegen_setFrequency(getOrderNote(gett,1));

     /* if(count==0)
      { 
     sprintf(stream,"%d: %i %i",gett->id,gett->bank1_startstop,gett->bank1_note);
     die(stream);
     }*/

      wavegen_setSound(getOrderStartstop(gett,2),2);//toggle it
      wavegen_setFrequency2(getOrderNote(gett,2));

      wavegen_setSound(getOrderStartstop(gett,3),3);//toggle it
      wavegen_setFrequency3(getOrderNote(gett,3));
    }


    count++;
    if((count == 15))
    {

      count=0;
      //step values
    }

    //wait here
    _delay_ms(100);
  }
return 0;


/**
  * Routine for handling keyboard realtime playback
  */

play_wave:
/*
  lcd_clear_and_home();
  lcd_line_one();
  lcd_write_string_0(stream);    // message from file on SD
  lcd_line_two();
  lcd_write_string_0(stream+16);    // message from file on SD
*/


  lcd_clear_and_home();
  lcd_line_one();
  lcd_write_string_0("ready");
  char str_buf[2];
  str_buf[0]=str_buf[1]=0x00;
    wavegen_setFrequency(215);//440 hz
    wavegen_setFrequency2(273);//200Hzk
  static char key_last=-1;
  while(1)
  {
    if(char_waiting)
    {
      key_code=keyboard_read_char();
      str_buf[0]=keyboard_render_scan_code(key_code);
      //lcd_line_two();
      //lcd_write_string_0(str_buf);

      int j=0;
      do
      {
        if(key_last!=key_code && str_buf[0]==qwerty[j] && release==0)
        {
          if(j<8)
          {
            wavegen_setFrequency(a_major[j % 8]);
            wavegen_setSound(1,1);//toggle it
          }
          if(j>=8 && j <16)
          {
            wavegen_setFrequency2(a_major[j % 8]);
            wavegen_setSound(1,2);//toggle it
          }
          if(j>=16 && j <24)
          {
            wavegen_setFrequency3(j % 8+5);
            wavegen_setSound(1,3);//toggle it
          }
          key_last=key_code;
          //lcd_clear_and_home();
          //lcd_line_one();
          //lcd_write_string_0("start");
        }
        if(str_buf[0]==qwerty[j] && release>0)
        {
          if(j<8)
          {
          wavegen_setSound(0,1);//toggle it
          }
          if(j>=8 && j <16)
          {
          wavegen_setSound(0,2);//toggle it
          }
          if(j>=16 && j <24)
          {
          wavegen_setSound(0,3);//toggle it
          }
          //lcd_clear_and_home();
          //lcd_line_one();
          //lcd_write_string_0("stop");
          key_last=-1;
        }
        j++;
      }while(j<24);
    }
  }
/**
  * Routine for debug playback 
  */

debug:
  lcd_clear_and_home();
  lcd_line_one();
  lcd_write_string_0("debug");
  char bff[10]="";
    unsigned int freq=6;//start at 30hz
    double freq_index=30;//start at 30hz
    wavegen_setSound(1,1);//enable sound1
    do
    {

      freq=(1000*freq_index-17719)/1965; //this is the magic formula for converting frequency to interrupt value
      //freq=20*freq_index/21+165481/10500;
      wavegen_setFrequency(freq);
      sprintf(bff,"%u    ",(unsigned int)freq_index);
  lcd_line_two();
  lcd_write_string_0(bff);
      keyboard_read_char();
      //_delay_ms(1000);//hold note for 1ms
      //freq_index+=20;
      if(release==0)
      {
      freq_index+=200;
      }
    }while(freq_index<12000);

/**
  * Routine for about screen
  */

about:
  lcd_clear_and_home();
  lcd_line_one();
  lcd_write_string_0("AVRSYNTH");
  char_waiting=0;
  lcd_line_one();
  lcd_write_string_0("AVRSYNTH");
  _delay_ms(1000);
  lcd_line_one();
  lcd_write_string_0("Developed by:");
  lcd_line_two();
  lcd_write_string_0("Matt Bagnara");
  _delay_ms(1000);
  lcd_line_one();
  lcd_write_string_0("Designed for:");
  lcd_line_two();
  lcd_write_string_0("Purdue Univ.");
  _delay_ms(1000);
  lcd_line_one();
  lcd_write_string_0("Contributors:");
  lcd_line_two();
  lcd_write_string_0("ChaN FATFS SD ");
  _delay_ms(500);
  lcd_line_two();
  lcd_write_string_0("card lib.     ");
  _delay_ms(1000);
  lcd_line_two();
  lcd_write_string_0("ATMEL Corp.");
  _delay_ms(1000);
  lcd_line_two();
  lcd_write_string_0("Wade Hsu     ");
  _delay_ms(1000);
  lcd_line_one();
  lcd_write_string_0("Software License:");
  lcd_line_two();
  lcd_write_string_0("GPL 3.0    ");
  _delay_ms(1000);
  lcd_line_one();
  lcd_write_string_0("Hardware License:");
  lcd_line_two();
  lcd_write_string_0("CC-BY-SA 4.0");
  _delay_ms(500);
  lcd_line_two();
  lcd_write_string_0("International");
  goto main_menu;
}

