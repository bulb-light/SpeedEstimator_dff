// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root or https://opensource.org/licenses/MIT

#include <Arduino.h>
#include <util/atomic.h>
#include <SpeedEstimator.h>

// Motor control pins
// Modify these pin definitions as per your wiring
#define IN1 8
#define IN2 7
#define ENA 10

// NOTE: The following steps are mandatory to use the SpeedEstimator class!
// Encoder pins (change as needed)
#define ENCA 3
#define ENCB 2

// Encoder parameters
float ppr = 22.0; // Pulses per revolution
float gearRatio = 9.3; // Gear ratio
SpeedEstimator speedEstimator(ppr, gearRatio);

// NOTE: These steps are mandatory to use the SpeedEstimator class!
// Implement your own method to read encoder pulses. This is just a simplified example.

// Global variables: Encoder counter
volatile int pos_i = 0;

// Method to read encoder pulses
void readEncoderPulses();

void setup() {
    Serial.begin(115200);
    speedEstimator.reset();

    // Motor control pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);

    // Motor setup: Move motor
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0); // Set speed (0-255)

    // Setting up encoder interrupts (example for Arduino Uno or Nano (using pin 2 and 3) with a simple encoder)
    attachInterrupt(digitalPinToInterrupt(ENCA), readEncoderPulses, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCB), readEncoderPulses, RISING);
}

void loop() {
    // Vary speed based on the elapsed time for demonstration
    long elapsed = millis();
    int speedValue = (elapsed / 10) % 256; // Speed value between 0-255
    analogWrite(ENA, speedValue);

    // Read the position in an atomic block to avoid inconsistency due to interrupts
    int currentPulses;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        currentPulses = pos_i;
    }
    // Estimate speed
    float speed = speedEstimator.estimateSpeed(currentPulses);

    // Serial.print("Motor Speed: ");
    Serial.print(speed);
    Serial.print(" ");
    Serial.println(0);
    // Serial.println(" RPM");

    delay(10); // Simulate periodic updates
}

void readEncoderPulses()
{
  // Just a simple counter increment example
  // In a real scenario, you would read the encoder pins and determine direction
  pos_i++;
}