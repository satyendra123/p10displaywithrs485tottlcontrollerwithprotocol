/*
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
  // Check if there are bytes available to read from RS485
  if (Serial.available() >= sizeof(CommunicationData)) {
    // Read the bytes into the struct
    CommunicationData receivedData;
    Serial.readBytes((char*)&receivedData, sizeof(CommunicationData));

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

        // Check if the received data is "123"
        if (strcmp(receivedChars, "123") == 0) {
          // Show "123" on the display
          show_text("123");
        }
      }
    }

    // Send back the received data
    Serial.write((byte*)&receivedData, sizeof(CommunicationData));
  }

  // Continuously read from serial and display the received text
  if (Serial.available() > 0) {
    String incomingString = Serial.readStringUntil('\n');
    show_text(incomingString.c_str());
    delay(2000); // Display the text for 2 seconds
    dmd.clearScreen(true); // Clear the screen after displaying
  }
}

void show_text(const char* text) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, strlen(text), GRAPHICS_NORMAL);
}
*/
/*
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

//EXAMPLE-3 display data with arrow sign and with the protocol
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "Arial_Black_16.h" // Note the corrected font header

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

// Arrow sign byte arrays
byte const arrow1[] PROGMEM = {
  32, 16,
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000001, B00000000, B00000000,
  B00000000, B00000011, B00000000, B00000000,
  B00000000, B00000111, B00000000, B00000000,
  B00000000, B00001111, B00000000, B00000000,
  B00000000, B00011111, B11111111, B11100000,
  B00000000, B00111111, B11111111, B11100000,
  B00000000, B01111111, B11111111, B11100000,
  B00000000, B01111111, B11111111, B11100000,
  B00000000, B00111111, B00000000, B00000000,
  B00000000, B00011111, B00000000, B00000000,
  B00000000, B00001111, B00000000, B00000000,
  B00000000, B00000111, B00000000, B00000000,
  B00000000, B00000011, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000
};

byte const arrow2[] PROGMEM = {
  32, 16,
  // Define the bitmap data for the second arrow sign here
};

byte const arrow3[] PROGMEM = {
  32, 16,
  // Define the bitmap data for the third arrow sign here
};

const byte* frames[] = {
    arrow1,
    arrow2,
    arrow3,
};
#define NUM_FRAMES  (sizeof(frames) / sizeof(frames[0]))
unsigned int frame = 0;

#define ADVANCE_MS  (1000 / NUM_FRAMES)

// Function prototypes
void ScanDMD();
void show_text(const char* text);

void setup() {
  Serial.begin(115200);
  Serial.println("Start...");

  // Initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
  Timer1.initialize(3000);  // period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt(ScanDMD);  // attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  pinMode(ledPin, OUTPUT);

  // Initialize DMD3
  dmd.begin();
}

void loop() {
  // Static byte array representing the received data: AA 01 17 03 31 32 33 8C
  byte testData[] = {0xAA, 0x01, 0x17, 0x03, 0x33, 0x32, 0x33, 0x8C};
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

        // Display the arrow sign
        dmd.drawStaticBitmap(frames[frame], 0, 0, 32, 16); // Change the index to select the desired arrow sign
        frame = (frame + 1) % NUM_FRAMES; // Move to the next frame
      }
    }
  }

  // Delay before clearing the display
  delay(2000);
  dmd.clearScreen(true); // Clear the screen after displaying
}

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

void show_text(const char* text) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(16, 0, text, strlen(text), GRAPHICS_NORMAL);
}
