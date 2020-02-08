#include <Wire.h>                 // Must include Wire library for I2C
#include "SparkFun_MMA8452Q.h"    // Click here to get the library: http://librarymanager/All#SparkFun_MMA8452Q

MMA8452Q accel;                   // create instance of the MMA8452 class

int curr_x;
int curr_y;
int curr_z;

int prev_x;
int prev_y;
int prev_z;

const int lowestPin = 2;
const int highestPin = 13;

int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int inputPin_photo = A0;               // for the photosensor
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int photo_val=0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(inputPin_photo, INPUT);
  Wire.begin();

  if (accel.begin() == false) {
    Serial.println("Not Connected");
    while (1);
  }
  curr_x=accel.getCalculatedX();
  curr_y=accel.getCalculatedY();
  curr_z=accel.getCalculatedZ();

  prev_x=accel.getCalculatedX();
  prev_y=accel.getCalculatedY();
  prev_z=accel.getCalculatedZ();
}
 
void loop(){
  curr_x=accel.getCalculatedX();
  curr_y=accel.getCalculatedY();
  curr_z=accel.getCalculatedZ();
  photo_val=analogRead(inputPin_photo);
  if (photo_val>100){
    Serial.println("backpack is opened");
    analogWrite(10, 128);
    delay(1000);
    analogWrite(10, 0);
    delay(1000);
  }
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    
      // we have just turned on
      Serial.println("Motion detected!");

      analogWrite(10, 128);
      delay(1000);
      analogWrite(10, 0);
      delay(1000);
    
  }
  
  
  if ( abs(curr_x-prev_x) > .1 || abs(curr_x>prev_x)>.1 || abs(curr_z-prev_z)>.1 ) {      // Wait for new data from accelerometer
   
    Serial.println("backpack_moved");
    analogWrite(10, 128);
    delay(1000);
    analogWrite(10, 0);
    delay(1000);
      
  }

  prev_x=curr_x;
  prev_y=curr_y;
  prev_z=curr_z;
  delay(50);
  
  
}
