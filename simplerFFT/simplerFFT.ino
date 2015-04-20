#include <fix_fft.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6
#define MINBAND 1

const int SAMPLE_WINDOW = 30; // Sample window width in mS (50 mS = 20Hz)
const int NUMBER_OF_LEDS = 60;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, PIN, NEO_GRB + NEO_KHZ800);

const uint32_t BLACK = strip.Color(0, 0, 0);
const double LOG2INV = 1/log(2);

char im[128];
char data[128];
uint8_t min_data[64];


void setup()
{
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(60);
  strip.show(); // all pixels start off
  int i = 0;
  int j = 0;
  int val;

  for (; j < 50; ++j) {
    for(; i < 129; ++i) {
      if (i < 128) {
        val = analogRead(0);
        //Serial.println(val);
        data[i] = val / 4 - 128;
        im[i] = 0;
        i++;

      }
      else {
        fix_fft(data, im, 7, 0);

        for (i = MINBAND; i < 64; i++) {
          data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);
          min_data[i] += data[i];
        }
      }
    }
  }
  for (i = MINBAND; i < 64; i++) {
    min_data[i] /= 50;
  }

}

void loop()
{
  static int i = 0;
  static long tt;
  int val;

  if (millis() > tt)
  {
    if (i < 128)
    {
      val = analogRead(0);
      //Serial.println(val);
      data[i] = val / 4 - 128;
      im[i] = 0;
      i++;

    }
    else
    {
      //this could be done with the fix_fftr function without the im array.
      fix_fft(data, im, 7, 0);

      //Serial.print("[ ");
      // I am only interessted in the absolute value of the transformation
      int16_t red = 0;
      int16_t green = 0;
      int16_t blue = 0;
      int32_t sum = 0;
      for (i = MINBAND; i < 64; i++)
      {
        data[i] = 5*floor(log(sqrt(data[i] * data[i] + im[i] * im[i]) - min_data[i])*LOG2INV);
        //data[i] = sqrt(data[i] * data[i] + im[i] * im[i]) - min_data[i];

        if (i < 20){
          red += data[i];
        }
        else if (i < 40){
          green += data[i];
        }
        else if (i < 60){
          blue += data[i];
        }
        sum += data[i];
        Serial.print(i);
        Serial.print(":");
        Serial.println((uint8_t)data[i]);

      }

      uint32_t color = strip.Color(red, green, blue);

      //do something with the data values 1..64 and ignore im
      setAllPixelsToColor(color);
      //strip.setBrightness(brightness);
      strip.show();
       //i = 0;
    }

    tt = millis();
  }
}

void setAllPixelsToColor(uint32_t color)
{
  for (int i = 0; i < NUMBER_OF_LEDS; i++)
  {
    strip.setPixelColor(i, color);
  }
}
