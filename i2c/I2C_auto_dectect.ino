#include <Wire.h>

struct I2CDeviceInfo {
  uint8_t address;
  String name;  // Name or identifier for the device
  // Add more information as needed
};

I2CDeviceInfo detectedDevices[10];  // Assuming a maximum of 10 devices on the bus
int detectedDevicesCount = 0;

bool isProperSignalDetected(uint8_t address) {
  Wire.beginTransmission(address);
  uint8_t result = Wire.endTransmission();

  // Proper signal conditions (device found and ACK received)
  return (result == 0 && Wire.available() == 0);
}

bool isGarbageValueDetected() {
  // Check if the bus is busy or a start or stop condition is detected
  uint8_t result = Wire.endTransmission();

  return (result != 0);
}

void detectI2CDevices() {
  Serial.println("Detecting I2C Devices...");

  Wire.begin();

  for (uint8_t address = 1; address < 127; ++address) {
    // Check for proper signal before attempting communication
    if (isProperSignalDetected(address)) {
      // Device found at this address, add it to the list
      detectedDevices[detectedDevicesCount].address = address;

      // Request more information from the device
      Wire.requestFrom(address, (uint8_t)1);  // Explicit cast to uint8_t
      if (Wire.available()) {
        detectedDevices[detectedDevicesCount].name = Wire.read();
        // Add more information retrieval if needed
      }

      detectedDevicesCount++;
      if (detectedDevicesCount >= sizeof(detectedDevices) / sizeof(detectedDevices[0])) {
        break;  // Reached the maximum number of detected devices
      }
    } else {
      // Handle garbage value or improper signal detection
      Serial.print("Garbage value or improper signal at address 0x");
      Serial.println(address, HEX);
    }
  }

  // Print detected devices
  Serial.println("Detected I2C Devices:");
  for (int i = 0; i < detectedDevicesCount; ++i) {
    Serial.print("Address: 0x");
    Serial.print(detectedDevices[i].address, HEX);
    Serial.print(", Name: ");
    Serial.println(detectedDevices[i].name);
    // Print additional information if needed
  }
}

void readI2CDataFromDevice(uint8_t deviceAddress, int numBytes) {
  Serial.print("Reading data from device at address 0x");
  Serial.println(deviceAddress, HEX);

  Wire.requestFrom(deviceAddress, (uint8_t)numBytes);  // Explicit cast to uint8_t

  // Check for garbage values during the read
  if (isGarbageValueDetected()) {
    Serial.println("Garbage value or improper signal during read");
    return;
  }

  while (Wire.available()) {
    char receivedChar = Wire.read();
    Serial.print(receivedChar);
  }

  Serial.println();
}

void setup() {
  Serial.begin(9600);
  Serial.println("I2C Auto-Detection and Reading Example");

  detectI2CDevices();
}

void loop() {
  // Read data from each detected device
  for (int i = 0; i < detectedDevicesCount; ++i) {
    readI2CDataFromDevice(detectedDevices[i].address, 10);  // Read 10 bytes (customize based on your devices)
    delay(500);  // Add a delay between readings
  }

  // Your main loop logic goes here
  // You can add additional code as needed
}
