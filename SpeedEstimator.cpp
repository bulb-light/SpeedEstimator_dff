/**
 * @file SpeedEstimator.cpp
 * @brief Implementation of the SpeedEstimator class.
 */

#include "SpeedEstimator.h"

SpeedEstimator::SpeedEstimator(float ppr, float gearRatio)
    : mPrevTime(0), mPrevNumPulses(0), mSpeedFilt(0), mSpeedPrev(0), mPpr(ppr), mGearRatio(gearRatio) {}

float SpeedEstimator::estimateSpeed(int pulsesCount) {
    long currTime = micros();
    float deltaTime = ((float)(currTime - mPrevTime)) / 1.0e6;

    if (deltaTime <= 0) {
        // Avoid division by zero or negative time intervals
        return mSpeedFilt;
    }

    float velocity = ((float)(pulsesCount - mPrevNumPulses)) / deltaTime;
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