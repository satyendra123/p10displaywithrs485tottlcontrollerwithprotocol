//Example-1 this is the code for p10 display. i can set the address by giving the protocol BB 01 EE. so it will set the address and stored in the eeprom memory. it will also display the
// address in the eeprom memory and display it in the screen. and if i send the data AA 01 17 03 31 32 33 8C then the data will be displayed in the screen 123. and if i send the
//protocol CC 01 06 means clear the screen which have the address 01.
/*
#include <DMD3.h>
#include "Arial_black_16.h"
#include <EEPROM.h>

// Define your CommunicationData struct for sending the data
struct DataPacket {
    byte startData;
    byte address;
    byte command;
    byte dataSize;
    byte data[3];
    byte checksum;
};

// Define your CommunicationData struct for setting the address
struct AddressData {
    byte startAddress;
    byte address;
    byte checksum;
};

// Define your ClearScreenData struct for clearing the screen
// Define your ClearScreenData struct for clearing the screen
struct ClearScreenData {
    byte startClear;
    byte address;
    byte checksum;
};


// Instance of DMD3 display
DMD3 display;

// Define EEPROM address to store the address
#define EEPROM_ADDRESS 0

// Global variable for the display address
byte displayAddress; // Default display address

// Function to refresh display
void scan() {
    display.refresh();
}

// Function to update display and EEPROM with new address
void updateAddress(byte newAddress) {
    displayAddress = newAddress;
    // Store the address in EEPROM
    EEPROM.write(EEPROM_ADDRESS, displayAddress);
    // Update the display
    display.clear();
    display.setFont(Arial_black_16);
    char addressStr[3]; // Extra byte for null terminator
    snprintf(addressStr, sizeof(addressStr), "%02X", displayAddress);
    display.drawText(0, 0, addressStr);
    display.refresh();
}

// Function to clear the display screen and display a default message
void clearScreen() {
    display.clear();
    display.setFont(Arial_black_16);
    display.drawText(0, 0, "Screen Cleared"); // Display default message
    display.refresh();
    delay(2000); // Delay to show the message for 2 seconds
    display.clear(); // Clear the screen after displaying the message
    display.refresh();
}


void setup() {
    // Initialize display and timer
    Timer1.initialize(2000);
    Timer1.attachInterrupt(scan);
    Timer1.pwm(9, 200);

    // Start serial communication
    Serial.begin(9600); // Change baud rate as needed

    // Read the address from EEPROM
    byte storedAddress = EEPROM.read(EEPROM_ADDRESS);
    if (storedAddress != 0xFF) { // Check if address is valid
        displayAddress = storedAddress;
    } else {
        // Set the default display address
        displayAddress = 0x01;
        // Store the default address in EEPROM
        EEPROM.write(EEPROM_ADDRESS, displayAddress);
    }

    // Display the address on the P10 display
    display.clear();
    display.setFont(Arial_black_16);
    char addressStr[3]; // Extra byte for null terminator
    snprintf(addressStr, sizeof(addressStr), "%02X", displayAddress);
    display.drawText(0, 0, addressStr);
    display.refresh();
}

void loop() {
    if (Serial.available() >= sizeof(DataPacket)) {
        // Read data from serial into the struct for sending data
        DataPacket receivedData;
        Serial.readBytes((char*)&receivedData, sizeof(DataPacket));

        // Verify if the received data is valid for sending data
        if (receivedData.startData == 0xAA && receivedData.checksum == 0x8C && receivedData.address == displayAddress) {
            // Display the data
            display.clear();
            // Convert the received data to ASCII characters and display
            char dataStr[4]; // Extra byte for null terminator
            for (int i = 0; i < receivedData.dataSize; i++) {
                dataStr[i] = receivedData.data[i];
            }
            dataStr[receivedData.dataSize] = '\0'; // Null terminate the string
            display.setFont(Arial_black_16);
            display.drawText(10, 0, dataStr); // Display the received text
            display.refresh();
        }
    }

    if (Serial.available() >= sizeof(AddressData)) {
        // Read data from serial into the struct for setting address
        AddressData receivedAddress;
        Serial.readBytes((char*)&receivedAddress, sizeof(AddressData));

        // Verify if the received data is valid for setting address
        if (receivedAddress.startAddress == 0xBB && receivedAddress.checksum == 0xEE) {
            // Update address
            updateAddress(receivedAddress.address);
        }
    }

    if (Serial.available() >= sizeof(ClearScreenData)) {
        // Read data from serial into the struct for clearing screen
        ClearScreenData receivedClear;
        Serial.readBytes((char*)&receivedClear, sizeof(ClearScreenData));

        // Verify if the received data is valid for clearing the screen
      if (receivedClear.startClear == 0xCC && receivedClear.address == displayAddress && receivedClear.checksum == 0xEE) {
      // Clear the screen
      clearScreen();
    }

    }

    // Wait for a while before proceeding
    delay(2000); // Adjust delay as needed
}
*/
//Example-2 now we can set the address by the protocol BB 01 EE. and i send the data AA 01 17 03 31 35 36 8C and i can also clear the screen by sending the command CC 01 EE for clear the screen  
//also i have send the data through python code and updated the screen.
#include <DMD3.h>
#include "Arial_black_16.h"
#include <EEPROM.h>

