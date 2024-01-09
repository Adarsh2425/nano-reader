#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>

// Define the maximum number of devices to detect
constexpr int MAX_UART_DEVICES = 10;
constexpr int MAX_I2C_DEVICES = 10;
constexpr int MAX_SPI_DEVICES = 10;

// LED pin for visual indication
constexpr int LED_PIN = 13;

// Structure to store information about detected UART devices
struct UARTDeviceInfo {
  uint8_t rxPin;
  uint8_t txPin;
  long baudRate;
  String name;
  // Add more information as needed
};

// Structure to store information about detected I2C devices
struct I2CDeviceInfo {
  uint8_t address;
  String name;
  // Add more information as needed
};

// Structure to store information about detected SPI devices
struct SPIDeviceInfo {
  uint8_t csPin;
  SPISettings settings;
  String name;  // Add more information as needed
};

// Arrays to store detected devices
UARTDeviceInfo detectedUARTDevices[MAX_UART_DEVICES];
I2CDeviceInfo detectedI2CDevices[MAX_I2C_DEVICES];
SPIDeviceInfo detectedSPIDevices[MAX_SPI_DEVICES];

// Counts of detected devices
int detectedUARTDevicesCount = 0;
int detectedI2CDevicesCount = 0;
int detectedSPIDevicesCount = 0;

// Function to initialize the LED
void initializeLED() {
  pinMode(LED_PIN, OUTPUT);
}

// Function to blink the LED as a visual indication
void blinkLED() {
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
}

// Function to send a request and check the response for UART devices
bool sendAndCheckResponse(uint8_t rxPin, uint8_t txPin, long baudRate, const String& request, const String& expectedResponse) {
  SoftwareSerial serialSoft(rxPin, txPin);
  serialSoft.begin(baudRate);
  serialSoft.write(request.c_str());

  delay(10);

  String response;
  while (serialSoft.available()) {
    response += char(serialSoft.read());
  }

  serialSoft.end();

  return (response == expectedResponse);
}

// Function to detect UART devices
void detectUARTDevices() {
  Serial.println("Detecting UART Devices...");

  long baudRates[] = {9600, 19200, 38400, 57600, 115200};

  for (int i = 0; i < sizeof(baudRates) / sizeof(baudRates[0]); ++i) {
    for (uint8_t rxPin = 2; rxPin <= 19; ++rxPin) {
      for (uint8_t txPin = 2; txPin <= 19; ++txPin) {
        if (rxPin != txPin && detectedUARTDevicesCount < MAX_UART_DEVICES) {
          // Send a request and check the response for UART devices
          if (sendAndCheckResponse(rxPin, txPin, baudRates[i], "ID_REQUEST", "ID_RESPONSE")) {
            detectedUARTDevices[detectedUARTDevicesCount++] = {rxPin, txPin, baudRates[i], "Device " + String(detectedUARTDevicesCount)};
            blinkLED(); // Blink LED on successful detection
          }
        }
      }
    }
  }
}

// Function to detect I2C devices
void detectI2CDevices() {
  Serial.println("Detecting I2C Devices...");

  for (uint8_t sda = 2; sda <= 19; ++sda) {
    for (uint8_t scl = 2; scl <= 19; ++scl) {
      if (sda != scl && detectedI2CDevicesCount < MAX_I2C_DEVICES) {
        Wire.begin();
        Wire.beginTransmission(sda);
        uint8_t result = Wire.endTransmission();

        if (result == 0) {
          detectedI2CDevices[detectedI2CDevicesCount++] = {sda, "Device " + String(detectedI2CDevicesCount)};
          blinkLED(); // Blink LED on successful detection
        } else {
          Serial.print("Garbage value or improper signal at address 0x");
          Serial.println(sda, HEX);
        }
      }
    }
  }
}

// Function to detect SPI devices
void detectSPIDevices() {
  Serial.println("Detecting SPI Devices...");

  for (uint8_t csPin = 2; csPin <= 19; ++csPin) {
    for (uint8_t spiMode = 0; spiMode <= 3; ++spiMode) {
      for (uint8_t bitOrder = 0; bitOrder <= 1; ++bitOrder) {
        for (uint32_t dataRate = 1000000; dataRate <= 8000000; dataRate *= 2) {
          if (detectedSPIDevicesCount < MAX_SPI_DEVICES) {
            SPISettings settings(dataRate, bitOrder ? MSBFIRST : LSBFIRST, spiMode);
            SPI.beginTransaction(settings);

            // Placeholder for SPI detection logic
            digitalWrite(csPin, LOW);
            delayMicroseconds(10); // Adjust the delay based on your SPI device requirements
            uint8_t response = SPI.transfer(0x00); // Send a dummy byte
            digitalWrite(csPin, HIGH);

            SPI.endTransaction();

            if (response != 0xFF) {
              detectedSPIDevices[detectedSPIDevicesCount++] = {csPin, settings, "Device " + String(detectedSPIDevicesCount)};
              blinkLED(); // Blink LED on successful detection
            }
          }
        }
      }
    }
  }
}

// Function to print information about detected UART devices
void printUARTDeviceInfo() {
  Serial.println("UART Devices:");
  for (int i = 0; i < detectedUARTDevicesCount; ++i) {
    Serial.print("Name: ");
    Serial.print(detectedUARTDevices[i].name);
    Serial.print(", RX Pin (Receive): ");
    Serial.print(detectedUARTDevices[i].rxPin);
    Serial.print(", TX Pin (Transmit): ");
    Serial.print(detectedUARTDevices[i].txPin);
    Serial.print(", Baud Rate: ");
    Serial.println(detectedUARTDevices[i].baudRate);
    // Print additional information if needed
  }
}

// Function to print information about detected I2C devices
void printI2CDeviceInfo() {
  Serial.println("I2C Devices:");
  for (int i = 0; i < detectedI2CDevicesCount; ++i) {
    Serial.print("Address: 0x");
    Serial.print(detectedI2CDevices[i].address, HEX);
    Serial.print(", Name: ");
    Serial.println(detectedI2CDevices[i].name);
    // Print additional information if needed
  }
}

// Function to print information about detected SPI devices
void printSPIDeviceInfo() {
  Serial.println("SPI Devices:");
  for (int i = 0; i < detectedSPIDevicesCount; ++i) {
    Serial.print("CS Pin: ");
    Serial.print(detectedSPIDevices[i].csPin);
    Serial.print(", Name: ");
    Serial.println(detectedSPIDevices[i].name);
    // Print additional information if needed
  }
}

// Function to print information about all detected devices
void printDeviceInfo() {
  Serial.println("Detected Devices Information:");

  printUARTDeviceInfo();
  printI2CDeviceInfo();
  printSPIDeviceInfo(); // Add more print functions if needed

  // You can add other device print functions here
}

// Function to detect and print information about all devices
void detectAndPrintDevices() {
  detectUARTDevices();
  detectI2CDevices();
  detectSPIDevices(); // Add more detection functions if needed

  // You can add other device detection functions here

  // Print all detected devices information
  printDeviceInfo();
}

void setup() {
  Serial.begin(9600);
  initializeLED();

  detectAndPrintDevices();
}

void loop() {
  // Your main loop logic goes here
  // You can add additional code as needed
}
