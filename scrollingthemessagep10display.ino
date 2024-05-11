#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#include "Arial14.h"
 
char b[8];
String str;
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
 
void ScanDMD(){
dmd.scanDisplayBySPI();
}
 
void setup() {
Serial.begin(9600);
Timer1.initialize(5000);
Timer1.attachInterrupt(ScanDMD);
dmd.clearScreen(true);
}
 
void loop()
{
  int slen=0;
  dmd.selectFont(Arial_Black_16);
  str = "HOUSTON SYSTEM";
  slen = str.length()+1;
  str.toCharArray(b, slen );
  dmd.drawMarquee( b, slen, (32*DISPLAYS_ACROSS)-1 ,1);
  unsigned long timing  = millis();
  boolean flag = false;
  while (!flag)
  {
 if ((timing + 70) < millis())
 {
    flag = dmd.stepMarquee(-1, 0);
    timing = millis();
 }
  }
}
