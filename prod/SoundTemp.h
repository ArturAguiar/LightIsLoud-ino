#ifndef SOUNDTEMP_H
#define SOUNDTEMP_H

#include <Arduino.h>

double ampToTemp(unsigned int peakToPeak);

double freqPeakToTemp();

uint32_t getColourFromTemp(double temp);

void setSoundTempData(int i, char dataV, char imV);

#endif