// Define your CommunicationData struct for sending the data
struct DataPacket {
    byte startData;
    byte address;
    byte command;
    byte dataSize;
    byte data[3];
    byte checksum;
};

// Define your CommunicationData struct for setting the address
struct AddressData {
    byte startAddress;
    byte address;
    byte checksum;
};

// Define your ClearScreenData struct for clearing the screen
struct ClearScreenData {
    byte startClear;
    byte address;
    byte checksum;
};

// Instance of DMD3 display
DMD3 display;

// Define EEPROM address to store the address
#define EEPROM_ADDRESS 0

// Global variable for the display address
byte displayAddress; // Default display address

// Function to refresh display
void scan() {
    display.refresh();
}

// Function to update display and EEPROM with new address
void updateAddress(byte newAddress) {
    displayAddress = newAddress;
    // Store the address in EEPROM
    EEPROM.write(EEPROM_ADDRESS, displayAddress);
    // Update the display
    display.clear();
    display.setFont(Arial_black_16);
    char addressStr[3]; // Extra byte for null terminator
    snprintf(addressStr, sizeof(addressStr), "%02X", displayAddress);
    display.drawText(0, 0, addressStr);
    display.refresh(); // Refresh the display after updating the address
}

// Function to clear the display screen and display a default message
void clearScreen() {
    display.clear();
    display.setFont(Arial_black_16);
    display.drawText(0, 0, "Screen Cleared"); // Display default message
    display.refresh();
    delay(2000); // Delay to show the message for 2 seconds
    display.clear(); // Clear the screen after displaying the message
    display.refresh();
}

void setup() {
    // Initialize display and timer
    Timer1.initialize(2000);
    Timer1.attachInterrupt(scan);
    Timer1.pwm(9, 200);

    // Start serial communication
    Serial.begin(9600); // Change baud rate as needed

    // Read the address from EEPROM
    byte storedAddress = EEPROM.read(EEPROM_ADDRESS);
    if (storedAddress != 0xFF) { // Check if address is valid
        displayAddress = storedAddress;
    } else {
        // Set the default display address
        displayAddress = 0x01;
        // Store the default address in EEPROM
        EEPROM.write(EEPROM_ADDRESS, displayAddress);
    }

    // Display the address on the P10 display
    display.clear();
    display.setFont(Arial_black_16);
    char addressStr[3]; // Extra byte for null terminator
    snprintf(addressStr, sizeof(addressStr), "%02X", displayAddress);
    display.drawText(0, 0, addressStr);
    display.refresh();
}

