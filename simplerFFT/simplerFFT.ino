#include <fix_fft.h>

char im[128];
char data[128];

void setup()
{
  //Serial.begin(9600);
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
       uint16_t red = 0;
       uint16_t green = 0;
       uint16_t blue = 0;
       for (i = 0; i < 60; i++)
       {
          data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);
          if (i < 20){
            red += data[i];
          }
          else if(i < 40){
            green += data[i];
          }
          else {
            blue += data[i];
          }
          //Serial.print((int8_t)data[i]);
          //Serial.print(" ");
       }
       red /= 2;
       green /= 2;
       blue /= 2;
       uint32_t color = strip.Color(red, green, blue)
       //Serial.println("]");

       //do something with the data values 1..64 and ignore im
     }

     tt = millis();
  }
}
