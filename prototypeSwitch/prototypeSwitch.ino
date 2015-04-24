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
const int  buttonPin2 = 2;    // the pin that the pushbutton is attached to


// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState1 = 0;         // current state of the button
int lastButtonState1 = 0;     // previous state of the button
int buttonState2 = 0;         // current state of the button
int lastButtonState2 = 0;     // previous state of the button

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {
  // read the pushbutton input pin:
  buttonState1 = digitalRead(buttonPin1);

  // compare the buttonState to its previous state
  if (buttonState1 != lastButtonState1) {
    // if the state has changed, increment the counter
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
  else {
    buttonState2 = digitalRead(buttonPin2);

    // compare the buttonState to its previous state
    if (buttonState2 != lastButtonState2) {
      // if the state has changed, increment the counter
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
    // save the current state as the last state,
    //for next time through the loop
    lastButtonState2 = buttonState2;
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState1 = buttonState1;


}
