#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#define INTERRUPT_PERIOD 512
#define FINT (F_CPU / INTERRUPT_PERIOD) 
#define FS (FINT)

/**
  *includes for lcd display library
  */
#include "../lcd8bit/lcd8bit.h" 

//set up pins for PS/2 I2C interface
#define KB_CLK PIN2_bm  //!< set the keyboard clock pin to PORTF.2
#define KB_DATA PIN1_bm //!< set the keyboard data pin to PORTF.1
#define KB_PORT PORTF //!< define the keyboard port as PORTF

volatile uint8_t kbd_data;
volatile uint8_t char_waiting;
uint8_t started;
uint8_t bit_count;
uint8_t shift;
uint8_t caps_lock;
uint8_t extended;
uint8_t release;


/**
  * This function initialiazes the ports on the PS/2 port
  * Sets up timer interrupts for PORTF.
  */
void keyboard_init();

char keyboard_render_scan_code(uint8_t data);

uint8_t keyboard_read_char();

void keyboard_clock_init(void);

#endif
