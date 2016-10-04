#include <elapsedMillis.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

int senderId;

RF24 radio(9, 10);

// this is not the channel address, but the transmitter address
const uint64_t pipe = 0xE8E8F0F0E1LL;

// Output LED
int LEDpin = 3;

//Time to turn the light off
elapsedMillis timeElapsed; 
// delay in milliseconds between blinks of the LED
unsigned int interval = 10000;
// state of the white LED 
boolean ledState = LOW;

void setup(void) {
Serial.begin(9600);

radio.begin();

// the following statements improve transmission range
radio.setPayloadSize(2); // setting the payload size to the needed value
radio.setDataRate(RF24_250KBPS); // reducing bandwidth

radio.openReadingPipe(1, pipe); // Open one of the 6 pipes for reception

radio.startListening(); // begin to listen

// Enable all the LED pins as output
pinMode(LEDpin, OUTPUT);
digitalWrite(LEDpin, LOW); // this is unnecessary but good practice nonetheless

}

void loop(void) {

// Turns off all the LEDs
//digitalWrite(LEDpin, LOW);

if (radio.available()) {

// this while is here to throw away all the packets but the last one
bool done = false;
while (!done) {

// read and write expect a reference to the payload (& symbol)
// second argument is the packet length in bytes (sizeof(int) == 2)
done = radio.read(&senderId, 1);
}

//Light up the correct LED for 50ms
if (senderId == 64)
{
digitalWrite(LEDpin, HIGH);

 }
  else
  {
 if ((timeElapsed / 5) > (interval))
 {
 digitalWrite(LEDpin, LOW);
 timeElapsed = 0;  
}
  }
}
}
