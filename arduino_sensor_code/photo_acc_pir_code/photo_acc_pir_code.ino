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
int motionPin = 2;              // choose the input pin (for PIR sensor)
int photoPin = A0;              // for the photosensor
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int photo_val=0;

// Arduino asserts this when motion detected:
#define MOTION_PIN 7
// Arduino turns on the alarm when this signal is high:
#define ALARM_PIN 8

#define BUZZ_PIN 5
#define BUZZ_VALUE_HIGH 128
#define BUZZ_VALUE_LOW 0

#define ACCEL_SENSITIVITY .3

// Has the backpack been moved?
int moved = 0;

// Are we currently buzzing? (Toggled by T1)
int buzz_signal = 0;

// Returns 1 if acceleometer moved, 0 if not
int accelerometerMoved() {
    return abs(curr_x-prev_x) > ACCEL_SENSITIVITY || abs(curr_y-prev_y) > ACCEL_SENSITIVITY || abs(curr_z-prev_z)> ACCEL_SENSITIVITY;
}

// Enables timer t1 to run at 2Hz
void enableT1() {
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = 31250;
    TCNT1 = 0;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12);
    TIMSK1 |= (1 << OCIE1A);
    interrupts();
}

// Timer T1 interrupt:
ISR(TIMER1_COMPA_vect) {
    //analogWrite(BUZZ_PIN, buzz ? 128 : 0);
    buzz_signal = !buzz_signal;
}

ISR(TIMER0_COMPA_vect) {
}

ISR(TIMER2_COMPA_vect) {
}


void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(motionPin, INPUT);     // declare sensor as input
  pinMode(photoPin, INPUT);
  pinMode(MOTION_PIN, OUTPUT);
  pinMode(ALARM_PIN, INPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  Wire.begin();
  enableT1();
 
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
 
void loop() {
  curr_x=accel.getCalculatedX();
  curr_y=accel.getCalculatedY();
  curr_z=accel.getCalculatedZ();

  moved = 0;
  if (moved) Serial.println("asdf1");
  moved = moved || (analogRead(photoPin) > 100);  // Backpack opened
  if (moved) Serial.println("asdf2");
  moved = moved || (digitalRead(motionPin) == HIGH);  // Motion detected
  if (moved) Serial.println("asdf3");
  moved = moved || (accelerometerMoved());       // Accelerometer
  if (moved) Serial.println("asdf4");
  Serial.println();

  prev_x=curr_x;
  prev_y=curr_y;
  prev_z=curr_z;

  // Assert motion pin
  digitalWrite(MOTION_PIN, (moved) ? HIGH : LOW);

  // Buzz
  int buzz = digitalRead(ALARM_PIN) == HIGH;
  analogWrite(BUZZ_PIN, (buzz & buzz_signal) ? BUZZ_VALUE_HIGH : BUZZ_VALUE_LOW);

  // Debug LED:
  digitalWrite(13, moved ? HIGH : LOW);

  delay(50);
}
