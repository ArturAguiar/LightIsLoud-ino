#include <fix_fft.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <specrend.h>

extern double bbTemp;

#define PIN 6
#define MINBAND 1

const int SAMPLE_WINDOW = 30; // Sample window width in mS (50 mS = 20Hz)
const int NUMBER_OF_LEDS = 60;
const double LOG2INV = 1/log(2);

static char im[128];
static char data[128];

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(10);
  strip.show(); // all pixels start off
}

void loop() {

  static unsigned int peakToPeak;  // peak-to-peak level
  static int sample;
  static int i;

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  for (i = 0; i < 128; ++i) { // toss out spurious readings

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
      --i;
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  //setAllPixelsToColor(getColourFromTemp(freqPeakToTemp()));
  //setAllPixelsToColor(getColourFromTemp(ampToTemp(peakToPeak)));
  setToSoundLevel(peakToPeak, getColourFromTemp(freqPeakToTemp()));
  //setToSoundLevel(peakToPeak, getColourFromTemp(ampToTemp(peakToPeak)));
  //strip.setBrightness(map(constrain(peakToPeak, 0, 1023), 0, 1023, 0, 255));
  strip.show();
}

void setToSoundLevel(unsigned int peakToPeak, uint32_t color) {

  static int lastPeak = 0;
  int displayPeak = map(constrain(peakToPeak, 0, 255), 0, 255, 0, NUMBER_OF_LEDS);
  if (displayPeak <= lastPeak) {
    if (lastPeak >= displayPeak + 3){
      displayPeak = lastPeak - 3;
    }
  }
  displayPeak = constrain(displayPeak, 0, 255);
  lastPeak = displayPeak;

  for(int i = 0; i < NUMBER_OF_LEDS; i++) {
    if (i >= displayPeak) {
      strip.setPixelColor(NUMBER_OF_LEDS - i - 1, 0);
    }
    else {
      strip.setPixelColor(NUMBER_OF_LEDS - i - 1, color);
    }
  }
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
  for (i = MINBAND; i < 64; i++) {
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

  return strip.Color(r*255, g*255, b*255);

}

void setAllPixelsToColor(uint32_t color) {

  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
}
