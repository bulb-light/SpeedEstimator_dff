// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root or https://opensource.org/licenses/MIT

#include <Arduino.h>
#include <util/atomic.h>
#include <SpeedEstimator.h>
// Make sure you have the DigitalFilter_dff library available
// in your project to use the filters in this example
#include <DigitalFilter.h>

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

// globals speed measurement and control variables
float speedRPM = 0.0; // current velocity
float speedRPMFiltEWMA = 0.0; // filtered velocity with EWMA Low Pass Filter
float speedRPMFiltMA = 0.0; // filtered velocity with Moving Average Filter
 
// timer control
// using unsigned long for millis() compatibility and overflow handling
unsigned long prevMillisSpeed = 0; // previous timestamp for speed measurement
unsigned long intervalTs = 10; // 10ms interval (sampling time)

// Create a low pass EWMA filter and Moving Average filter for speed readings
float cutoffFrequency = 8.0; // Cutoff frequency for EWMA Low Pass Filter (Hz)
float samplingRate = 1.0 / ((float)intervalTs / 1000.0); // Sampling rate (Hz)
DigitalFilter ewmaLowPassFilter(DigitalFilter::Type::EWMALowPass, cutoffFrequency, samplingRate);

// 10-point Moving Average Filter
DigitalFilter movingAverageFilter(DigitalFilter::Type::MovingAverage, 10);

// NOTE: These steps are mandatory to use the SpeedEstimator class!
// Implement your own method to read encoder pulses. This is just a simplified example.

// Global variables: Encoder counter
volatile long pos_i = 0;

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
    unsigned long elapsed = millis();
    unsigned int speedValue = (elapsed / 10) % 256; // Speed value between 0-255
    analogWrite(ENA, speedValue);

    // Read the position in an atomic block to avoid inconsistency due to interrupts
    long currentPulses;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        currentPulses = pos_i;
    }
    // Estimate speed at defined time interval (10ms)
    if ((millis() - prevMillisSpeed) > intervalTs) {
        speedRPM = speedEstimator.estimateSpeed(currentPulses);
        // aply low pass filter to speed reading
        speedRPMFiltEWMA = ewmaLowPassFilter.computeFilterOut(speedRPM);
        speedRPMFiltMA = movingAverageFilter.computeFilterOut(speedRPM);
        prevMillisSpeed = millis();

        // Serial.print("Motor Speed: ");
        Serial.print(speedRPMFiltEWMA);
        Serial.print(" ");
        Serial.print(speedRPMFiltMA);
        Serial.print(" ");
        Serial.println(speedRPM);
        // Serial.println(" RPM");
    }
}

void readEncoderPulses()
{
  // Just a simple counter increment example
  // In a real scenario, you would read the encoder pins and determine direction
  pos_i++;
}