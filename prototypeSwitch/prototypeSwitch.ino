#include <Button.h>

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {

  static Button button1 = Button(2);
  static Button button2 = Button(3);
  static int buttonPushCounter = 0;

  if(button1.uniquePress()){
    buttonPushCounter++;
    Serial.print("count:  ");
    Serial.println(buttonPushCounter);
  }

  if(button2.uniquePress()){
    buttonPushCounter--;
    Serial.print("count:  ");
    Serial.println(buttonPushCounter);
  }
  
}
