/*
//EXAMPLE-1 this code works only for when i send the data through serial monitor to arduino. if i will 
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
  Timer1.initialize(3000);  // period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt(ScanDMD);  // attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String incomingString = Serial.readStringUntil('\n');
    show_text(incomingString.c_str());
    delay(2000);
    //dmd.clearScreen(true); // Clear the screen after displaying
    incomingString="";
  }
}

void show_text(const char* text) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, strlen(text), GRAPHICS_NORMAL);
}
*/

#include <SoftwareSerial.h>
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "Arial_black_16.h"

const byte TX_PIN = 2;
const byte RX_PIN = 3;
const byte LED_PIN = 13;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

#define DISPLAYS_ACROSS 1   // Number of P1 panels in horizontal
#define DISPLAYS_DOWN   1   // Number of P1 panels in vertical
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

String receivedString = ""; // Variable to store the received string

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  Soft_Serial.begin(9600);
  Serial.println("\n\nWelcome to JP Learning\n");
  Serial.println("Receiver Start\n");

  Serial.println("Start...");

  // Initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
  Timer1.initialize(3000);  // period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt(ScanDMD);  // attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  pinMode(LED_PIN, OUTPUT);
}

void loop() 
{
  if (Soft_Serial.available() > 0) 
  {
    receivedString = Soft_Serial.readStringUntil('\n');
    Serial.print(receivedString);
    show_text(receivedString.c_str()); // Convert String to const char*
    delay(2000); // Display the text for 2 seconds
    dmd.clearScreen(true); // Clear the screen after displaying
    receivedString = ""; // Variable to store the received string="";
  }
}

void show_text(const char* text) 
{
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, strlen(text), GRAPHICS_NORMAL);
}




/*
//EXAMPLE-2 extract the protocol and print the data into the p10 display
#include <SPI.h>       
#include <DMD.h>       // use SPI bus 
#include <TimerOne.h>
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 1   // Number of P1 panels in horizontal
#define DISPLAYS_DOWN   1   // Number of P1 panels in vertical
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

const int ledPin = 13;

// Define a struct for your data
struct CommunicationData {
  byte start;
  byte address;
  byte command;
  byte length;
  byte data[3]; // Array to store 3 bytes of data
  byte checksum;
};

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
  // Static byte array representing the received data: AA 01 17 03 31 32 33 8C
  byte testData[] = {0xAA, 0x01, 0x17, 0x03, 0x31, 0x32, 0x33, 0x8C};
  int testDataLength = sizeof(testData);

  // Check if the received data length matches the size of CommunicationData struct
  if (testDataLength == sizeof(CommunicationData)) {
    // Create a CommunicationData struct and copy the test data into it
    CommunicationData receivedData;
    memcpy(&receivedData, testData, sizeof(CommunicationData));

    // Verify if the received data is valid
    if (receivedData.start == 0xAA && receivedData.checksum == 0x8C && receivedData.length == 3) {
      // Verify if the received command is 0x17 and length is 3
      if (receivedData.command == 0x17 && receivedData.length == 3) {
        // Convert the received data to ASCII characters
        char receivedChars[4]; // Extra byte for null terminator
        for (int i = 0; i < 3; i++) {
          receivedChars[i] = receivedData.data[i];
        }
        receivedChars[3] = '\0'; // Null terminate the string

        // Show the received data on the display
        show_text(receivedChars);
      }
    }
  }

  // Delay before clearing the display
  delay(2000);
  dmd.clearScreen(true); // Clear the screen after displaying
}

void show_text(const char* text) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, strlen(text), GRAPHICS_NORMAL);
}
*/
