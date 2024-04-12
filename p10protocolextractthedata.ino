/*
const int LED_PIN = 13;
void setup() {
  Serial.begin(9600); // Begin serial communication
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as output
}

void loop() {
  String protocol = "|S|1|4|1|128-0-#r201|";
  
  // Split the protocol string by '|'
  String parts[6];
  int partsCount = splitString(protocol, '|', parts);
  
  // Extracting each part
  String command = parts[1];
  int address = parts[2].toInt();
  
  // Extracting x and y coordinates, color, and display data if available
  String coordinates_color_data[3];
  int coordinates_color_data_count = splitString(parts[5], '-', coordinates_color_data);
  
  // Ensure correct format before extraction
  if (coordinates_color_data_count == 3) {
    int x_coordinate = coordinates_color_data[0].toInt();
    int y_coordinate = coordinates_color_data[1].toInt();
  
    // Extracting color and display data
    String color = "";
    int display_data = -1;
    int color_index = coordinates_color_data[2].indexOf('#');
    if (color_index != -1) {
      color = coordinates_color_data[2].substring(color_index, color_index + 2); // Extract color code from '#' onwards
      String data_str = coordinates_color_data[2].substring(color_index + 2); // Extract data after '#'
      display_data = data_str.toInt(); // Convert data to int if it's numeric
      
      // Output the extracted data
      Serial.print("Command: ");
      Serial.println(command);
      Serial.print("Address: ");
      Serial.println(address);
      Serial.print("X Coordinate: ");
      Serial.println(x_coordinate);
      Serial.print("Y Coordinate: ");
      Serial.println(y_coordinate);
      Serial.print("Color: ");
      Serial.println(color);
      Serial.print("Display Data: ");
      if (display_data != -1) {
        Serial.println(display_data);
      } else {
        Serial.println("None");
      }

      // Check if color is #r and turn on the LED
      if (color.equals("#r")) {
        Serial.println("Turning on LED...");
        digitalWrite(LED_PIN, HIGH);
      } else {
        Serial.println("Turning off LED...");
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
  
  delay(1000); // Delay for readability
}

int splitString(String input, char separator, String parts[]) {
  int partIndex = 0;
  int startIndex = 0;
  
  for (int i = 0; i < input.length(); i++) {
    if (input.charAt(i) == separator) {
      parts[partIndex++] = input.substring(startIndex, i);
      startIndex = i + 1;
    }
  }
  
  if (startIndex < input.length()) {
    parts[partIndex++] = input.substring(startIndex);
  }
  
  return partIndex;
}

*/

const int LED_PIN = 13;

void setup() {
  Serial.begin(9600); // Begin serial communication
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as output
}

void loop() {
  // Read the protocol string from serial input
  String protocol = readProtocol();

  // Split the protocol string by '|'
  String parts[6];
  int partsCount = splitString(protocol, '|', parts);
  
  // Ensure correct number of parts
  if (partsCount == 6) {
    // Extracting each part
    String command = parts[1];
    int address = parts[2].toInt();
  
    // Extracting x and y coordinates, color, and display data if available
    String coordinates_color_data[3];
    int coordinates_color_data_count = splitString(parts[5], '-', coordinates_color_data);
    
    // Ensure correct format before extraction
    if (coordinates_color_data_count == 3) {
      int x_coordinate = coordinates_color_data[0].toInt();
      int y_coordinate = coordinates_color_data[1].toInt();
    
      // Extracting color and display data
      String color = "";
      int display_data = -1;
      int color_index = coordinates_color_data[2].indexOf('#');
      if (color_index != -1) {
        color = coordinates_color_data[2].substring(color_index, color_index + 2); // Extract color code from '#' onwards
        String data_str = coordinates_color_data[2].substring(color_index + 2); // Extract data after '#'
        display_data = data_str.toInt(); // Convert data to int if it's numeric
        
        // Output the extracted data
        Serial.print("Command: ");
        Serial.println(command);
        Serial.print("Address: ");
        Serial.println(address);
        Serial.print("X Coordinate: ");
        Serial.println(x_coordinate);
        Serial.print("Y Coordinate: ");
        Serial.println(y_coordinate);
        Serial.print("Color: ");
        Serial.println(color);
        Serial.print("Display Data: ");
        if (display_data != -1) {
          Serial.println(display_data);
        } else {
          Serial.println("None");
        }

        // Check if color is #r and turn on the LED
        if (color.equals("#r")) {
          Serial.println("Turning on LED...");
          digitalWrite(LED_PIN, HIGH);
        } else {
          Serial.println("Turning off LED...");
          digitalWrite(LED_PIN, LOW);
        }
      }
    }
  }
  
  delay(1000); // Delay for readability
}

String readProtocol() {
  String protocol = "";
  
  // Read the serial input until '|' is received
  while (!Serial.available()) {}
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '|') {
      break;
    }
    protocol += c;
  }
  
  return protocol;
}

int splitString(String input, char separator, String parts[]) {
  int partIndex = 0;
  int startIndex = 0;
  
  for (int i = 0; i < input.length(); i++) {
    if (input.charAt(i) == separator) {
      parts[partIndex++] = input.substring(startIndex, i);
      startIndex = i + 1;
    }
  }
  
  if (startIndex < input.length()) {
    parts[partIndex++] = input.substring(startIndex);
  }
  
  return partIndex;
}
