/* Using the following libraries 
 *  Elapse Millis library  
 *  http://playground.arduino.cc//Code/ElapsedMillis
 *  Using some code based on the following for the radion communication 
 *  https://just4electronics.wordpress.com/2015/08/30/wireless-remote-using-2-4-ghz-nrf24l01-simple-tutorial-using-of-nrf24l01-arduino/
 */

#include <elapsedMillis.h>

//Int the radio communication 
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

int transmitterId = 0;

RF24 radio(9, 10);

// Transmitter address
const uint64_t pipe = 0xE8E8F0F0E1LL;

// Sets pins for the RGB color
const int redPin = 6;
const int greenPin = 5;
const int bluePin = 3;

// Photo cell setup 
int photocellPin = 0;
unsigned int photocellReading; 

// Turn on white light for set time
const int led = 2;
elapsedMillis timeElapsed; 
// delay in milliseconds between blinks of the LED
unsigned int interval = 10000;
// state of the white LED 
boolean ledState = LOW;

// Motion stuff
int motion = A1;
int motionSensor = LOW;

//Mic stuff - have NOT tested with a mic
int mic = A2;
int micSensor = 0;

void setup() { 
// CHANGE THIS PER EACH TRANSMITTER, from 0 to 255
transmitterId = 64;
radio.begin();

// the following statements improve transmission range
radio.setPayloadSize(2); // setting the payload size to the needed value
radio.setDataRate(RF24_250KBPS); // reducing bandwidth
radio.openWritingPipe(pipe); // set the transmitter address

Serial.begin(9600);
// initialize the motion pin as input
pinMode(motion, INPUT);
// initialize the mic pin as input
pinMode(mic, INPUT);
  
// initialize the digital pin as an output.
pinMode(led, OUTPUT);
digitalWrite(led, LOW);

// Set the RGB LED to off on start 
setColourRgb(0,0,0);

}

void loop() 
{

  // Start photo cell reading 
photocellReading = analogRead(photocellPin);
// if the photo cell reading is > x turn the RGB light off.
// Adjust "> x" to tune how seansative the photo cell is. "2" works for most situations. 
if (photocellReading > 100){
setColourRgb(0,0,0);
}
else {
  
  unsigned int rgbColour[3];

  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  


  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      
      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
      delay(4); // Adjust this up to slow the RGB color change 

  }
    }
  }
}

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
{
    // if the motion sensor is LOW pass data over the radio 
  if (motionSensor == LOW);{
// some implementations automatically shut down the radio after a transmission: this ensures the radio is powered up before sending data
radio.powerUp();

// read and write expect a reference to the payload (& symbol)
// second argument is the packet length in bytes (sizeof(int) == 2)
radio.write(&transmitterId, 0);
  }
}
// Motion and light sensor to turn on the bright LEDs and pass a 1 over the radio to turn on the other light 
long motionSensor = digitalRead(motion);
  if((motionSensor == HIGH) && (photocellReading < 100)){ // This is adjusted high for testing Lower this to 1-3 
     digitalWrite (led, HIGH);
     ledState = HIGH; 
     if(motionSensor == HIGH)
     radio.write(&transmitterId, 1);
  }
  else
  {
     radio.write(&transmitterId, 0);
 if ((timeElapsed / 5) > (interval)) //Length of time the light in the room is on 
 {       
    ledState = LOW;    // toggle the state from HIGH to LOW to HIGH to LOW ... 
    digitalWrite(led, ledState);
    timeElapsed = 0;       // reset the counter to 0 so the counting starts over...
  }
  }
}








    

