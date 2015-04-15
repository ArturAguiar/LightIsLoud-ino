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