void loop() {
    if (Serial.available() >= sizeof(DataPacket)) {
        // Read data from serial into the struct for sending data
        DataPacket receivedData;
        Serial.readBytes((char*)&receivedData, sizeof(DataPacket));

        // Verify if the received data is valid for sending data
        if (receivedData.startData == 0xAA && receivedData.checksum == 0x8C && receivedData.address == displayAddress) {
            // Display the data
            display.clear();
            // Convert the received data to ASCII characters and display
            char dataStr[4]; // Extra byte for null terminator
            for (int i = 0; i < receivedData.dataSize; i++) {
                dataStr[i] = receivedData.data[i];
            }
            dataStr[receivedData.dataSize] = '\0'; // Null terminate the string
            display.setFont(Arial_black_16);
            display.drawText(10, 0, dataStr); // Display the received text
            display.refresh();
        }
    }

    if (Serial.available() >= sizeof(AddressData)) {
        // Read data from serial into the struct for setting address
        AddressData receivedAddress;
        Serial.readBytes((char*)&receivedAddress, sizeof(AddressData));

        // Verify if the received data is valid for setting address
        if (receivedAddress.startAddress == 0xBB && receivedAddress.checksum == 0xEE) {
            // Update address
            updateAddress(receivedAddress.address);
        }
    }

    if (Serial.available() >= sizeof(ClearScreenData)) {
        // Read data from serial into the struct for clearing screen
        ClearScreenData receivedClear;
        Serial.readBytes((char*)&receivedClear, sizeof(ClearScreenData));

        // Verify if the received data is valid for clearing the screen
        if (receivedClear.startClear == 0xCC && receivedClear.address == displayAddress && receivedClear.checksum == 0xEE) {
            // Clear the screen
            clearScreen();
        }
    }

    // Wait for a while before proceeding
    delay(2000); // Adjust delay as needed
}
/*
import serial
import time

# Define the serial port and baud rate
serial_port = 'COM3'  # Change this to match your serial port
baud_rate = 9600

# Initialize the serial connection
ser = serial.Serial(serial_port, baud_rate)

try:
    # Define the data packets

    data_packets = [
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x31, 0x39, 0x37, 0x8C]),
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x31, 0x35, 0x37, 0x8C]),
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x31, 0x39, 0x33, 0x8C]),
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x34, 0x39, 0x36, 0x8C]),
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x31, 0x39, 0x37, 0x8C]),
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x31, 0x38, 0x37, 0x8C]),
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x32, 0x35, 0x37, 0x8C]),
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x35, 0x39, 0x33, 0x8C]),
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x34, 0x39, 0x38, 0x8C]),
        bytearray([0xAA, 0x05, 0x17, 0x03, 0x31, 0x39, 0x33, 0x8C]),
    ]

    # Loop through the data packets and send them individually
    for data_packet in data_packets:
        # Convert the data packet to uppercase hexadecimal string with spaces
        hex_data = ' '.join(format(byte, '02X') for byte in data_packet)
        
        # Send the data packet
        ser.write(data_packet)
        
        print("Data sent:", hex_data)
        
        # Wait for a short interval before sending the next packet
        time.sleep(2)  # 2 seconds delay between packets
except KeyboardInterrupt:
    # Close the serial connection on Ctrl+C
    ser.close()
    print("Serial connection closed")

*/
/*
i want the same code which is written in arduino

/#include <SoftwareSerial.h>

// Define the data packets
byte data_packets[][8] = {
  {0xAA, 0x05, 0x17, 0x03, 0x31, 0x39, 0x37, 0x8C},
  {0xAA, 0x05, 0x17, 0x03, 0x31, 0x35, 0x37, 0x8C},
  {0xAA, 0x05, 0x17, 0x03, 0x31, 0x39, 0x33, 0x8C},
  {0xAA, 0x05, 0x17, 0x03, 0x34, 0x39, 0x36, 0x8C},
  {0xAA, 0x05, 0x17, 0x03, 0x31, 0x39, 0x37, 0x8C},
  {0xAA, 0x05, 0x17, 0x03, 0x31, 0x38, 0x37, 0x8C},
  {0xAA, 0x05, 0x17, 0x03, 0x32, 0x35, 0x37, 0x8C},
  {0xAA, 0x05, 0x17, 0x03, 0x35, 0x39, 0x33, 0x8C},
  {0xAA, 0x05, 0x17, 0x03, 0x34, 0x39, 0x38, 0x8C},
  {0xAA, 0x05, 0x17, 0x03, 0x31, 0x39, 0x33, 0x8C}
};

// Define the number of data packets
#define NUM_PACKETS 10

// Define RS485 pin connections
#define RS485_RX 3
#define RS485_TX 2

// Initialize SoftwareSerial for RS485 communication
SoftwareSerial RS485Serial(RS485_RX, RS485_TX);

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Initialize RS485 communication
  RS485Serial.begin(9600);
}

void loop() {
  // Loop through the data packets and send them individually
  for (int i = 0; i < NUM_PACKETS; i++) {
    // Send the data packet
    for (int j = 0; j < 8; j++) {
      RS485Serial.write(data_packets[i][j]);
    }
    // Delay for 2 seconds between sending packets
    delay(2000);
  }
}
*/