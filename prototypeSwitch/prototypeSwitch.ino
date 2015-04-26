/*
State change detection (edge detection)

Often, you don't need to know the state of a digital input all the time,
but you just need to know when the input changes from one state to another.
For example, you want to know when a button goes from OFF to ON.  This is called
state change detection, or edge detection.

This example shows how to detect when a button or button changes from off to on
and on to off.

The circuit:
* pushbutton attached to pin 2 from +5V
* 10K resistor attached to pin 2 from ground
* LED attached from pin 13 to ground (or use the built-in LED on
most Arduino boards)

created  27 Sep 2005
modified 30 Aug 2011
by Tom Igoe

This example code is in the public domain.

http://arduino.cc/en/Tutorial/ButtonStateChange

*/

// this constant won't change:
const int  buttonPin1 = 2;    // the pin that the pushbutton is attached to
const int  buttonPin2 = 3;    // the pin that the pushbutton is attached to


// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState1 = 0;         // current state of the button
int lastButtonState1 = 0;     // previous state of the button
int buttonState2 = 0;         // current state of the button
int lastButtonState2 = 0;     // previous state of the button

long lastDebounceTime1 = 0;  // the last time the output pin was toggled
long lastDebounceTime2 = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {

  int reading = digitalRead(buttonPin1);

   // check to see if you just pressed the button
   // (i.e. the input went from LOW to HIGH),  and you've waited
   // long enough since the last press to ignore any noise:

   // If the switch changed, due to noise or pressing:
   if (reading != lastButtonState1) {
     // reset the debouncing timer
     lastDebounceTime1 = millis();
   }

   if ((millis() - lastDebounceTime1) > debounceDelay) {
     // whatever the reading is at, it's been there for longer
     // than the debounce delay, so take it as the actual current state:

     // if the button state has changed:
     if (reading != buttonState1) {
       buttonState1 = reading;

       if (buttonState1 == HIGH) {
           // if the current state is HIGH then the button
           // wend from off to on:
           buttonPushCounter++;
           Serial.println("on");
           Serial.print("count:  ");
           Serial.println(buttonPushCounter);
         }
         else {
           // if the current state is LOW then the button
           // wend from on to off:
           Serial.println("off");
         }
     }
   }

   // save the reading.  Next time through the loop,
   // it'll be the lastButtonState:
   lastButtonState1 = reading;

  reading = digitalRead(buttonPin2);

   // check to see if you just pressed the button
   // (i.e. the input went from LOW to HIGH),  and you've waited
   // long enough since the last press to ignore any noise:

   // If the switch changed, due to noise or pressing:
   if (reading != lastButtonState2) {
     // reset the debouncing timer
     lastDebounceTime2 = millis();
   }

   if ((millis() - lastDebounceTime2) > debounceDelay) {
     // whatever the reading is at, it's been there for longer
     // than the debounce delay, so take it as the actual current state:

     // if the button state has changed:
     if (reading != buttonState2) {
       buttonState2 = reading;

       if (buttonState2 == HIGH) {
           // if the current state is HIGH then the button
           // wend from off to on:
           buttonPushCounter--;
           Serial.println("on");
           Serial.print("count:  ");
           Serial.println(buttonPushCounter);
         }
         else {
           // if the current state is LOW then the button
           // wend from on to off:
           Serial.println("off");
         }
     }
   }

   // save the reading.  Next time through the loop,
   // it'll be the lastButtonState:
   lastButtonState2 = reading;

}
