/*
Miku Soundboard 

The Miku soundboard is a sketch that visualizes analog audio in a VU Meter
*/

#include "Arduino.h"
#include <avr/pgmspace.h>
#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 31

// Since this a neopixel, we only need the data pin.
#define DATA_PIN 7

// Analog pin to read the mic's output
#define MIC_PIN A0

//Enable or disable DEBUG
# define DEBUG true

// Array of LEDs
CRGB leds[NUM_LEDS];

int input, i, level;

int randomNumber() {
    // Return a random LED
    int rnd = random(NUM_LEDS);
    Serial.print("RND = ");
    Serial.print(rnd);
    return rnd;
}

int getAudioLevel() {
    // Use analog pin to read audio input
    input = analogRead(MIC_PIN);
    Serial.print("A0 = ");
    Serial.print(input);

    return int(float(input) / 512 * NUM_LEDS);
}

void setup () {
    // Enable serial for debugging purposes
    Serial.begin(9600);
    Serial.println(F("\n\nMiku Soundboard\nSerial Started"));

    Serial.println(F("Configuring..."));
    Serial.print("LEDs: DATA_PIN = "); Serial.print(DATA_PIN); Serial.print("; NUM_LEDS = "); Serial.println(NUM_LEDS);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    Serial.print("MIC : MIC_PIN = "); Serial.println(MIC_PIN);
    pinMode(MIC_PIN, INPUT);
    Serial.print("\n"); 

}

void loop () {

   
    if (DEBUG) {
        level = randomNumber();
    }
    else {
        level = getAudioLevel();
    }

    // Turn all LEDs off
    for (i = 0; i < NUM_LEDS; i ++) {
        leds[i] = CRGB::Black;
    }

    // Make sure the level is limited by the number of leds
    level = min(NUM_LEDS, level);
    Serial.print("  level = ");
    Serial.print(level);

    for (i = 0; i < level; i ++) {
        leds[i] = CRGB::Red;
    }

    FastLED.show();

    Serial.print("\n");
    delay(200);

}