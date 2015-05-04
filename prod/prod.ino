#include <Adafruit_NeoPixel.h>
#include <Button.h>
#include <fix_fft.h>
#include <specrend.h>
#include "Strip.h"
#include "SoundTemp.h"

const int SAMPLE_WINDOW = 50; // Sample window width in mS (50 mS = 20Hz)

static Button buttonUp = Button(2);
static Button buttonDown = Button(3);

static Strip legs = Strip(90, 6, false);
static Strip torso = Strip(60, 5, true);
static Strip arms = Strip(60, 4, false);

static Strip strips[] = {legs, torso, arms};

void setup(){}

void loop() {

  static unsigned int peakToPeak;  // peak-to-peak level
  static int sample;
  static int i = 0;
  static int frame = 0;
  static uint32_t color = WHITE;
  static int mode = 0;

  unsigned long startMillis = millis();  // Start of sample window

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  if(buttonUp.uniquePress()){    
    if (mode == 1){
      for (int j = 0; j < 3; ++j){
        strips[j].buildUp(0, true);
      }
    }
    ++mode;
    mode %= 6;
  }
  else if(buttonDown.uniquePress()){
    if (mode == 1){
      for (int j = 0; j < 3; ++j){
        strips[j].buildUp(0, true);
      }
    }
    mode += 5;
    mode %= 6;
  }

  while(millis() - startMillis < SAMPLE_WINDOW){
    if (i < 128){

      sample = analogRead(0);

      if (sample < 1024) {
        if (sample > signalMax) {
          signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin) {
          signalMin = sample;  // save just the min levels
        }
        setSoundTempData(i, sample / 4 - 128, 0);
      }
      else {
        continue;
      }
      ++i;
    }
    else {
      switch (mode){
        case 3:
        if (frame++%64==63){
          color = getColourFromTemp(freqPeakToTemp());
          frame = 0;

        }
        break;
        case 5:
        color = getColourFromTemp(freqPeakToTemp());
        break;
      }
      i = 0;
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  switch (mode){
    case 1:
    for (int j = 0; j < 3; ++j){
      strips[j].buildUp(peakToPeak, false);
    }
    break;
    case 3:
    arms.runTheCourse(peakToPeak, color, false);
    legs.runTheCourse(torso.runTheCourse(peakToPeak, color, true)==BLACK ? 0 : 255, color, false);
    break;
    case 5:
    for (int j = 0; j < 3; ++j){
      strips[j].setToSoundLevel(peakToPeak, color, j == 2);
    }
    break;
    default:
    for (int j = 0; j < 3; ++j){
      strips[j].clear();
    }
    break;
  }
}
