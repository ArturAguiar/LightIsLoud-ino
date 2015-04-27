#include <fix_fft.h>
#include <specrend.h>
#include "SoundTemp.h"

#define MINBAND 1

const double LOG2INV = 1/log(2);

extern double bbTemp;

static char im[128];
static char data[128];

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

void setSoundTempData(int i, char dataV, char imV){
  data[i] = dataV;
  im[i] = imV;
}
