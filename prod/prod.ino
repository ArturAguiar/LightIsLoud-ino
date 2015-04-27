#include <fix_fft.h>
#include <Adafruit_NeoPixel.h>
#include <specrend.h>
#include "Strip.h"

extern double bbTemp;

#define MINBAND 1

const int SAMPLE_WINDOW = 30; // Sample window width in mS (50 mS = 20Hz)
const double LOG2INV = 1/log(2);

static char im[128];
static char data[128];

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
        data[i] = sample / 4 - 128;
        im[i] = 0;
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



double ampToTemp(unsigned int peakToPeak) {

  return map(constrain(peakToPeak, 0, 255), 0, 255, 1000, 15000);
}

double freqPeakToTemp() {
  //this could be done with the fix_fftr function without the im array.
  fix_fft(data, im, 7, 0);

  // I am only interessted in the absolute value of the transformation
  uint32_t num = 0;
  uint32_t den = 0;
  int i;
  for (i = MINBAND; i < 64; ++i) {
    data[i] = log(sqrt(data[i] * data[i] + im[i] * im[i]))*LOG2INV;
    num += data[i]*i;
    den += data[i];
  }
  num /= den;

  //do something with the data values 1..64
  return map(constrain(num, MINBAND, 64), MINBAND, 64, 1000, 15000);
}

uint32_t getColourFromTemp(double temp) {

  static int lastTemp = 0;
  if (temp <= lastTemp) {
    if (lastTemp >= temp + 500) {
      temp = lastTemp - 500;
    }
  }
  else{
    if (temp >= lastTemp + 100) {
      temp = lastTemp + 100;
    }
  }
  temp = constrain(temp, 1000, 15000);
  lastTemp = temp;

  double x, y, z, r, g, b;
  static struct colourSystem *cs = &SMPTEsystem;
  bbTemp = temp;

  spectrum_to_xyz(bb_spectrum, &x, &y, &z);
  xyz_to_rgb(cs, x, y, z, &r, &g, &b);
  constrain_rgb(&r, &g, &b);
  norm_rgb(&r, &g, &b);

  return ((uint32_t)((uint8_t)(r*255)) << 16) | ((uint32_t)((uint8_t)(g*255)) <<  8) | ((uint8_t)(b*255));

}
