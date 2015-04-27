#ifndef STRIP_H
#define STRIP_H

#include <Adafruit_NeoPixel.h>

#define BLACK 0x000000
#define WHITE 0xFFFFFF

class Strip{
private:
  Adafruit_NeoPixel* strip;
  int length;
  bool split, invert;
public:
  Strip(uint16_t length, uint8_t pin, bool split, bool invert);
  void setAllPixelsToColor(uint32_t color);
  void setToSoundLevel(unsigned int peakToPeak, uint32_t color);
  void runTheCourse(unsigned int peakToPeak, uint32_t color);
  void show(){strip->show();}
  void setPixelColor(uint16_t i, uint32_t colour);
};

#endif
