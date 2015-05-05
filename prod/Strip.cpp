#include "Strip.h"
#include "SoundTemp.h"

Strip::Strip(uint16_t length, uint8_t pin, bool split){
  strip = new Adafruit_NeoPixel(length, pin);
  this->length = length/(split+1);
  this->split = split;
  strip->begin();
  strip->setBrightness(10);
}


void Strip::setAllPixelsToColor(uint32_t color) {

  for (int i = 0; i < length; i++) {
    setPixelColor(i, color, false);
  }
  show();
}

void Strip::setToSoundLevel(unsigned int peakToPeak, uint32_t color, bool invert) {

  static int lastPeak = 0;
  int displayPeak = map(constrain(peakToPeak, 0, 255), 0, 255, 0, length);
  if (displayPeak <= lastPeak) {
    if (lastPeak >= displayPeak + 3){
      displayPeak = lastPeak - 3;
    }
  }
  displayPeak = constrain(displayPeak, 0, length);
  lastPeak = displayPeak;

  for(int i = 0; i < length; ++i) {
    if (i >= displayPeak) {
      setPixelColor(i, BLACK, invert);
    }
    else {
      setPixelColor(i, color, invert);
    }
  }
  show();
}

uint32_t Strip::runTheCourse(unsigned int peakToPeak, uint32_t color, bool invert){
  int displayPeak = map(constrain(peakToPeak, 0, 255), 0, 255, 0, 100);

  if (displayPeak > 20) {
    setPixelColor(0, color, invert);
    show();
  }

  uint32_t toReturn = getPixelColor(length-1, invert);

  for(int16_t i = length-1; i >= 0; --i)  {
    uint32_t thisColor = getPixelColor(i, invert);

    if (thisColor != BLACK) {
      setPixelColor(i, BLACK, invert);

      if (i + 1 < length) {
        setPixelColor(i + 1, color, invert);
      }
    }
  }
  show();
  return toReturn;
}

float Strip::buildUp(unsigned int peakToPeak, uint32_t color, bool resetBrightness) {
  static float buildUpBrightness = 1.0f;

  if (resetBrightness) {
    buildUpBrightness = 1.0f;
    strip->setBrightness(1);
    //setAllPixelsToColor(WHITE);
    show();
    strip->setBrightness(10);
    return 0;
  }

  if (buildUpBrightness < 20) {
    int displayPeak = map(constrain(peakToPeak, 0, 255), 0, 255, 0, 100);

    if (displayPeak > 20) {
      buildUpBrightness += 0.1f;
    }
    setAllPixelsToColor(color);
    strip->setBrightness((int)buildUpBrightness);

    show();

    strip->setBrightness(10);
  }
  return buildUpBrightness;
}

void Strip::setPixelColor(uint16_t i, uint32_t color, bool invert){

  if(invert) {
    i = length-i-1;
  }

  if (split) {
    strip->setPixelColor(i, color);
    strip->setPixelColor(2*length-i-1, color);
  }
  else {
    strip->setPixelColor(i, color);
  }
}

uint32_t Strip::getPixelColor(uint16_t i, bool invert){

  if(invert) {
    i = length-i-1;
  }

  return strip->getPixelColor(i);
}
