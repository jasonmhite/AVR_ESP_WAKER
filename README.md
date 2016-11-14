# Instructions

![pinout](http://arduinoelettronica.files.wordpress.com/2012/02/attiny85_pinout.png)

Make these connections:

* VCC -> 3.3V (can be in parallel with the ESP)    
* GND -> GND
* PCINT3 -> one side of the reed switch
* Other side of the reed switch -> ground
* PC0 -> connect to ESP Reset pin with a 10-100 K resistor in series between them

The internal pullup resistor is switched on for the input pin, so when the
reed switch is open the input pin will be high. Very little current will
flow because of the pullup resistor. This helps alleviate false triggers.
When the switch closes it will pull the pin to ground and trigger the interrupt
and wake the ATTiny with a 30 low pulse.

The ATTiny will normally hold the reset pin of the ESP high because that's
normal operations. The resistor between PC0 and reset is to limit the
current flow.

The makefile is designed for Linux, you're on your own for others. To build, 
make sure you have avr-gcc and avrdude installed and in your PATH. Run
`make` to build the code.

Make sure to flash the ATTiny BEFORE permanently fixing it into your circuit
or it'll be a headache.

To flash, first grab an Arduino like an Uno or Nano. Flash the ArduinoISP
sketch as on https://www.arduino.cc/en/Tutorial/ArduinoISP but stop at 
step 4. Instead, wire up the correct connections to the ATTiny (MISO on the
Arduino to MISO on the ATTiny and so on). Set the variable in the Makefile
to match the serial port of your Arduino and then do `make flash`.
