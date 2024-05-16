// using the ArrowDisplay.h this library i can display the arrow on the p10 display.
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

//EXAMPLE-2 using the bitmap i have another code in which without using the above library i can display the arrow on the p10 display
/*
#include <SPI.h>
#include <DMD3.h>
#include <TimerOne.h>
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
*/
/*
//Yes, you can split the screen in half to display an arrow sign on one side and text on the other side. Here's how you can modify the code to achieve this:
#include <SoftwareSerial.h>
#include <SPI.h>
#include <DMD3.h>
#include <TimerOne.h>
#include <font/TimesNewRoman12.h>

const int ledPin = 13;
DMD3 display;

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

byte const arrow1[] PROGMEM = {
  16, 13,
  B00000001, B00000000,
  B00000011, B00000000,
  B00000111, B00000000,
  B00001111, B00000000,
  B00011111, B11111111,
  B00111111, B11111111,
  B01111111, B11111111,
  B01111111, B11111111,
  B00111111, B00000000,
  B00011111, B00000000,
  B00001111, B00000000,
  B00000111, B00000000,
  B00000011, B00000000,
};

const byte* frames[] = {
  arrow1,
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
  Soft_Serial.begin(9600);
  Timer1.initialize(2000);
  Timer1.attachInterrupt(scan);
  Timer1.pwm(9, 255);
}

void loop() {
  // Continuously display the arrow
  show_arrow(frames[frame]);
  delay(ADVANCE_MS);

  frame = (frame + 1) % NUM_FRAMES;

  // Check for serial data
  if (Soft_Serial.available() > 0) {
    process_serial_data();
  }
}

void process_serial_data() {
  char buffer[20];
  int bytesRead = Soft_Serial.readBytesUntil('\n', buffer, sizeof(buffer));

  Serial.print("Received raw data: ");
  Serial.println(buffer);
  
  // Check if the start and end bytes are correct
  if (buffer[0] == 'A' && buffer[1] == 'A' && buffer[bytesRead - 2] == '5' && buffer[bytesRead - 1] == '5') {
    // Extract the vacancy byte
    char vacancyCharHigh = buffer[14]; // Extract the high digit
    char vacancyCharLow = buffer[15];  // Extract the low digit
    //Serial.print(buffer[14]);
    //Serial.print(buffer[15]);
    int vacancyHigh = vacancyCharHigh - '0';
    int vacancyLow = vacancyCharLow - '0';
    int vacancy = vacancyHigh * 10 + vacancyLow;
    Serial.print("Vacancy: ");
    Serial.println(vacancy);
    // Display vacancy and arrow on the P10 display
    show_text(vacancy);
  }
  memset(buffer, 0, sizeof(buffer));
}

void show_text(int vacancy) {
  display.clear();
  char vacancyText[4];
  sprintf(vacancyText, "%d", vacancy);
  display.setFont(TimesNewRoman12); // Use a smaller font
  display.drawText(10, 0, vacancyText); // Adjust the position for text
  display.refresh(); // Refresh the display after drawing text
  delay(2000);
}

void show_arrow(const byte* arrow) {
  display.clear(); // Clear the display buffer before drawing the arrow
  display.drawBitmap(20, 2, arrow); // Draw arrow at the left side
  display.refresh(); // Refresh the display after drawing arrow
}
