/* Using the Elapse Millis library  
 *  http://playground.arduino.cc//Code/ElapsedMillis
 */

#include <elapsedMillis.h>

// Sets pins for the RGB color
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

// Photo cell setup 
int photocellPin = 0;
unsigned int photocellReading; 

// Turn on white light for set time
int led = 6;
elapsedMillis timeElapsed; 
// delay in milliseconds between blinks of the LED
unsigned int interval = 5000;
// state of the white LED 
boolean ledState = LOW;

// Motion stuff
int motion = A1;
int motionSensor = LOW;

//Mic stuff - have NOT tested with a mic
int mic = A2;
int micSensor = 0;

void setup() { 
  Serial.begin(9600);
  // initialize the motion pin as input
  pinMode(motion, INPUT);
    // initialize the mic pin as input
  pinMode(mic, INPUT);
  
 // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
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
// start the color 
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
    delay(5);

  }
    }
  }
}


void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);

// Motion and Mic sensors
long motionSensor = digitalRead(motion);
long mic = analogRead(micSensor);
  if((motionSensor == HIGH) && (photocellReading < 100)){
     digitalWrite (led, HIGH);
     Serial.print (motionSensor);
  }
  else
  {
 if (timeElapsed > interval) 
 {       
    ledState = LOW;    // toggle the state from HIGH to LOW to HIGH to LOW ... 
    digitalWrite(led, ledState);
    timeElapsed = 0;       // reset the counter to 0 so the counting starts over...
  }
  }
}




    
