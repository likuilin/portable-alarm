#include <avr/sleep.h>
#include <avr/power.h>
int inputPin_photo = A0; 
int photo_val=0;
int interrupt_pin=3;
volatile int awake_flag = 1;

void setup() {
  
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin),buttonPressed1,RISING);
  pinMode(interrupt_pin, INPUT);
}

void loop() {
  Serial.print("awakeflag:");
  Serial.println(awake_flag);

  if (awake_flag){
    /*do code*/
  }
  else{
    sleep(1);
  }
  
}


void buttonPressed1()           //ISR function excutes when push button at pinD2 is pressed
{ 
  awake_flag=!awake_flag;

}
