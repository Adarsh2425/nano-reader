# I2C Auto-Detection and Reading Example

This Arduino sketch demonstrates I2C auto-detection and reading from devices on the I2C bus. The code checks for proper signals and identifies potential garbage values during the detection and reading processes.

## Overview

The project consists of an Arduino sketch written in C++ using the Arduino Wire library. The main functionalities include:

- I2C auto-detection of connected devices on the bus.
- Reading data from each detected device.

## Dependencies

- [Arduino IDE](https://www.arduino.cc/en/software)
- [Wire Library](https://www.arduino.cc/en/Reference/Wire)

## Setup and Installation

1. Clone the repository.

    ```bash
    git clone https://github.com/your-username/your-repository.git
    ```

2. Open the Arduino IDE.

3. Open the main sketch file (`sketch_jan9a.ino` or similar) in the Arduino IDE.

4. Connect your Arduino board to your computer.

5. Upload the sketch to your Arduino board.

## Project Structure

- `sketch_jan9a.ino`: The main Arduino sketch file.

## Configuration

- No specific configuration is required for this example.

## Usage

1. The sketch will automatically detect I2C devices connected to the bus during setup.
2. Detected devices and their addresses will be printed to the Serial Monitor.
3. Data will be read from each detected device and printed to the Serial Monitor.

## Troubleshooting

- If "Garbage value or improper signal" messages are encountered, it indicates a potential issue during detection or reading. Review the troubleshooting section in the code or consult the documentation.
