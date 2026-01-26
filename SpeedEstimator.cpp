// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root or https://opensource.org/licenses/MIT

/**
 * @file SpeedEstimator.cpp
 * @brief Implementation of the SpeedEstimator class.
 */

#include "SpeedEstimator.h"

SpeedEstimator::SpeedEstimator(float ppr, float gearRatio)
    : mPrevTime(0), mPrevNumPulses(0), mSpeedFilt(0), mSpeedPrev(0), mPpr(ppr), mGearRatio(gearRatio) {}

float SpeedEstimator::estimateSpeed(int pulsesCount) {
    unsigned long currTime = micros();
    // Handle micros() overflow: unsigned arithmetic automatically wraps correctly
    unsigned long deltaTimeMicros = currTime - mPrevTime;
    float deltaTime = ((float)deltaTimeMicros) / 1.0e6;

    if (deltaTime <= 0) {
        // Avoid division by zero or negative time intervals
        return mSpeedFilt;
    }

    // Handle pulse counter overflow by calculating the signed difference
    // If pulsesCount wrapped around, this correctly computes the difference
    int pulseDiff = pulsesCount - mPrevNumPulses;
    float velocity = ((float)pulseDiff) / deltaTime;
    
    mPrevNumPulses = pulsesCount;
    mPrevTime = currTime;

    // Convert counts/s to RPM (60 seconds per minute)
    velocity = (velocity / mPpr) * (1.0 / mGearRatio) * 60.0;

    // Low-pass filter
    // TODO: Tune filter coefficients as needed, or expand the
    // class to allow user-defined coefficients
    mSpeedFilt = 0.7265 * mSpeedFilt + 0.1367 * velocity + 0.1367 * mSpeedPrev;
    mSpeedPrev = velocity;

    return mSpeedFilt;
}

void SpeedEstimator::reset() {
    mPrevTime = 0;
    mPrevNumPulses = 0;
    mSpeedFilt = 0;
    mSpeedPrev = 0;
}