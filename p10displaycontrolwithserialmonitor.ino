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
/*
//EXAMPLE-2
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
*/



/*
//EXAMPLE-2 extract the protocol and print the data into the p10 display
#include <font/TimesNewRoman12.h> // Include a smaller font

const int ledPin = 13;

DMD3 display;

// Define the structure for communication data
struct CommunicationData {
  byte start;
  byte address;
  byte command;
  byte length;
  byte data[3];
  byte end;
};

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
   // arrow2,
    //arrow3,
};
#define NUM_FRAMES  (sizeof(frames) / sizeof(frames[0]))
unsigned int frame = 0;

#define ADVANCE_MS  (1000 / NUM_FRAMES)

void scan() {
    display.refresh();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start...");

  Timer1.initialize(2000);
  Timer1.attachInterrupt(scan);
  Timer1.pwm(9, 255);
}

void loop() {
  byte testData[] = {0xAA, 0x01, 0x17, 0x03, 0x33, 0x32, 0x33, 0x8C};
  int testDataLength = sizeof(testData);

  // Parse testData
  CommunicationData receivedData;
  for (int i = 0; i < sizeof(CommunicationData); i++) {
    ((byte*)&receivedData)[i] = testData[i];
  }

  if (receivedData.start == 0xAA && receivedData.end == 0x8C && receivedData.length == 3) {
    char receivedChars[4];
    for (int i = 0; i < 3; i++) {
      receivedChars[i] = receivedData.data[i];
    }
    receivedChars[3] = '\0';

    // Display data in the serial monitor
    Serial.print("Received Data: ");
    Serial.println(receivedChars);

    // Display arrow and text on the P10 display
    show_text(receivedChars, frames[frame]);
    delay(1000);
    display.refresh();

    frame = (frame + 1) % NUM_FRAMES;
  }

  delay(2000);
  display.clear();
}

void show_text(const char* text, const byte* arrow) {
  display.clear();
  display.setFont(TimesNewRoman12); // Use a smaller font
  display.drawText(10, 0, text); // Adjust the position for text
  display.refresh(); // Refresh the display after drawing text
  delay(2000);
  
  display.clear(); // Clear the display buffer before drawing the arrow
  display.drawBitmap(20, 5, arrow); // Draw arrow at the left side
  display.refresh(); // Refresh the display after drawing arrow
  delay(2000);
}

/*
#include <Arduino.h>
#include "ArrowDisplay.h"

// Create an instance of the ArrowDisplay class
ArrowDisplay arrowDisplay;

void setup() {
  // Initialize the ArrowDisplay
  arrowDisplay.init();
}

void loop() {
  // Show right arrows
  arrowDisplay.showRightArrows();
  delay(1000); // Wait for 1 second between each arrow

  // Show left arrows
  arrowDisplay.showLeftArrows();
  delay(1000); // Wait for 1 second between each arrow
}
*/
*/
/*
//EXAMPLE-3 this code works and extract the vacancy data and display the data in the serial monitor. with the protocol which i have made in esp32 for combi sensors
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 1   // Number of P1 panels in horizontal
#define DISPLAYS_DOWN   1   // Number of P1 panels in vertical
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

const int ledPin = 13;

// Define a struct for your data
struct CommunicationData {
  byte start;
  byte zone_id;
  byte total_sensor;
  byte sensor_status_1;
  byte sensor_status_2;
  byte total_engaged;
  byte total_disengaged;
  byte total_vacancy;
  byte total_error;
  byte end;
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
  // Static byte array representing the received data: AA 01 02 01 01 02 00 00 00 55
  byte testData[] = {0xAA, 0x01, 0x02, 0x01, 0x01, 0x02, 0x00, 0x05, 0x00, 0x55};
  int testDataLength = sizeof(testData);

  // Check if the received data length matches the size of CommunicationData struct
  if (testDataLength == sizeof(CommunicationData)) {
    // Create a CommunicationData struct and copy the test data into it
    CommunicationData receivedData;
    memcpy(&receivedData, testData, sizeof(CommunicationData));

    // Verify if the received data is valid
    if (receivedData.start == 0xAA && receivedData.end == 0x55) {
      // Convert the total vacancy byte to ASCII characters
      char vacancyChar = receivedData.total_vacancy + '0';

      // Show the total vacancy on the display
      show_text(&vacancyChar, 1);

      // Print the total vacancy to the serial monitor
      Serial.print("Vacancy: ");
      Serial.println(vacancyChar);
    }
  }

  // Delay before clearing the display
  delay(2000);
  dmd.clearScreen(true); // Clear the screen after displaying
}

void show_text(const char* text, int length) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, length, GRAPHICS_NORMAL);
}
*/

