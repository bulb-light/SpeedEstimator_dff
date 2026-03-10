// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root or https://opensource.org/licenses/MIT

/**
 * @file counters_overflow_tests_arduino.cpp
 * @brief Test cases for verifying overflow handling in timing and pulse counter calculations on Arduino hardware.
 */

#include <Arduino.h>
#include <limits.h> // Include limits.h for ULONG_MAX

// ============================================================================
// Test 1: Unsigned Long Timing Overflow
// ============================================================================
void testUnsignedTimingOverflow() {
    Serial.println("\n=== Test 1: Unsigned Long Timing Overflow ===");
    Serial.println("Testing micros() overflow handling with unsigned long arithmetic\n");

    // Test Case 1.1: Normal operation (no overflow)
    {
        unsigned long prevTime = 1000000UL;  // 1 second
        unsigned long currTime = 1100000UL;  // 1.1 seconds
        unsigned long deltaTimeMicros = currTime - prevTime;

        Serial.println("Case 1.1: Normal operation");
        Serial.print("  prevTime = "); Serial.print(prevTime); Serial.println(" us");
        Serial.print("  currTime = "); Serial.print(currTime); Serial.println(" us");
        Serial.print("  deltaTime = "); Serial.print(deltaTimeMicros); Serial.print(" us (");
        Serial.print(deltaTimeMicros / 1000.0); Serial.println(" ms)");
        Serial.println("  Expected: 100000 us (100 ms)");
        Serial.println(deltaTimeMicros == 100000UL ? "  Result: PASS ✓\n" : "  Result: FAIL ✗\n");
    }

    // Test Case 1.2: Overflow at maximum value
    {
        unsigned long prevTime = ULONG_MAX - 95;  // Near ULONG_MAX (4,294,967,295)
        unsigned long currTime = 100UL;          // Wrapped around to small value
        unsigned long deltaTimeMicros = currTime - prevTime;

        Serial.println("Case 1.2: Overflow near ULONG_MAX");
        Serial.print("  prevTime = "); Serial.print(prevTime); Serial.println(" us");
        Serial.print("  currTime = "); Serial.print(currTime); Serial.println(" us (after overflow)");
        Serial.print("  deltaTime = "); Serial.print(deltaTimeMicros); Serial.println(" us");

        unsigned long expected = (ULONG_MAX - prevTime) + currTime + 1;
        Serial.print("  Expected: "); Serial.print(expected); Serial.println(" us");
        Serial.println(deltaTimeMicros == expected ? "  Result: PASS ✓\n" : "  Result: FAIL ✗\n");
    }

    // Test Case 1.3: Exact overflow point
    {
        unsigned long prevTime = ULONG_MAX;  // Maximum value
        unsigned long currTime = 0UL;        // Wrapped to zero
        unsigned long deltaTimeMicros = currTime - prevTime;

        Serial.println("Case 1.3: Exact overflow at ULONG_MAX");
        Serial.print("  prevTime = "); Serial.print(prevTime); Serial.println(" us (ULONG_MAX)");
        Serial.print("  currTime = "); Serial.print(currTime); Serial.println(" us");
        Serial.print("  deltaTime = "); Serial.print(deltaTimeMicros); Serial.println(" us");
        Serial.println("  Expected: 1 us");
        Serial.println(deltaTimeMicros == 1UL ? "  Result: PASS ✓\n" : "  Result: FAIL ✗\n");
    }

    // Test Case 1.4: What would happen with signed long (wrong approach)
    {
        long prevTime = LONG_MAX - 47;   // Near LONG_MAX (2,147,483,647)
        long currTime = 100L;           // After overflow (would be negative)
        long deltaTime = currTime - prevTime;

        Serial.println("Case 1.4: WRONG approach with signed long");
        Serial.print("  prevTime = "); Serial.print(prevTime); Serial.println(" us");
        Serial.print("  currTime = "); Serial.print(currTime); Serial.println(" us");
        Serial.print("  deltaTime = "); Serial.print(deltaTime); Serial.println(" us");
        Serial.print("  This gives HUGE NEGATIVE value: "); Serial.print(deltaTime); Serial.println(" us");
        Serial.println("  Result: FAIL ✗ (demonstrates why unsigned is needed)\n");
    }
}

