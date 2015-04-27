#include "Strip.h"

Strip::Strip(uint16_t length, uint8_t pin, bool split, bool invert){
  strip = new Adafruit_NeoPixel(length, pin);
  this->length = length/(split+1);
  this->split = split;
  this->invert = invert;
  strip->begin();
  strip->setBrightness(10);
}


void Strip::setAllPixelsToColor(uint32_t color) {

  for (int i = 0; i < length; i++) {
    setPixelColor(i, color);
  }
}

void Strip::setToSoundLevel(unsigned int peakToPeak, uint32_t color) {

  static int lastPeak = 0;
  int displayPeak = map(constrain(peakToPeak, 0, 255), 0, 255, 0, length);
  if (displayPeak <= lastPeak) {
    if (lastPeak >= displayPeak + 3){
      displayPeak = lastPeak - 3;
    }
  }
  displayPeak = constrain(displayPeak, 0, 255);
  lastPeak = displayPeak;

  for(int i = 0; i <= length; i++) {
    if (i >= displayPeak) {
      setPixelColor(i, 0);
    }
    else {
      setPixelColor(i, color);
    }
  }
}

void Strip::runTheCourse(unsigned int peakToPeak, uint32_t color)
{
  int displayPeak = map(constrain(peakToPeak * 4, 0, 1023), 0, 1023, 0, 100);

  if (displayPeak > 20)  {
    setPixelColor(0, color);
    show();
  }

  for(int16_t i = length+1; i >= 0; i--)  {
    uint32_t thisColor = strip->getPixelColor(i);

    if (thisColor != BLACK)
    {
      //Serial.print("COLOR AT ");
      //Serial.println(thisColor);

      setPixelColor(i, BLACK);

      if (i + 1 < length)
      {
        setPixelColor(i + 1, color);
      }
    }
  }
}

void Strip::setPixelColor(uint16_t i, uint32_t colour){
  if(invert){
    i = length-i-1;
  }
  if (split){
    strip->setPixelColor(i, colour);
    strip->setPixelColor(2*length-i, colour);

  }
  else {
    strip->setPixelColor(i, colour);
  }
}