/*
//EXAMPLE-4 in this code i will send the protocol data by the docklight software. it will receive the data from the docklight and after that extract the vacancy data and prints it on the serial monitor. when i 
// send the hex data from the rs485tousb then it receives the data and prints on the serial monitor. i send the hex data from the docklight.
#include <SoftwareSerial.h>
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 1   // Number of P1 panels in horizontal
#define DISPLAYS_DOWN   1   // Number of P1 panels in vertical
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

const byte TX_PIN = 2;
const byte RX_PIN = 3;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

// Define a struct for your data
struct CommunicationData {
  byte start;
  byte zone_id;
  byte total_sensor;
  byte sensor_status_1;
  byte sensor_status_2;
  byte total_engaged;
  byte total_disengaged;
  byte total_vacancy;
  byte total_error;
  byte end;
};

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start...");

  Soft_Serial.begin(9600);
  Timer1.initialize(3000);
  Timer1.attachInterrupt(ScanDMD);
}

void loop() {
  // Check if data is available on the SoftwareSerial port
  if (Soft_Serial.available() >= sizeof(CommunicationData)) {
    byte buffer[sizeof(CommunicationData)];
    Soft_Serial.readBytes(buffer, sizeof(CommunicationData));

    CommunicationData receivedData;
    memcpy(&receivedData, buffer, sizeof(CommunicationData));

    if (receivedData.start == 0xAA && receivedData.end == 0x55) {
      // Convert the total vacancy byte to ASCII characters
      char vacancyChar = receivedData.total_vacancy + '0';
      show_text(&vacancyChar, 1);
      Serial.print("Vacancy: ");
      Serial.println(vacancyChar);
    }
  }
  delay(2000);
  dmd.clearScreen(true);
}

void show_text(const char* text, int length) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, length, GRAPHICS_NORMAL);
}
*/
/* EXAMPLE-3 i am sending the ascii characters a a 0 1 0 2 0 1 0 1 0 2 0 1 0 1 0 1 5 5 like this and also receiving the data in the arduino serial monitor
#include <SoftwareSerial.h>
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 1   // Number of P1 panels in horizontal
#define DISPLAYS_DOWN   1   // Number of P1 panels in vertical
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

const byte TX_PIN = 2;
const byte RX_PIN = 3;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

// Define a struct for your data
struct CommunicationData {
  byte start;
  byte zone_id;
  byte total_sensor;
  byte sensor_status_1;
  byte sensor_status_2;
  byte total_engaged;
  byte total_disengaged;
  byte total_vacancy;
  byte total_error;
  byte end;
};

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start...");

  Soft_Serial.begin(9600);
  Timer1.initialize(3000);
  Timer1.attachInterrupt(ScanDMD);
}

void loop() {
  if (Soft_Serial.available() > 0) {
    char buffer[50]; // Increase buffer size to accommodate larger messages
    int bytesRead = Soft_Serial.readBytesUntil('\n', buffer, sizeof(buffer));

    Serial.print("Received raw data: ");
    for (int i = 0; i < bytesRead; i++) {
      Serial.print(buffer[i]);
      Serial.print(" ");
    }
    Serial.println();

    byte hexData[sizeof(buffer)/2];
    for (int i = 0; i < bytesRead/2; i++) 
    {
      hexData[i] = strtol(&buffer[i * 2], NULL, 16);
    }

    if (hexData[0] == 0xAA && hexData[bytesRead/2 - 1] == 0x55) {
      // Convert the total vacancy byte to ASCII characters
      char vacancyChar = hexData[7] + '0';
      show_text(&vacancyChar, 1);
      Serial.print("Vacancy: ");
      Serial.println(vacancyChar);
    }
  }

  delay(2000);
  dmd.clearScreen(true);
}

void show_text(const char* text, int length) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, length, GRAPHICS_NORMAL);
}
*/
#include <SoftwareSerial.h>
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 1   // Number of P1 panels in horizontal
#define DISPLAYS_DOWN   1   // Number of P1 panels in vertical
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

