// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root or https://opensource.org/licenses/MIT

/**
 * @file counters_overflow_tests.cpp
 * @brief Test cases for verifying overflow handling in timing and pulse counter calculations.
 * C++11 standard is used. Compile this file with a C++11 compatible compiler like g++ or clang++.
 */

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <limits>
#include <bitset>

using namespace std;

// ============================================================================
// Test 1: Unsigned Long Timing Overflow
// ============================================================================
void testUnsignedTimingOverflow() {
    cout << "\n=== Test 1: Unsigned Long Timing Overflow ===" << endl;
    cout << "Testing micros() overflow handling with unsigned long arithmetic\n" << endl;

    // Test Case 1.1: Normal operation (no overflow)
    {
        unsigned long prevTime = 1000000UL;  // 1 second
        unsigned long currTime = 1100000UL;  // 1.1 seconds
        unsigned long deltaTimeMicros = currTime - prevTime;
        
        cout << "Case 1.1: Normal operation" << endl;
        cout << "  prevTime = " << prevTime << " us" << endl;
        cout << "  currTime = " << currTime << " us" << endl;
        cout << "  deltaTime = " << deltaTimeMicros << " us (" << (deltaTimeMicros / 1000.0) << " ms)" << endl;
        cout << "  Expected: 100000 us (100 ms)" << endl;
        cout << "  Result: " << (deltaTimeMicros == 100000UL ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 1.2: Overflow at maximum value
    {
        unsigned long prevTime = 4294967200UL;  // Near ULONG_MAX (4,294,967,295)
        unsigned long currTime = 100UL;          // Wrapped around to small value
        unsigned long deltaTimeMicros = currTime - prevTime;
        
        cout << "Case 1.2: Overflow near ULONG_MAX" << endl;
        cout << "  prevTime = " << prevTime << " us" << endl;
        cout << "  currTime = " << currTime << " us (after overflow)" << endl;
        cout << "  deltaTime = " << deltaTimeMicros << " us" << endl;
        
        // Manual calculation: (ULONG_MAX - prevTime + 1) + currTime
        unsigned long expected = (ULONG_MAX - prevTime) + currTime + 1;
        cout << "  Expected: " << expected << " us" << endl;
        cout << "  Result: " << (deltaTimeMicros == expected ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 1.3: Exact overflow point
    {
        unsigned long prevTime = ULONG_MAX;  // Maximum value
        unsigned long currTime = 0UL;        // Wrapped to zero
        unsigned long deltaTimeMicros = currTime - prevTime;
        
        cout << "Case 1.3: Exact overflow at ULONG_MAX" << endl;
        cout << "  prevTime = " << prevTime << " us (ULONG_MAX)" << endl;
        cout << "  currTime = " << currTime << " us" << endl;
        cout << "  deltaTime = " << deltaTimeMicros << " us" << endl;
        cout << "  Expected: 1 us" << endl;
        cout << "  Result: " << (deltaTimeMicros == 1UL ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 1.4: What would happen with signed long (wrong approach)
    {
        long prevTime = 2147483600L;   // Near LONG_MAX
        long currTime = 100L;           // After overflow (would be negative)
        long deltaTime = currTime - prevTime;
        
        cout << "Case 1.4: WRONG approach with signed long" << endl;
        cout << "  prevTime = " << prevTime << " us" << endl;
        cout << "  currTime = " << currTime << " us" << endl;
        cout << "  deltaTime = " << deltaTime << " us" << endl;
        cout << "  This gives HUGE NEGATIVE value: " << deltaTime << " us" << endl;
        cout << "  Result: FAIL ✗ (demonstrates why unsigned is needed)\n" << endl;
    }
}

// ============================================================================
// Test 2: Signed Int Pulse Counter Overflow
// ============================================================================
void testSignedPulseCounterOverflow() {
    cout << "\n=== Test 2: Signed Int Pulse Counter Overflow ===" << endl;
    cout << "Testing pulse counter overflow with signed int arithmetic\n" << endl;

    // Test Case 2.1: Normal forward motion
    {
        int prevPulses = 1000;
        int currPulses = 1100;
        int pulseDiff = currPulses - prevPulses;
        
        cout << "Case 2.1: Normal forward motion" << endl;
        cout << "  prevPulses = " << prevPulses << endl;
        cout << "  currPulses = " << currPulses << endl;
        cout << "  pulseDiff = " << pulseDiff << endl;
        cout << "  Expected: 100 pulses (forward)" << endl;
        cout << "  Result: " << (pulseDiff == 100 ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 2.2: Reverse motion (negative velocity)
    {
        int prevPulses = 1100;
        int currPulses = 1000;
        int pulseDiff = currPulses - prevPulses;
        
        cout << "Case 2.2: Reverse motion" << endl;
        cout << "  prevPulses = " << prevPulses << endl;
        cout << "  currPulses = " << currPulses << endl;
        cout << "  pulseDiff = " << pulseDiff << endl;
        cout << "  Expected: -100 pulses (backward)" << endl;
        cout << "  Result: " << (pulseDiff == -100 ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 2.3: Small overflow (16-bit int simulation)
    {
        int16_t prevPulses = 32760;   // Near INT16_MAX (32767)
        int16_t currPulses = -32728;  // After overflow
        int16_t pulseDiff = currPulses - prevPulses;
        
        cout << "Case 2.3: Overflow with 16-bit int" << endl;
        cout << "  prevPulses = " << prevPulses << " (near INT16_MAX)" << endl;
        cout << "  currPulses = " << currPulses << " (after overflow)" << endl;
        cout << "  pulseDiff = " << pulseDiff << endl;
        cout << "  Binary representation:" << endl;
        cout << "    prevPulses: " << bitset<16>(prevPulses) << endl;
        cout << "    currPulses: " << bitset<16>(currPulses) << endl;
        cout << "    pulseDiff:  " << bitset<16>(pulseDiff) << endl;
        
        // The actual number of pulses that occurred
        // From 32760 to 32767 (7 pulses) + wrap + from -32768 to -32728 (40 pulses) = 48 pulses
        int16_t expected = 48;
        cout << "  Expected: " << expected << " pulses" << endl;
        cout << "  Result: " << (pulseDiff == expected ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 2.4: Exact overflow boundary (16-bit)
    {
        int16_t prevPulses = 32767;   // INT16_MAX
        int16_t currPulses = -32768;  // INT16_MIN (overflow)
        int16_t pulseDiff = currPulses - prevPulses;
        
        cout << "Case 2.4: Exact overflow at INT16_MAX" << endl;
        cout << "  prevPulses = " << prevPulses << " (INT16_MAX)" << endl;
        cout << "  currPulses = " << currPulses << " (INT16_MIN)" << endl;
        cout << "  pulseDiff = " << pulseDiff << endl;
        cout << "  Expected: 1 pulse (one step forward causes overflow)" << endl;
        cout << "  Result: " << (pulseDiff == 1 ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 2.5: Multiple wraps forward
    {
        int16_t prevPulses = 32700;
        int16_t currPulses = -32636;  // 200 pulses after overflow
        int16_t pulseDiff = currPulses - prevPulses;
        
        cout << "Case 2.5: 200 pulses across overflow boundary" << endl;
        cout << "  prevPulses = " << prevPulses << endl;
        cout << "  currPulses = " << currPulses << endl;
        cout << "  pulseDiff = " << pulseDiff << endl;
        cout << "  Calculation: (32767-32700) + 1 + (-32636-(-32768)) = 67 + 1 + 132 = 200" << endl;
        cout << "  Expected: 200 pulses" << endl;
        cout << "  Result: " << (pulseDiff == 200 ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 2.6: Backward across overflow boundary
    {
        int16_t prevPulses = -32668;  // Already wrapped
        int16_t currPulses = 32700;   // Went backward, wrapped back
        int16_t pulseDiff = currPulses - prevPulses;
        
        cout << "Case 2.6: Backward motion across overflow boundary" << endl;
        cout << "  prevPulses = " << prevPulses << endl;
        cout << "  currPulses = " << currPulses << endl;
        cout << "  pulseDiff = " << pulseDiff << endl;
        // From -32668 backward to -32768 (100) + wrap to 32767 (1) + to 32700 (67) = -168 total
        int16_t expected = -168;
        cout << "  Calculation: -[(−32768−(−32668)) + 1 + (32767−32700)] = -(100 + 1 + 67) = -168" << endl;
        cout << "  Expected: " << expected << " pulses (backward)" << endl;
        cout << "  Result: " << (pulseDiff == expected ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }
}

// ============================================================================
// Test 3: Combined Real-World Scenario
// ============================================================================
void testRealWorldScenario() {
    cout << "\n=== Test 3: Real-World Scenarios ===" << endl;
    cout << "Simulating actual SpeedEstimator behavior\n" << endl;

    // Constants for a typical motor
    const float PPR = 374.0f;        // Pulses per revolution
    const float GEAR_RATIO = 30.0f;  // 30:1 gear ratio

    auto calculateRPM = [&](int pulseDiff, float deltaTime) -> float {
        float velocity = (float)pulseDiff / deltaTime;
        return (velocity / PPR) * (1.0f / GEAR_RATIO) * 60.0f;
    };

    // Test Case 3.1: Motor at steady speed
    {
        int prevPulses = 10000;
        int currPulses = 10374;  // One revolution
        float deltaTime = 1.0f;   // 1 second
        int pulseDiff = currPulses - prevPulses;
        float rpm = calculateRPM(pulseDiff, deltaTime);
        
        cout << "Case 3.1: Motor at steady speed" << endl;
        cout << "  Time interval: " << deltaTime << " seconds" << endl;
        cout << "  Pulse difference: " << pulseDiff << " pulses" << endl;
        cout << "  Calculated RPM: " << rpm << endl;
        cout << "  Expected: ~2 RPM (one output shaft revolution/sec)" << endl;
        cout << "  Result: " << (abs(rpm - 2.0f) < 0.1f ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 3.2: Motor slowing down
    {
        int prevPulses = 10000;
        int currPulses = 10187;  // Half revolution
        float deltaTime = 1.0f;
        int pulseDiff = currPulses - prevPulses;
        float rpm = calculateRPM(pulseDiff, deltaTime);
        
        cout << "Case 3.2: Motor slowing down" << endl;
        cout << "  Time interval: " << deltaTime << " seconds" << endl;
        cout << "  Pulse difference: " << pulseDiff << " pulses" << endl;
        cout << "  Calculated RPM: " << rpm << endl;
        cout << "  Expected: ~1 RPM (half revolution)" << endl;
        cout << "  Result: " << (abs(rpm - 1.0f) < 0.1f ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 3.3: Counter overflow during operation (16-bit simulation)
    {
        int16_t prevPulses = 32700;
        int16_t currPulses = -32668;  // 100 pulses after overflow
        float deltaTime = 0.1f;        // 100ms
        int16_t pulseDiff = currPulses - prevPulses;
        float rpm = calculateRPM(pulseDiff, deltaTime);
        
        cout << "Case 3.3: Counter overflow during high-speed operation" << endl;
        cout << "  prevPulses: " << prevPulses << endl;
        cout << "  currPulses: " << currPulses << " (after overflow)" << endl;
        cout << "  Time interval: " << deltaTime << " seconds" << endl;
        cout << "  Pulse difference: " << pulseDiff << " pulses" << endl;
        cout << "  Calculated RPM: " << rpm << endl;
        cout << "  Note: Should be positive RPM (forward motion)" << endl;
        cout << "  Result: " << (rpm > 0 && rpm < 100 ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }

    // Test Case 3.4: Reverse direction
    {
        int prevPulses = 10374;
        int currPulses = 10000;  // One revolution backward
        float deltaTime = 1.0f;
        int pulseDiff = currPulses - prevPulses;
        float rpm = calculateRPM(pulseDiff, deltaTime);
        
        cout << "Case 3.4: Motor running in reverse" << endl;
        cout << "  Time interval: " << deltaTime << " seconds" << endl;
        cout << "  Pulse difference: " << pulseDiff << " pulses" << endl;
        cout << "  Calculated RPM: " << rpm << endl;
        cout << "  Expected: ~-2 RPM (negative = reverse)" << endl;
        cout << "  Result: " << (abs(rpm + 2.0f) < 0.1f ? "PASS ✓" : "FAIL ✗") << "\n" << endl;
    }
}

// ============================================================================
// Main Test Runner
// ============================================================================
int main() {
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║  Overflow Handling Test Suite for SpeedEstimator          ║" << endl;
    cout << "║  Testing timing and pulse counter overflow scenarios      ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;

    cout << "\nSystem information:" << endl;
    cout << "  sizeof(int) = " << sizeof(int) << " bytes" << endl;
    cout << "  sizeof(long) = " << sizeof(long) << " bytes" << endl;
    cout << "  sizeof(int16_t) = " << sizeof(int16_t) << " bytes" << endl;
    cout << "  INT16_MAX = " << INT16_MAX << endl;
    cout << "  INT16_MIN = " << INT16_MIN << endl;
    cout << "  ULONG_MAX = " << ULONG_MAX << endl;

    testUnsignedTimingOverflow();
    testSignedPulseCounterOverflow();
    testRealWorldScenario();

    cout << "\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║  All tests completed!                                      ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝\n" << endl;

    return 0;
}
