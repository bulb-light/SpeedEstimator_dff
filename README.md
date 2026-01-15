# SpeedEstimator Library for Arduino

## Overview

The SpeedEstimator library provides a simple and efficient way to calculate motor speed in RPM using encoder pulse data. It is designed to be independent of the data source for position readings, making it versatile for various motor control applications.

## Features

- Calculates motor speed in RPM based on encoder pulse readings.
- Supports gear ratio adjustments.
- Provides a reset functionality to clear internal states.
- Lightweight and easy to integrate into Arduino projects.

## Installation

1. Copy the `SpeedEstimator.h` and `SpeedEstimator.cpp` files into your Arduino project's `lib/SpeedEstimator_dff/` directory if you are using PlatformIO in VS Code.
2. If you are using the Arduino IDE, copy the files to your Arduino libraries folder (e.g., `C:\Users\<YourUsername>\Documents\Arduino\libraries\SpeedEstimator_dff\`).
3. Alternatively, clone the repository directly into your libraries folder using the following command:

   ```bash
   git clone https://github.com/bulb-light/SpeedEstimator_dff.git <path_to_libraries_folder>
   ```
4. Include the library in your project using `#include <SpeedEstimator.h>`.

## API Reference

### Constructor

```cpp
SpeedEstimator(float ppr, float gearRatio);
```
- `ppr`: Pulses per revolution of the encoder.
- `gearRatio`: Gear ratio of the motor.

### Methods

#### `float estimateSpeed(int pulsesCount)`
Calculates the motor speed in RPM based on the number of encoder pulses.

- `pulsesCount`: The number of pulses counted by the encoder.
- **Returns**: The calculated speed in RPM.

#### `void reset()`
Resets the internal state of the speed estimator.

## Example Usage

Below is an example of using the SpeedEstimator library to calculate motor speed:

```cpp
#include <Arduino.h>
#include <SpeedEstimator.h>

// Encoder parameters
float ppr = 22.0; // Pulses per revolution
float gearRatio = 9.3; // Gear ratio
SpeedEstimator speedEstimator(ppr, gearRatio);

void setup() {
    Serial.begin(115200);
    speedEstimator.reset();
}

void loop() {
    int currentPulses = 500; // Example pulse count
    float speed = speedEstimator.estimateSpeed(currentPulses);

    Serial.print("Motor Speed: ");
    Serial.print(speed);
    Serial.println(" RPM");

    delay(10); // Simulate periodic updates
}
```

## Important Notes

- The `estimateSpeed` method includes a hardcoded filter for smoothing speed calculations. This may limit the library's effectiveness for high-speed motors or applications requiring rapid response times.
- The hardcoded filter in `estimateSpeed` is designed for a sampling time of 0.01 seconds and implements a first-order Butterworth (IIR) filter. Adjustments may be required for different sampling times or filtering requirements.
- Contributions to improve the library, such as making the filter configurable, are highly encouraged. Feel free to submit pull requests or open issues on the GitHub repository.

## Notes

- Ensure that the `estimateSpeed(int pulsesCount)` method is called at regular intervals to maintain accurate speed calculations.
- The library assumes that the encoder pulses are counted externally and passed to the `estimateSpeed` method.

## License

This library is open-source and available under the MIT License.