// ============================================================================
// Test 2: Pulse Counter Overflow
// ============================================================================
void testPulseCounterOverflow() {
    Serial.println("\n=== Test 2: Pulse Counter Overflow ===");
    Serial.println("Testing pulse counter overflow\n");

    // Test Case 2.1: Normal forward motion
    {
        long prevPulses = 1000;
        long currPulses = 1100;
        long pulseDiff = currPulses - prevPulses;

        Serial.println("Case 2.1: Normal forward motion");
        Serial.print("  prevPulses = "); Serial.println(prevPulses);
        Serial.print("  currPulses = "); Serial.println(currPulses);
        Serial.print("  pulseDiff = "); Serial.println(pulseDiff);
        Serial.println(pulseDiff == 100 ? "  Result: PASS ✓\n" : "  Result: FAIL ✗\n");
    }

    // Test Case 2.2: Reverse motion (negative velocity)
    {
        long prevPulses = 1100;
        long currPulses = 1000;
        long pulseDiff = currPulses - prevPulses;

        Serial.println("Case 2.2: Reverse motion");
        Serial.print("  prevPulses = "); Serial.println(prevPulses);
        Serial.print("  currPulses = "); Serial.println(currPulses);
        Serial.print("  pulseDiff = "); Serial.println(pulseDiff);
        Serial.println(pulseDiff == -100 ? "  Result: PASS ✓\n" : "  Result: FAIL ✗\n");
    }

    // Test Case 2.3: Small overflow scenario
    {
        long prevPulses = LONG_MAX - 7;   // Near maximum value for long (2,147,483,647)
        long currPulses = -(LONG_MAX - 39);  // After overflow
        long pulseDiff = currPulses - prevPulses;

        Serial.println("Case 2.3: Small overflow scenario with 32-bit long");
        Serial.print("  prevPulses = "); Serial.println(prevPulses);
        Serial.print("  currPulses = "); Serial.println(currPulses);
        Serial.print("  pulseDiff = "); Serial.println(pulseDiff);

        // The actual number of pulses that occurred
        // From 2,147,483,640 to 2,147,483,647 (7 pulses) + wrap + from -2,147,483,648 to -2,147,483,608 (40 pulses) = 48 pulses
        long expected = 48;
        Serial.print("  Expected: "); Serial.print(expected); Serial.println(" pulses");
        Serial.println(pulseDiff == expected ? "  Result: PASS ✓\n" : "  Result: FAIL ✗\n");
    }
    
    // Test Case 2.4: Exact overflow boundary
    {
        long prevPulses = LONG_MAX;   // Maximum value for long (2,147,483,647)
        long currPulses = LONG_MIN;   // Minimum value for long (-2,147,483,648)
        long pulseDiff = currPulses - prevPulses;

        Serial.println("Case 2.4: Exact overflow boundary at LONG_MAX");
        Serial.print("  prevPulses = "); Serial.println(prevPulses);
        Serial.print("  currPulses = "); Serial.println(currPulses);
        Serial.print("  pulseDiff = "); Serial.println(pulseDiff);
        Serial.println(pulseDiff == 1 ? "  Result: PASS ✓\n" : "  Result: FAIL ✗\n");
    }

    // Test Case 2.5: Multiple wraps forward
    {
        long prevPulses = LONG_MAX - 100;   // Near maximum value for long
        long currPulses = LONG_MIN + 100;   // After overflow with 200 pulses
        long pulseDiff = currPulses - prevPulses;

        Serial.println("Case 2.5: Multiple wraps forward");
        Serial.print("  prevPulses = "); Serial.println(prevPulses);
        Serial.print("  currPulses = "); Serial.println(currPulses);
        Serial.print("  pulseDiff = "); Serial.println(pulseDiff);

        // The actual number of pulses that occurred
        // From (LONG_MAX - 100) to LONG_MAX (100 pulses) + wrap + from LONG_MIN to (LONG_MIN + 100) (100 pulses) = 201 pulses
        long expected = 201;
        Serial.print("  Expected: "); Serial.print(expected); Serial.println(" pulses");
        Serial.println(pulseDiff == expected ? "  Result: PASS ✓\n" : "  Result: FAIL ✗\n");
    }

    // Test Case 2.6: Backward across overflow boundary
    {
        long prevPulses = LONG_MIN + 50;   // Near minimum value for long
        long currPulses = LONG_MAX - 50;   // After overflow with 100 pulses backward
        long pulseDiff = currPulses - prevPulses;

        Serial.println("Case 2.6: Backward across overflow boundary");
        Serial.print("  prevPulses = "); Serial.println(prevPulses);
        Serial.print("  currPulses = "); Serial.println(currPulses);
        Serial.print("  pulseDiff = "); Serial.println(pulseDiff);

        // The actual number of pulses that occurred
        // From (LONG_MIN + 50) to LONG_MIN (50 pulses backward) + wrap + from LONG_MAX to (LONG_MAX - 50) (50 pulses backward) = -101 pulses
        long expected = -101;
        Serial.print("  Expected: "); Serial.print(expected); Serial.println(" pulses");
        Serial.println(pulseDiff == expected ? "  Result: PASS ✓\n" : "  Result: FAIL ✗\n");
    }
}

// ============================================================================
// Main Test Runner
// ============================================================================
void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("╔════════════════════════════════════════════════════════════╗");
    Serial.println("║  Overflow Handling Test Suite for Arduino                ║");
    Serial.println("╚════════════════════════════════════════════════════════════╝\n");

    testUnsignedTimingOverflow();
    testPulseCounterOverflow();

    Serial.println("\n╔════════════════════════════════════════════════════════════╗");
    Serial.println("║  All tests completed!                                      ║");
    Serial.println("╚════════════════════════════════════════════════════════════╝\n");
}

void loop() {
    // Do nothing
}