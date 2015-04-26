//include the class definition
#include "Button.h"

/*
|| <<constructor>>
|| @parameter buttonPin sets the pin that this switch is connected to
|| @parameter buttonMode indicates PULLUP or PULLDOWN resistor
*/
Button::Button(uint8_t buttonPin){
	this->pin=buttonPin;
	pinMode(pin, INPUT_PULLUP);
}

/*
|| Return true if the button is pressed, and was not pressed before
*/
bool Button::uniquePress(void){

	bool toReturn = false;

	uint8_t reading = digitalRead(pin);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH),  and you've waited
	// long enough since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (reading != lastState) {
		// reset the debouncing timer
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay) {
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != state) {
			state = reading;

			if (state == HIGH) {
				toReturn = true;
			}
		}
	}

	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	lastState = reading;

	return toReturn;
}
