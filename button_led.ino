#include <FastLED.h>

#define NUM_LEDS 150
#define DATA_PIN 7

int brightness = 0;
float hue = 0;
float inc = 0;
float hueInc = .3;
int mode = 0;
int recent = 0;
int last = 0;

const int buttonPin = 4;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(255);
  bootUp();
}

void bootUp() {

  while (brightness < 254) {
    LEDS.setBrightness(brightness);
    loop();
    brightness += 1;
    delay(5);
  }

}

void checkButton() {
  buttonState = digitalRead(buttonPin);
  if (buttonState != recent) {
    if (buttonState == 1) {
      Serial.println("CHANGED");
      recent = buttonState;

      if (mode == 0) {
        mode = 1;
      }
      else {
        mode = 0;
      }
    }
    else {
      recent = 0; // reset for letting go of button
    }
  }
}

void loop() {

  checkButton();

  if (mode == 0) { // normal fade
    hue = inc;
    inc += .05;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue, 255, 255);
      hue += .5; // incremenration of hues in the strip
    }
    FastLED.show();
  }

  else { // strobe
    if (last == 0) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(255, 255, 255);
      }
      last = 1;

    }
    else {
      last = 0;
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(0, 0, 0);
      }
    }
    FastLED.show();
    delay(35);
  }

}
