#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <DMD2.h>
#include <SPI.h>
#include "fonts/Arial_Black_16_ISO_8859_1.h"

const int MAX_IDS = 5;
const int MESSAGE_LENGTH = 30; // Adjust based on expected message size
SoftDMD dmd(1, 1);
const uint8_t *FONT = Arial_Black_16_ISO_8859_1;
DMD_TextBox box(dmd);

void setup() {
    Serial.begin(9600);
    dmd.setBrightness(250);
    dmd.selectFont(FONT);
    dmd.begin();

    // Read saved ID from EEPROM
    int savedID = EEPROM.read(0);
    Serial.print("Saved ID on startup: ");
    Serial.println(savedID);
     // Display the saved message from EEPROM on startup
    handleReadMessageCommand();
}

void loop() {
    if (Serial.available()) {
        String receivedString = Serial.readStringUntil('\n');
        Serial.print("Received: ");
        Serial.println(receivedString);

        // Check for lowercase letters
        for (char c : receivedString) {
            if (c >= 'a' && c <= 'z') {
                Serial.println("INVALID COMMAND: Lowercase letters are not accepted.");
                return; // Exit if any lowercase letter is found
            }
        }

        // Handle saving IDs to EEPROM
        if (receivedString.startsWith("|S|")) {
            handleSaveCommand(receivedString);
        } else if (receivedString.startsWith("|C|") && receivedString.endsWith("|6|")) {
            handleConditionalClearCommand(receivedString);
        } 
        // Handle displaying data
        else if (receivedString.startsWith("|C|")) {
            handleDisplayCommand(receivedString);
        } else {
            Serial.println("INVALID COMMAND");
        }
    }
}

void handleSaveCommand(const String& command) {
    int startIndex = command.indexOf("|S|") + 3;
    int endIndex = command.indexOf("E", startIndex);

    if (startIndex < endIndex) {
        String idStr = command.substring(startIndex, endIndex);
        int id = idStr.toInt();

        if (id >= 1 && id <= MAX_IDS) {
            EEPROM.write(0, id); // Save only the new ID in the first byte
            Serial.print("Saved ID: ");
            Serial.println(id);
        } else {
            Serial.println("INVALID ID. Use 1 to 5.");
        }
    } else {
        Serial.println("INVALID SAVE COMMAND");
    }
}

void handleDisplayCommand(const String& command) {
    Serial.print("Received Display Command: ");
    Serial.println(command);

    // Validate command structure
    int startIndex = command.indexOf("|C|") + 3; 
    int endIndex1 = command.indexOf("|", startIndex);
    String idStr = command.substring(startIndex, endIndex1);
    int id = idStr.toInt();

    // Check saved ID in EEPROM
    if (EEPROM.read(0) != id) {
        Serial.println("Error: ID not found in EEPROM.");
        return;
    }

    // Extract temporary flag
    startIndex = endIndex1 + 1;
    int endIndex2 = command.indexOf("|", startIndex);
    String tempStr = command.substring(startIndex, endIndex2);
    int tempFlag = tempStr.toInt();

    // Extract next parameter
    startIndex = endIndex2 + 1;
    int endIndex3 = command.indexOf("|", startIndex);
    String nextParamStr = command.substring(startIndex, endIndex3);
    int nextParam = nextParamStr.toInt();

    // Extract x-y-input
    startIndex = endIndex3 + 1;
    int endIndex4 = command.indexOf("|", startIndex);
    String xyInput;
    if (endIndex4 == -1) {
        xyInput = command.substring(startIndex);
    } else {
        xyInput = command.substring(startIndex, endIndex4);
    }

    // Split x and y values
    int dashIndex = xyInput.indexOf('-');
    if (dashIndex == -1) {
        Serial.println("Error: No dash found in XY Input.");
        return;
    }

    String xStr = xyInput.substring(0, dashIndex);
    String yStr = xyInput.substring(dashIndex + 1);
    int x = xStr.toInt();
    int y = yStr.toInt();

    // Extract data to be displayed
    String data = xyInput.substring(xyInput.indexOf('-') + 1);
    int hashIndex = data.indexOf('#');
    if (hashIndex == -1) {
        Serial.println("Error: No hash found in input data.");
        return;
    }
    data = data.substring(hashIndex + 1); // Extract input after '#'

    // Validate data
    if (data.length() == 0) {
        Serial.println("Error: No data to display.");
        return;
    }

    // Handle temporary or storage behavior based on tempFlag
    if (tempFlag == 4) {
        // Temporary display
        dmd.clearScreen();
        dmd.drawString(x, y, data);
        Serial.println("Temporary data displayed.");
    } else if (tempFlag == 8) {
        // Store message in EEPROM
        for (int i = 0; i < MESSAGE_LENGTH; i++) {
            if (i < data.length()) {
                EEPROM.write(1 + i, data[i]); // Store after the ID
            } else {
                EEPROM.write(1 + i, 0); // Clear remaining EEPROM space
            }
        }
        Serial.print("Stored message in EEPROM: ");
        Serial.println(data);

        // Display the message
        dmd.clearScreen();
        dmd.drawString(x, y, data);
        Serial.println("Stored data displayed.");
    } else {
        Serial.println("INVALID TEMPORARY FLAG");
    }
}

void handleConditionalClearCommand(const String& command) {
    Serial.print("Received Clear Command: ");
    Serial.println(command);

    // Validate command structure
    int startIndex = command.indexOf("|C|") + 3; 
    int endIndex1 = command.indexOf("|", startIndex);
    if (endIndex1 == -1) {
        Serial.println("Error: No first pipe found.");
        return; // Exit if no pipe found
    }

    String idStr = command.substring(startIndex, endIndex1);
    int id = idStr.toInt();
    Serial.print("Extracted ID: ");
    Serial.println(id);

    // Check saved ID in EEPROM
    if (EEPROM.read(0) == id) {
        dmd.clearScreen();
        Serial.println("Display cleared.");
    } else {
        Serial.println("Error: ID does not match. Screen not cleared.");
    }
}
void handleReadMessageCommand() {
    Serial.println("message from EEPROM...");

    // Read the message from EEPROM
    String message = "";
    for (int i = 1; i < 1 + MESSAGE_LENGTH; i++) {
        char c = EEPROM.read(i);
        if (c == 0) break; // Stop reading at null terminator
        message += c;
    }

    // Display the message
    dmd.clearScreen();
    dmd.drawString(0, 0, message); // Display at coordinates (0,0)
    Serial.print("Displayed saved message: ");
    Serial.println(message);
}