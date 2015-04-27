#include <Adafruit_NeoPixel.h>
#include <fix_fft.h>
#include <specrend.h>
#include "Strip.h"
#include "SoundTemp.h"

const int SAMPLE_WINDOW = 30; // Sample window width in mS (50 mS = 20Hz)

Strip left_leg = Strip(90, 4, false, false);
Strip right_leg = Strip(90, 5, false, false);

Strip left_torso = Strip(60, 6, true, false);
Strip right_torso = Strip(60, 8, true, false);

Strip left_arm = Strip(60, 7, false, true);
Strip right_arm = Strip(60, 9, false, true);

Strip strips[] = {left_leg, right_leg, left_torso, right_torso, left_arm, right_arm};

void setup(){}

void loop() {

  static unsigned int peakToPeak;  // peak-to-peak level
  static int sample;
  static int i = 0;
  static int frame = 0;
  static uint32_t colour = WHITE;
  unsigned long startMillis = millis();  // Start of sample window

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

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
      if (frame++%4==3){
        colour = getColourFromTemp(freqPeakToTemp());
        frame = 0;
      }
      i = 0;
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  for (int j = 0; j < 6; ++j){
    strips[j].setToSoundLevel(peakToPeak, colour);
    //strips[j].runTheCourse(peakToPeak, colour);
    strips[j].show();
  }

  // left_torso.runTheCourse(peakToPeak, colour);
  // left_torso.show();

  //setAllPixelsToColor(getColourFromTemp(freqPeakToTemp()));
  //setAllPixelsToColor(getColourFromTemp(ampToTemp(peakToPeak)));
  //setToSoundLevel(peakToPeak, getColourFromTemp(freqPeakToTemp()));
  //setToSoundLevel(peakToPeak, getColourFromTemp(ampToTemp(peakToPeak)));
  //strip.setBrightness(map(constrain(peakToPeak, 0, 1023), 0, 1023, 0, 255));
  //strip.show();
}
