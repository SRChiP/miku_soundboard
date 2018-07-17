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
const bool DEBUG = false;

// Array of LEDs
CRGB leds[NUM_LEDS];

const int STATIC_LEDS[] = {0, 1, 2, 27, 28, 29, 30};
const CRGB STATIC_COLOUR = CRGB::Red;

const int LEVEL_LED_GROUPS[8][3] = {
                               { 3,  4,  5},  //0
                               { 6,  7,  8},  //1
                               { 9, 10, 11},  //2
                               {12, 13, 14},  //3
                               {15, 16, 17},  //4
                               {18, 19, 20},  //5
                               {21, 22, 23},  //6
                               {24, 25, 26} };//7 
const CRGB LEVEL_LED_COLOUR = CRGB::LightCyan;
const int NUMBER_OF_LEVELS = sizeof(LEVEL_LED_GROUPS)/sizeof(LEVEL_LED_GROUPS[0]);

// Variables for LED display
int i, j;

// Audio related variables
const int sampleWindow = 100; // Sample window width in mS (100 mS = 40Hz)
int corrected_level;
unsigned int sample, peakToPeak, signalMax, signalMin;
unsigned long startMillis;  // Start of sample window
double volts, level;


void updateLevelGroup(int number, CRGB colour) {
    int numberOfElements;

    for (i = 0; i < NUMBER_OF_LEVELS; i ++) {
        numberOfElements = sizeof(LEVEL_LED_GROUPS[i])/sizeof(int);
        for (j = 0; j < numberOfElements; j++) {
            if (number >= i + 1) {
                if (DEBUG) {Serial.print("L");Serial.print(LEVEL_LED_GROUPS[i][j]);Serial.println("colour");}
                leds[LEVEL_LED_GROUPS[i][j]] = colour;
            }
            else {
                if (DEBUG) {Serial.print("L");Serial.print(LEVEL_LED_GROUPS[i][j]);Serial.println("black");}
                leds[LEVEL_LED_GROUPS[i][j]] = CRGB::Black;
            }
        }
    }
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
    Serial.print(F("\nNumber of Steps for Visualizer :"));
    Serial.println(NUMBER_OF_LEVELS);

    if (DEBUG) {
        Serial.println(F("TEST MODE -----"));
        FastLED.delay(2000);
        FastLED.show();
        for (i = 0; i < NUMBER_OF_LEVELS; i ++) {
            Serial.print("Level ");Serial.println(i);
            updateLevelGroup(i, CRGB::WhiteSmoke);
            FastLED.show();
            FastLED.delay(1000);
        }
        Serial.println(F("TEST MODE END----"));
    }
}

void loop () {

    // Calculate audio level
    startMillis = millis();  // Start of sample window
    peakToPeak = 0;   // peak-to-peak level
 
    signalMax = 0;
    signalMin = 1024;
 
    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
        sample = analogRead(MIC_PIN);
        if (sample < 1024)  // toss out spurious readings
        {
            if (sample > signalMax)
            {
                signalMax = sample;  // save just the max levels
            }
            else if (sample < signalMin)
            {
                signalMin = sample;  // save just the min levels
            }
        }
    }
    Serial.print("max=");Serial.print((signalMax * 5.0) / 1024);Serial.print(" min=");Serial.print((signalMin * 5.0) / 1024);
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    volts = (peakToPeak * 5.0) / 1024;  // convert to volts
 
    /* 
    peak-to-peak data
    0.18 idle min (0.04 sometimes)
    1.4 normal sound we need
    2.47 max 
    */
    Serial.print(" volts=");Serial.print(volts); // Peak-to-Peak volts
   
    level = (max(0.0, volts - 0.18) / (2.47 - 0.18)) * NUMBER_OF_LEVELS;
 
    // Round up rather than truncate to show more lights
    corrected_level = (int) round(level) ;
    // Make sure the level is limited by the number of levels that can be displayed
    corrected_level = min(NUMBER_OF_LEVELS, corrected_level);
    Serial.print(F("  level = "));
    Serial.print(corrected_level);
    Serial.print(F(" ("));
    Serial.print(level);
    Serial.print(F(" actual)"));
    

    updateLevelGroup(corrected_level, LEVEL_LED_COLOUR);

    FastLED.show();

    Serial.print(F("\n"));
    // delay(200);

}