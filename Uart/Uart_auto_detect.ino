#include <Arduino.h>
#include <SoftwareSerial.h>

// Structure to store UART device information
struct UARTDeviceInfo {
  uint8_t rxPin;      // RX pin for the device
  uint8_t txPin;      // TX pin for the device
  long baudRate;      // Baud rate for the device
  String name;        // Name or identifier for the device
  // Add more information as needed
};

constexpr int MAX_DEVICES = 10;
UARTDeviceInfo detectedDevices[MAX_DEVICES];
int detectedDevicesCount = 0;

bool sendAndCheckResponse(uint8_t rxPin, uint8_t txPin, long baudRate, const String& request, const String& expectedResponse) {
  SoftwareSerial serialSoft(rxPin, txPin);
  serialSoft.begin(baudRate);

  serialSoft.print(request);
  delay(10);

  String response;
  while (serialSoft.available()) {
    response += char(serialSoft.read());
  }

  serialSoft.end();

  return (response == expectedResponse);
}

void detectUARTDevices() {
  Serial.println("Detecting UART Devices...");

  for (uint8_t rxPin = 2; rxPin <= 19; ++rxPin) {
    for (uint8_t txPin = 2; txPin <= 19; ++txPin) {
      if (rxPin != txPin && detectedDevicesCount < MAX_DEVICES) {
        if (sendAndCheckResponse(rxPin, txPin, 9600, "ID_REQUEST", "ID_RESPONSE")) {
          detectedDevices[detectedDevicesCount++] = {rxPin, txPin, 9600, "Device " + String(detectedDevicesCount)};
        }
      }
    }
  }

  Serial.println("Detected UART Devices:");
  for (int i = 0; i < detectedDevicesCount; ++i) {
    Serial.print("RX Pin: ");
    Serial.print(detectedDevices[i].rxPin);
    Serial.print(", TX Pin: ");
    Serial.print(detectedDevices[i].txPin);
    Serial.print(", Baud Rate: ");
    Serial.print(detectedDevices[i].baudRate);
    Serial.print(", Name: ");
    Serial.println(detectedDevices[i].name);
    // Print additional information if needed
  }
}

void readUARTDataFromDevice(uint8_t rxPin, uint8_t txPin, long baudRate, int numBytes) {
  SoftwareSerial serialSoft(rxPin, txPin);
  serialSoft.begin(baudRate);

  Serial.print("Reading data from device with RX Pin ");
  Serial.print(rxPin);
  Serial.print(" and TX Pin ");
  Serial.println(txPin);

  for (int i = 0; i < numBytes; ++i) {
    if (serialSoft.available()) {
      char receivedChar = serialSoft.read();
      Serial.print(receivedChar);
    }
  }

  serialSoft.end();
}

void setup() {
  Serial.begin(9600);
  Serial.println("UART Auto-Detection and Reading Example");

  detectUARTDevices();
}

void loop() {
  // Read data from each detected device
  for (int i = 0; i < detectedDevicesCount; ++i) {
    readUARTDataFromDevice(detectedDevices[i].rxPin, detectedDevices[i].txPin, detectedDevices[i].baudRate, 10);
    delay(500);  // Add a delay between readings
  }

  // Your main loop logic goes here
  // You can add additional code as needed
}
