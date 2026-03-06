// ==========================================================
// Solar Still Automation Controller
// Author: Solar Still Project Team
// Platform: Arduino Nano
//
// Function:
// 1. Monitor water level using an analog water level sensor.
// 2. Automatically refill water using Pump 1 when level is low.
// 3. Periodically drain brine water using Pump 2.
// ==========================================================

// Include Arduino core library
#include <Arduino.h>

// ---------------------------
// Pin Definitions
// ---------------------------

// Analog pin connected to water level sensor
const int waterLevelSensorPin = A0;

// Digital pins connected to relay module
const int pump1RelayPin = 9;   // Relay controlling refill pump
const int pump2RelayPin = 10;  // Relay controlling drainage pump


// ==========================================================
// Setup Function
// Runs once when Arduino starts
// ==========================================================
void setup()
{
    // Configure relay pins as output
    pinMode(pump1RelayPin, OUTPUT);
    pinMode(pump2RelayPin, OUTPUT);

    // Initialize serial communication for debugging
    Serial.begin(9600);
}


// ==========================================================
// Loop Function
// Runs continuously after setup()
// ==========================================================
void loop()
{
    // ------------------------------------------------------
    // Read water level sensor value
    // ------------------------------------------------------
    int waterLevel = analogRead(waterLevelSensorPin);

    // Print sensor value to Serial Monitor for debugging
    Serial.print("Water level: ");
    Serial.println(waterLevel);


    // ------------------------------------------------------
    // Time management using Arduino millis()
    // Pump 2 runs periodically to drain brine water
    // ------------------------------------------------------

    // Total cycle = 84600 seconds (~23.5 hours)
    // Convert to milliseconds
    unsigned long currentTime = millis() % 84600000;


    // ------------------------------------------------------
    // Pump 2 operation period (first 30 minutes of cycle)
    // ------------------------------------------------------
    if (currentTime >= 0 && currentTime < 1800000)
    {
        // Activate drainage pump
        digitalWrite(pump2RelayPin, HIGH);

        // Ensure refill pump is OFF during drainage
        digitalWrite(pump1RelayPin, LOW);

        Serial.println("Pump 2 is running. Pump 1 turned off.");

        // Run pump for 30 minutes
        delay(1800000);
    }

    else
    {
        // Stop drainage pump after its scheduled time
        digitalWrite(pump2RelayPin, LOW);


        // --------------------------------------------------
        // Water level control logic
        // --------------------------------------------------

        // If water level is high
        if (waterLevel > 500)
        {
            // Turn OFF refill pump
            digitalWrite(pump1RelayPin, LOW);

            Serial.println("Water level is high. Pump 1 turned off.");
        }

        // If water level is low
        else
        {
            // Turn ON refill pump
            digitalWrite(pump1RelayPin, HIGH);

            Serial.println("Water level is low or normal. Pump 1 turned on.");
        }
    }


    // ------------------------------------------------------
    // Reset cycle after ~24 hours
    // ------------------------------------------------------
    if (currentTime >= 84598500)
    {
        // Ensure drainage pump is OFF
        digitalWrite(pump2RelayPin, LOW);

        // Wait remaining time before restarting cycle
        delay(84600000 - currentTime);
    }


    // ------------------------------------------------------
    // Stability delay
    // Prevents excessive sensor readings
    // ------------------------------------------------------
    delay(1000);  // Check system every second
}
