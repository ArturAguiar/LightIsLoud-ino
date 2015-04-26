/*
||
|| @file Button.h
|| @version 1.6
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making buttons
|| #
||
|| @license
|| | Copyright (c) 2009 Alexander Brevig. All rights reserved.
|| | This code is subject to AlphaLicence.txt
|| | alphabeta.alexanderbrevig.com/AlphaLicense.txt
|| #
||
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

#define debounceDelay 50

class Button{
public:
  Button(uint8_t buttonPin);
  bool uniquePress();
private:
  uint8_t pin;
  uint8_t state;
  uint8_t lastState;
  long lastDebounceTime;
};

#endif
