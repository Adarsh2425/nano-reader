#include <SPI.h>

struct SPIDeviceInfo {
  uint8_t csPin;  // Chip Select (CS) pin for the device
  String name;    // Name or identifier for the device
  // Add more information as needed
};

constexpr int MAX_DEVICES = 10;
SPIDeviceInfo detectedDevices[MAX_DEVICES];
int detectedDevicesCount = 0;

void detectSPIDevices() {
  Serial.println("Detecting SPI Devices...");

  SPI.begin();

  for (uint8_t csPin = 10; csPin <= 13; ++csPin) {
    digitalWrite(csPin, LOW);
    delayMicroseconds(10);

    if (digitalRead(csPin) == LOW && detectedDevicesCount < MAX_DEVICES) {
      detectedDevices[detectedDevicesCount++] = {csPin, "Device " + String(detectedDevicesCount)};
    }

    digitalWrite(csPin, HIGH);
  }

  Serial.println("Detected SPI Devices:");
  for (int i = 0; i < detectedDevicesCount; ++i) {
    Serial.print("CS Pin: ");
    Serial.print(detectedDevices[i].csPin);
    Serial.print(", Name: ");
    Serial.println(detectedDevices[i].name);
    // Print additional information if needed
  }
}

void readSPIDataFromDevice(uint8_t csPin, int numBytes) {
  Serial.print("Reading data from device with CS Pin ");
  Serial.println(csPin);

  digitalWrite(csPin, LOW);
  delayMicroseconds(10);

  for (int i = 0; i < numBytes; ++i) {
    byte data = SPI.transfer(0x00);
    Serial.print("Data Byte ");
    Serial.print(i + 1);
    Serial.print(": 0x");
    Serial.println(data, HEX);
  }

  digitalWrite(csPin, HIGH);
}

void setup() {
  Serial.begin(9600);
  Serial.println("SPI Auto-Detection and Reading Example");

  for (int csPin = 10; csPin <= 13; ++csPin) {
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);
  }

  detectSPIDevices();
}

void loop() {
  for (int i = 0; i < detectedDevicesCount; ++i) {
    readSPIDataFromDevice(detectedDevices[i].csPin, 5);  // Read 5 bytes (customize based on your devices)
    delay(500);
  }

  // Your main loop logic goes here
  // You can add additional code as needed
}
