// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root or https://opensource.org/licenses/MIT

/**
 * @file SpeedEstimator.h
 * @brief Library for calculating motor speed using encoder pulse data.
 *
 * This library provides a class to calculate the speed of a motor in RPM
 * based on encoder pulse readings, independent of the data source.
 * 
 * @note This class is independent of the data source for position readings.
 * This implementation assumes that estimateSpeed(int pulsesCount) is called periodically.
 */

#ifndef __SPEEDESTIMATOR_H__
#define __SPEEDESTIMATOR_H__

#include <Arduino.h>

/**
 * @class SpeedEstimator
 * @brief A class to calculate motor speed using encoder pulse data.
 * @note This class is independent of the data source for position readings.
 * This implementation assumes that estimateSpeed(int pulsesCount) is called periodically.
 * 
 * Example usage:
 * @code
 * SpeedEstimator speedEstimator(ppr, gearRatio); // ppr (pulses per revolution), gearRatio
 * float speed = speedEstimator.estimateSpeed(currentPulses);
 * @endcode
 */
class SpeedEstimator {
    private:
        unsigned long mPrevTime; ///< Previous timestamp in microseconds.
        int mPrevNumPulses; ///< Previous number of pulses.
        float mSpeedFilt; ///< Filtered velocity.
        float mSpeedPrev; ///< Previous velocity.

        float mPpr; ///< Pulses per revolution of the encoder.
        float mGearRatio; ///< Gear ratio of the motor.

    public:
        /**
         * @brief Constructor for SpeedEstimator.
         * @param ppr Pulses per revolution of the encoder.
         * @param gearRatio Gear ratio of the motor.
         */
        SpeedEstimator(float ppr, float gearRatio);

        /**
         * @brief Calculate the speed of the motor in RPM.
         * @param pulsesCount The number of pulses counted by the encoder.
         * @return The calculated speed in RPM.
         */
        float estimateSpeed(int pulsesCount);

        /**
         * @brief Reset the internal state of the estimator.
         */
        void reset();
};

#endif