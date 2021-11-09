#include <FastLED.h>
#include <OneButton.h>

#define NUM_LEDS  35
#define LED_PIN   6
#define NextShow_PIN   3  
#define NumCountUp_PIN   4

CRGB leds[NUM_LEDS];

uint8_t whichNum = 0;
uint8_t whichShow = 0;

OneButton NextShowBtn(NextShow_PIN, true, true);
OneButton NumCountUpBtn(NumCountUp_PIN, true, true);

void checkTick() {
    NextShowBtn.tick();
    NumCountUpBtn.tick();
}

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(NextShow_PIN),checkTick,CHANGE);
  attachInterrupt(digitalPinToInterrupt(NumCountUp_PIN),checkTick,CHANGE);
  NextShowBtn.attachClick(IncrementNextShow);
  NumCountUpBtn.attachClick(IncrementNumberShow);
}

void CurrentStatus(){
  Serial.print("ShowNumber: ");
  Serial.print(whichShow);
  Serial.print(" WhichNumber: ");
  Serial.print(whichNum);
  Serial.print('\n');
}

void loop() {
  NextShowBtn.tick();
  NumCountUpBtn.tick();
  FastLED.show();

  while (whichShow == 0) {
    NextShowBtn.tick();
    BlinkShow();
}
}

void IncrementNextShow() {
  whichShow = (whichShow + 1) % 3;
  CurrentStatus();
  DisplayNextShow();
}

void DisplayNextShow() {
  switch (whichShow){
    case 0:
    DisplayNumberShow();
    break;
    case 1:
    FlagShow();
    break;
    case 2:
//    BlinkShow();
    break;
  }
  FastLED.show();
}

void IncrementNumberShow() {
whichNum = (whichNum + 1) % 7;
CurrentStatus();
DisplayNumberShow();
}

void DisplayNumberShow() {
  switch (whichNum) {
    case 0:
      Num0();
      break;
    case 1:
      Num1();
      break;
    case 2:
      Num2();
      break;
    case 3:
      Num3();
      break;
    case 4:
      Num4();
      break;
    case 5:
      Num5();
      break;
    case 6:
      Num6();
      break;
   }

void FlagShow() {
	leds[0] = CRGB(0, 0, 255);
	leds[1] = CRGB(0, 0, 255);
	leds[2] = CRGB(0, 0, 255);
	leds[3] = CRGB(255, 0, 0);
	leds[4] = CRGB(255, 0, 0);
	leds[5] = CRGB(255, 0, 0);
	leds[6] = CRGB(255, 0, 0);
	leds[7] = CRGB(255, 255, 255);
	leds[8] = CRGB(255, 255, 255);
	leds[9] = CRGB(255, 255, 255);
	leds[10] = CRGB(255, 255, 255);
	leds[11] = CRGB(0, 0, 255);
	leds[12] = CRGB(255, 255, 255);
	leds[13] = CRGB(0, 0, 255);
	leds[14] = CRGB(0, 0, 255);
	leds[15] = CRGB(0, 0, 255);
	leds[16] = CRGB(0, 0, 255);
	leds[17] = CRGB(255, 0, 0);
	leds[18] = CRGB(255, 0, 0);
	leds[19] = CRGB(255, 0, 0);
	leds[20] = CRGB(255, 0, 0);
	leds[21] = CRGB(255, 255, 255);
	leds[22] = CRGB(255, 255, 255);
	leds[23] = CRGB(255, 255, 255);
	leds[24] = CRGB(255, 255, 255);
	leds[25] = CRGB(255, 255, 255);
	leds[26] = CRGB(255, 255, 255);
	leds[27] = CRGB(255, 255, 255);
	leds[28] = CRGB(255, 0, 0);
	leds[29] = CRGB(255, 0, 0);
	leds[30] = CRGB(255, 0, 0);
	leds[31] = CRGB(255, 0, 0);
	leds[32] = CRGB(255, 0, 0);
	leds[33] = CRGB(255, 0, 0);
	leds[34] = CRGB(255, 0, 0);
}
void BlinkShow() {
	fill_solid(leds, NUM_LEDS, CRGB::Red);
	FastLED.show();
	delay(500);
	// Now turn the LED off, then pause
	fill_solid(leds, NUM_LEDS, CRGB::Blue);
	FastLED.show();
	delay(500);
}
