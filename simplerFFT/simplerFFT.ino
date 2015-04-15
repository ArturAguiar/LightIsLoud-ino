#include <fix_fft.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6

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

char im[128];
char data[128]; 

void setup()
{
  //Serial.begin(9600);
  strip.begin();
  strip.show(); // all pixels start off
}

void loop()
{
 int static i = 0;
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
       for (i = 0; i < 64; i++)
       {
          data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); 
          //Serial.print((int8_t)data[i]);
          //Serial.print(" ");
       }
       //Serial.println("]");
       
       //do something with the data values 1..64 and ignore im
     }

     tt = millis();
  }
}
