/*
#include <SPI.h>       
#include <DMD.h>       // use spi bus 
#include <TimerOne.h>
#include "Arial_black_16.h"


#define DISPLAYS_ACROSS 1   // no. of P1 panel in horizontal
#define DISPLAYS_DOWN   1   // no. of P1 panel in vertical
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

void ScanDMD()
{ 
    dmd.scanDisplayBySPI();
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Start...");
 
  
   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 3000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  
}

 

//==========================================================
void show_text()
{
    dmd.clearScreen( true );
    dmd.selectFont(Arial_Black_16);
    dmd.drawString( 0,  0, "ABC", 3, GRAPHICS_NORMAL );
}

//==========================================================
void scroll_demo()
{
     // chinese char
    dmd.clearScreen( true );
    dmd.selectFont(Arial_Black_16);

     
    dmd.drawMarquee("Demo",4,(32*DISPLAYS_ACROSS)-1,0);
    long start = millis();
    long timer = start;
    boolean ret=false;
    while(!ret)
    {
         if ((timer+100) < millis()) 
         {
            ret = dmd.stepMarquee(-1,0);
            timer = millis();
        }
    }
    delay(500);
     
}

 
void loop(void)
{ 
    show_text(); delay(2000);
    scroll_demo();  delay(2000);
}
*/
//Example-2 display the data from serially. send anydata and it will display. this is working fine. when i send the data from the hercules then it display on the screen
#include <SPI.h>       
#include <DMD.h>       // use SPI bus 
#include <TimerOne.h>
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 1   // Number of P1 panels in horizontal
#define DISPLAYS_DOWN   1   // Number of P1 panels in vertical
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

const int ledPin = 13;

void ScanDMD() { 
  dmd.scanDisplayBySPI();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start...");

  // Initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
  Timer1.initialize(3000);  // period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt(ScanDMD);  // attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming bytes
    String incomingString = Serial.readStringUntil('\n');
    // Display the received text
    show_text(incomingString.c_str()); // Convert String to const char*
    delay(2000); // Display the text for 2 seconds
    //dmd.clearScreen(true); // Clear the screen after displaying
    incomingString="";
  }
}

void show_text(const char* text) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, strlen(text), GRAPHICS_NORMAL);
}
