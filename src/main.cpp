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
#define DATA_PIN 8

// Analog pin to read the mic's output
#define MIC_PIN A0

//Enable or disable DEBUG
const bool DEBUG = true;

// Array of LEDs
CRGB leds[NUM_LEDS];

const int STATIC_LEDS[] = {4, 6, 8, 9, 12, 13, 16, 18, 19, 22, 23, 25, 26, 28, 29};
const CRGB STATIC_COLOUR = CRGB::Yellow;

const int LEVEL_LED_GROUPS[4][4] = {
                               {1, 5, 11, 15},  //0
                               {3, 7, 14, 20},  //1
                               {2, 10, 17, 16},  //2
                               {21, 24, 27, 30} }; //3
const CRGB LEVEL_LED_COLOUR = CRGB::Violet;
const int NUMBER_OF_LEVELS = sizeof(LEVEL_LED_GROUPS)/sizeof(LEVEL_LED_GROUPS[0]);

int input, i, j, level;

int randomNumber() {
    // Return a random LED
    int rnd = random(NUMBER_OF_LEVELS);
    Serial.print("RND = ");
    Serial.print(rnd);
    return rnd;
}

void updateLevelGroup(int number, CRGB colour) {
    number = min(NUMBER_OF_LEVELS, number);
    Serial.print("Number of Levels: ");Serial.print(NUMBER_OF_LEVELS);Serial.print(", current Level: ");Serial.println(number);
    int numberOfElements;

    for (i = 0; i < NUMBER_OF_LEVELS; i ++) {
        numberOfElements = sizeof(LEVEL_LED_GROUPS[i])/sizeof(int);
        for (j = 0; j < numberOfElements; j++) {
            if (number >= i + 1) {
                Serial.print("L");Serial.print(LEVEL_LED_GROUPS[i][j]);Serial.println("colour");
                leds[LEVEL_LED_GROUPS[i][j]] = colour;
            }
            else {
                Serial.print("L");Serial.print(LEVEL_LED_GROUPS[i][j]);Serial.println("black");
                leds[LEVEL_LED_GROUPS[i][j]] = CRGB::Black;
            }
        }
    }
}

int getAudioLevel() {
    // Use analog pin to read audio input
    input = analogRead(MIC_PIN);
    Serial.print(MIC_PIN);Serial.print(" = ");Serial.print(input);

    return int(float(input) / 512 * NUM_LEDS);
}

void setup () {
    // Enable serial for debugging purposes
    Serial.begin(9600);
    Serial.println(F("\n\nMiku Soundboard\nSerial Started"));

    Serial.println(F("Configuring..."));
    Serial.print(F("LEDs: DATA_PIN = ")); Serial.print(DATA_PIN); Serial.print(F("; NUM_LEDS = ")); Serial.println(NUM_LEDS);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    Serial.print(F("MIC : MIC_PIN = ")); Serial.println(MIC_PIN);
    pinMode(MIC_PIN, INPUT);
    Serial.print("\n"); 
    
    
    Serial.println(F("Setting static colours"));
    const int staticCount = sizeof(STATIC_LEDS)/sizeof(int);
    Serial.print(F("Static count :"));
    Serial.print(staticCount);
    Serial.print(F("\nLEDs: "));
    for (i = 0; i < staticCount; i++) {
        Serial.print(STATIC_LEDS[i]);Serial.print(" ");
        leds[STATIC_LEDS[i]] = STATIC_COLOUR;
    };
    Serial.println();

    if (DEBUG) {
        Serial.println(F("TEST MODE -----"));
        FastLED.delay(2000);
        FastLED.show();
        Serial.println(F("Level 0"));
        updateLevelGroup(0, CRGB::Pink);
        FastLED.show();
        FastLED.delay(1000);
        Serial.println(F("Level 1"));
        updateLevelGroup(1, CRGB::Red);
        FastLED.show();
        FastLED.delay(1000);
        Serial.println(F("Level 2"));
        updateLevelGroup(2, CRGB::Blue);
        FastLED.show();
        FastLED.delay(1000);
        Serial.println(F("Level 3"));
        updateLevelGroup(3, CRGB::White);
        FastLED.show();
        FastLED.delay(1000);
        Serial.println(F("Level 4"));
        updateLevelGroup(4, CRGB::Green);
        FastLED.show();
        FastLED.delay(1000);
        Serial.println(F("TEST MODE END----"));
    }
}

void loop () {
    
   
    if (DEBUG) {
        level = randomNumber();
    }
    else {
        level = getAudioLevel();
    }

 
    // Make sure the level is limited by the number of leds
    level = min(NUM_LEDS, level);
    Serial.print("  level = ");
    Serial.print(level);

    updateLevelGroup(level, LEVEL_LED_COLOUR);

    FastLED.show();

    Serial.print("\n");
    delay(200);

}