#include <FastLED.h>

#define NUM_LEDS 150
#define DATA_PIN 7

int brightness = 0;
float hue = 0;
float inc = 0;
float hueInc = .3;
float incSpeed;

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
  incSpeed = random(4, 6) / 100.0;
  creepBootUp();
  holdWhite();
  toColor();
  Serial.begin(9600);
}

void creepBootUp() {
  float start = 0;
  float bright = 0;
  
  while (start < NUM_LEDS+10) {
    start+=.5;
    for (int i = 0; i < NUM_LEDS; i++) {
      if(i<=start){
      bright = (start - i)*3;
      if(bright>255){
        bright=255;
      }
      
      leds[i] = CRGB( bright, bright, bright);
      }
      else{
        leds[i] = CRGB( 0, 0, 0);
      }
    }
    FastLED.show();
    delay(5);
  }

}

void holdWhite() {
  float start = 0;
  float localHue = start;
  start += .7;
  for (int x = 0; x < 60; x++) { // run for 400 frames
    for (int i = 0; i < NUM_LEDS; i++) {
      if(x==59){
        leds[i] = CRGB( 255, 255, 255);
      }else{
         leds[i] = CHSV(localHue, 255, 255);
         localHue += 65;
      }    
    }
    FastLED.show();
    delay(50);
  }
}

void toColor() {
  float g = 255;
  float b = 255;
  float changeSpeed = .6;

  while (g > 0 ) {
    g -= changeSpeed;
    b -= changeSpeed;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB( 255, g, b);
    }
    FastLED.show();
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
