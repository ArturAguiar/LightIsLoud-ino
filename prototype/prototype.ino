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
unsigned int sample;

uint16_t stackRemaining = NUMBER_OF_LEDS;


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.setBrightness(60);
  strip.show(); // Initialize all pixels to 'off'

  //Serial.begin(9600);
}

void loop()
{
  unsigned long startMillis = millis();  // Start of sample window
  unsigned int peakToPeak = 0;  // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < SAMPLE_WINDOW)
  {
    sample = analogRead(0);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  //double volts = (peakToPeak * 3.3) / 1024;  // convert to volts

  // multiply peakToPeak by something > 1 to make it more sensitive to volume.
  //int displayPeak = map(peakToPeak * 16, 0, 1023, 0, NUMBER_OF_LEDS);
  //int displayPeak = map(constrain(peakToPeak * 2, 0, 1023), 0, 1023, 0, NUMBER_OF_LEDS);

  //int displayPeak = map(constrain(peakToPeak * 4, 0, 1023), 0, 1023, 0, 100);

  //runTheCourse(peakToPeak, strip.Color(34, 150, 72));

  //runTheCourseStack(strip.Color(34, 150, 72));

  showSoundLevel(peakToPeak, strip.Color(34, 150, 72));
  //amplitudeToBrightness(peakToPeak, strip.Color(34, 150, 72));
  //showSoundLevel(peakToPeak, strip.Color(34, 150, 72));
  strip.show();
}

void showSoundLevel(unsigned int peakToPeak, uint32_t color)
{
  int displayPeak = map(constrain(peakToPeak * 4, 0, 1023), 0, 1023, 0, NUMBER_OF_LEDS);

  for(int i = 0; i < NUMBER_OF_LEDS; i++)
  {
    if (i >= displayPeak)
    {
      strip.setPixelColor(NUMBER_OF_LEDS - i - 1, 0);
    }
    else
    {
      strip.setPixelColor(NUMBER_OF_LEDS - i - 1, color);
    }
  }

  strip.show();              // Refresh LED states
}

void runTheCourse(unsigned int peakToPeak, uint32_t color)
{
  int displayPeak = map(constrain(peakToPeak * 4, 0, 1023), 0, 1023, 0, 100);

  if (displayPeak > 20)
  {
    strip.setPixelColor(0, color);
    strip.show();
  }

  for(int16_t i = NUMBER_OF_LEDS; i >= 0; i--)
  {
    uint32_t thisColor = strip.getPixelColor(i);

    if (thisColor != BLACK)
    {
      //Serial.print("COLOR AT ");
      //Serial.println(thisColor);

      strip.setPixelColor(i, BLACK);

      if (i + 1 < NUMBER_OF_LEDS)
      {
        strip.setPixelColor(i + 1, color);
      }
    }
  }
}

void runTheCourseStack(uint32_t color)
{
  for(uint16_t i = 0; i < stackRemaining; i++)
  {
    if (i > 0)
    {
     strip.setPixelColor(i - 1, BLACK);
    }
    strip.setPixelColor(i, color);
    strip.show();
    delay(20);
  }

  if (stackRemaining == 0)
  {
    stackRemaining = NUMBER_OF_LEDS;
  }
  else
  {
    stackRemaining--;
  }
}

void amplitudeToBrightness(unsigned int peakToPeak, uint32_t color)
{
  if (peakToPeak < 50)
  {
    strip.clear();
    return;
  }

  int displayPeak = map(constrain(peakToPeak, 0, 1023), 0, 1023, 0, 255);

  //uint32_t brightnessAdjustedColor = strip.Color(map(color.red, 0, 255, 0, displayPeak),
  //                                               map(color.green, 0, 255, 0, displayPeak),
  //                                               map(color.blue, 0, 255, 0, displayPeak));

  for(int i = 0; i < NUMBER_OF_LEDS; i++)
  {
    strip.setPixelColor(i, color);
    strip.setBrightness(displayPeak);
  }
}
