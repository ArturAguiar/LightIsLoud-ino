#ifndef STRIP_H
#define STRIP_H

#include <Adafruit_NeoPixel.h>

#define BLACK 0x000000
#define WHITE 0xFFFFFF

class Strip{
private:
  Adafruit_NeoPixel* strip;
  uint16_t length;
  bool split;
  void show(){strip->show();}
  void setPixelColor(uint16_t i, uint32_t color, bool invert);
  uint32_t getPixelColor(uint16_t i, bool invert);
public:
  Strip(uint16_t length, uint8_t pin, bool split);
  void setAllPixelsToColor(uint32_t color);
  void setToSoundLevel(unsigned int peakToPeak, uint32_t color, bool invert);
  uint32_t runTheCourse(unsigned int peakToPeak, uint32_t color, bool invert);
  void buildUp(unsigned int peakToPeak, bool resetBrightness);
  void clear(){strip->clear(); show();}

};

#endif