const byte TX_PIN = 2;
const byte RX_PIN = 3;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

// Define a struct for your data
struct CommunicationData {
  byte start;
  byte zone_id;
  byte total_sensor;
  byte sensor_status_1;
  byte sensor_status_2;
  byte total_engaged;
  byte total_disengaged;
  byte total_vacancy;
  byte total_error;
  byte end;
};

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Start...");

  Soft_Serial.begin(9600);
  Timer1.initialize(3000);
  Timer1.attachInterrupt(ScanDMD);
}

void loop() {
  if (Soft_Serial.available() > 0) {
    char buffer[50]; // Increase buffer size to accommodate larger messages
    int bytesRead = Soft_Serial.readBytesUntil('\n', buffer, sizeof(buffer));

    Serial.print("Received raw data: ");
    Serial.println(buffer);

    // Check if the start and end bytes are correct
    if (buffer[0] == 'A' && buffer[1] == 'A' && buffer[bytesRead - 2] == '5' && buffer[bytesRead - 1] == '5') {
      // Extract the vacancy byte
// Extract the vacancy bytes (ASCII characters)
char vacancyCharHigh = buffer[14]; // Extract the high digit
char vacancyCharLow = buffer[15];  // Extract the low digit

// Convert ASCII characters to integers
int vacancyHigh = vacancyCharHigh - '0'; // Convert ASCII to integer
int vacancyLow = vacancyCharLow - '0';   // Convert ASCII to integer

// Calculate the actual vacancy value
int vacancy = vacancyHigh * 10 + vacancyLow;

// Print the vacancy to the serial monitor
Serial.print("Vacancy: ");
Serial.println(vacancy);


    }
  }

  delay(2000);
  dmd.clearScreen(true);
}


void show_text(const char* text, int length) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, length, GRAPHICS_NORMAL);
}


/*
// EXAMPLE-6 in this i am sending the ascii data in bytes likes b'\xaa\x01\x03\x02\x01\x03\x01\x01\x01\x00U' 

#include <SoftwareSerial.h>
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "Arial_black_16.h"

#define DISPLAYS_ACROSS 1   // Number of P1 panels in horizontal
#define DISPLAYS_DOWN   1   // Number of P1 panels in vertical
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

const byte TX_PIN = 2;
const byte RX_PIN = 3;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

// Define a struct for your data
struct CommunicationData {
  byte start;
  byte zone_id;
  byte total_sensor;
  byte sensor_status_1;
  byte sensor_status_2;
  byte total_engaged;
  byte total_disengaged;
  byte total_vacancy;
  byte total_error;
  byte end;
};

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start...");

  Soft_Serial.begin(9600);
  Timer1.initialize(3000);
  Timer1.attachInterrupt(ScanDMD);
}

void loop() {
  const int bufferSize = 20; // Adjust buffer size as needed
  char buffer[bufferSize];

  if (Soft_Serial.available() > 0) {
    int bytesRead = Soft_Serial.readBytesUntil('\n', buffer, bufferSize);

    byte hexData[bytesRead / 2];
    for (int i = 0; i < bytesRead / 2; i++) {
      hexData[i] = strtol(&buffer[i * 2], NULL, 16);
    }

    if (hexData[0] == 0xAA && hexData[bytesRead / 2 - 1] == 0x55) {
      // Convert the total vacancy byte to ASCII characters
      char vacancyChar = hexData[7] + '0';
      show_text(&vacancyChar, 1);
      Serial.print("Vacancy: ");
      Serial.println(vacancyChar);
    }
  }

  delay(2000);
  dmd.clearScreen(true);
}

void show_text(const char* text, int length) {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(0, 0, text, length, GRAPHICS_NORMAL);
}
*/
