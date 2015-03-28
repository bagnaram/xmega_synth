# xmega_synth
8 bit synthesizer ATXMEGA256D3

This is an AVR synthesizer build using the ATXMEGA256D3 microcontroller. See the LaTEX
document in bagnaram/xmega_synth/report for in-depth documentation

REQUIREMENTS:
The "make program" target relies on an AVRISP programmer. The default configuration
is using an AVR-ISP MKII USB programmer. This uses the avrdude tool to program the micro.
Have avrdude installed!

The "make" target generates a hex target using the avr-gcc toolkit. Have that installed.

COMPILE DOXYGEN DOCUMENTATION:
Simply run doxygen doxyfile to create a doc listing. The default will build a PDF document.

