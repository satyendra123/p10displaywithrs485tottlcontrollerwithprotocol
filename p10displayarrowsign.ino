#include <SPI.h>
#include <TimerOne.h>
#include <DMD.h>
#include <Arial_Black_16.h>

#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1

DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

byte arrow1[] = {
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
  B00000000, B00000011, B00000000, B00000000
};

void scanDisplay() {
  dmd.scanDisplayBySPI();
}

void setup() {
  Timer1.initialize(5000); // Refresh rate in microseconds
  Timer1.attachInterrupt(scanDisplay);
  dmd.clearScreen(true);
}

void loop() {
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  const char* arrow1Text = reinterpret_cast<const char*>(arrow1);
  dmd.drawMarquee(arrow1Text, 32, DISPLAYS_ACROSS);
  delay(2000); // Display the arrow for 2 seconds
}
// EXAMPLE-2 arrow sign display using the DMD3 library
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
