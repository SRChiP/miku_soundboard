/*
Miku Soundboard 

The Miku soundboard is a sketch that visualizes analog audio in a VU Meter
*/

#include "Arduino.h"
#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 21

// Since this a neopixel, we only need the data pin.
#define DATA_PIN 7

// Analog pin to read the mic's output
#define MIC_PIN A0

// Array of LEDs
CRGB leds[NUM_LEDS];

int input, i, level;

void setup () {
    // Enable serial for debugging purposes
    Serial.begin(9600);
    Serial.println("\n\nMiku Soundboard\nSerial Started");

    Serial.println("Configuring...");
    Serial.print("LEDs: DATA_PIN = "); Serial.print(DATA_PIN); Serial.print("; NUM_LEDS = "); Serial.println(NUM_LEDS);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    Serial.print("MIC : MIC_PIN = "); Serial.println(MIC_PIN);
    pinMode(MIC_PIN, INPUT);
    Serial.print("\n");

}

void loop () {
    // Use analog pin to read audio input
    input = analogRead(MIC_PIN);
    Serial.print("A0 = ");
    Serial.print(input);

    // Turn all LEDs off
    for (i = 0; i < NUM_LEDS; i ++) {
        leds[i] = CRGB::Black;
    }

    level = int(float(input) / 512 * NUM_LEDS);
    // Make sure the level is limited by the number of leds
    level = min(NUM_LEDS, level);
    Serial.print("  level = ");
    Serial.print(level);

    for (i = 0; i < level; i ++) {
        leds[i] = CRGB::Blue;
    }

    FastLED.show();

    Serial.print("\n");
    delay(200);

}