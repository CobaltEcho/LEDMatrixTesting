#include <FastLED.h>
#include <OneButton.h>

const uint8_t DisplayWidth = 14;
const uint8_t DisplayHeight = 10;

const uint16_t NumLeds = DisplayWidth * DisplayHeight;

#define LED_PIN   6
#define NextShow_PIN   3  
#define NumCountUp_PIN   4

CRGB leds[NumLeds];

const uint8_t MaxNum = 7;
uint8_t whichNum = 0;

const uint8_t MaxShow = 3;
uint8_t whichShow = 0;

OneButton NextShowBtn(NextShow_PIN, true, true);
OneButton NumCountUpBtn(NumCountUp_PIN, true, true);

const uint8_t FontWidth = 7;
const uint8_t FontHeight = 10;
const uint8_t NumCharacters = 10;

const uint8_t FontTable[NumCharacters][FontHeight] = {
	{ 0x7e, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7e, 0x7e },  // 0
	{ 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60 },  // 1
	{ 0x7e, 0x7e, 0x60, 0x60, 0x7e, 0x7e, 0x06, 0x06, 0x7e, 0x7e },  // 2
	{ 0x7e, 0x7e, 0x60, 0x60, 0x7c, 0x7c, 0x60, 0x60, 0x7e, 0x7e },  // 3
	{ 0x66, 0x66, 0x66, 0x66, 0x7e, 0x7e, 0x60, 0x60, 0x60, 0x60 },  // 4
	{ 0x7e, 0x7e, 0x06, 0x06, 0x7e, 0x7e, 0x60, 0x60, 0x7e, 0x7e },  // 5
	{ 0x06, 0x06, 0x06, 0x06, 0x7e, 0x7e, 0x66, 0x66, 0x7e, 0x7e },  // 6
	{ 0x7e, 0x7e, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60 },  // 7
	{ 0x7e, 0x7e, 0x66, 0x66, 0x7e, 0x7e, 0x66, 0x66, 0x7e, 0x7e },  // 8
	{ 0x7e, 0x7e, 0x66, 0x66, 0x7e, 0x7e, 0x60, 0x60, 0x60, 0x60 }   // 9
};

const uint8_t NumDigits = DisplayWidth / FontWidth;


// Calculate the index for a given LED in the matrix, given its X/Y coordinates
uint16_t xy(uint16_t x, uint16_t y) {
	const int RowBase = DisplayWidth * y;  // number of LEDs before the current row
	if (x >= DisplayWidth) x = DisplayWidth - 1;
	if (y >= DisplayHeight) y = DisplayHeight - 1;

	uint16_t output;
	if (y % 2 == 0) output = RowBase + x;  // normal on even rows
	else output = RowBase + (DisplayWidth - x - 1);  // reversed on odd rows (serpentine)

	if (output >= NumLeds) output = NumLeds - 1;

	return output;
}

void clearDigit(int pos, CRGB color = CRGB::Black);
void writeDigit(int pos, int num, CRGB color, CRGB background = CRGB::Black);
void writeNumber(int num, CRGB color, CRGB background = CRGB::Black);


void clearDigit(int pos, CRGB color) {
	if (pos < 0 || pos >= NumDigits) return;  // display index out of range

	const uint8_t ColumnOffset = FontWidth * pos;  // offset the column position per digit

	for (uint8_t row = 0; row < FontHeight; row++) {
		for (uint8_t col = 0; col < FontWidth; col++) {
			leds[xy(col + ColumnOffset, row)] = color;  // assign color to LED array
		}
	}
}

void writeDigit(int pos, int num, CRGB color, CRGB background) {
	if (num < 0 || num >= NumCharacters) return;  // number out of range
	if (pos < 0 || pos >= NumDigits) return;  // display index out of range

	const uint8_t* Character = FontTable[num];  // get the font array from the table
	const uint8_t ColumnOffset = FontWidth * pos;  // offset the column position per digit

	for (uint8_t row = 0; row < FontHeight; row++) {
		for (uint8_t col = 0; col < FontWidth; col++) {
			const bool lit = Character[row] & (1 << col);  // extract bit for this LED
			leds[xy(col + ColumnOffset, row)] = lit ? color : background;  // assign color to LED array
		}
	}
}

void writeNumber(int num, CRGB color, CRGB background) {
	num = abs(num);  // not supporting negative numbers yet

	for (uint8_t i = 0; i < NumDigits; i++) {
		uint8_t digit = num % 10;
		writeDigit(NumDigits - i - 1, digit, color, background);  // right to left
		num /= 10;
	}
}

void ShowNumber() {
	writeNumber(whichNum, CRGB::Blue);
	printLEDs();
}

void IncrementNextShow() {
	whichShow++;
	if (whichShow >= MaxShow) whichShow = 0;
	DebugStatus();
}

void IncrementNumberShow() {
	whichNum++;
	if (whichNum >= MaxNum) whichNum = 0;
	DebugStatus();

	ShowNumber();
}

void setup() {
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NumLeds);
	FastLED.setBrightness(100);

	Serial.begin(115200);

	NextShowBtn.attachClick(IncrementNextShow);
	NumCountUpBtn.attachClick(IncrementNumberShow);

	ShowNumber();
}

void loop() {
	NextShowBtn.tick();
	NumCountUpBtn.tick();

	switch (whichShow) {
	case 0:
		break;  // LEDs written in button function
	case 1:
		FlagShow();
		break;
	case 2:
		BlinkShow();
		break;
	}
	FastLED.show();
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
	const unsigned long BlinkSpeed = 1000;  // ms
	static unsigned long lastChange = 0;
	static bool blinkState = false;

	const unsigned long now = millis();

	if (now - lastChange >= BlinkSpeed) {
		CRGB color = blinkState ? CRGB::Blue : CRGB::Red;
		fill_solid(leds, NumLeds, color);
		FastLED.show();

		lastChange = now;
		blinkState = !blinkState;
	}
}

void DebugStatus() {
	Serial.print("ShowNumber: ");
	Serial.print(whichShow);
	Serial.print(" WhichNumber: ");
	Serial.print(whichNum);
	Serial.print('\n');
}

// Debug visualization for the LED on/off states
void printLEDs() {
	for (uint8_t row = 0; row < DisplayHeight; row++) {
		for (uint8_t col = 0; col < DisplayWidth; col++) {
			bool on = leds[xy(col, row)].getLuma() >= 16;  // arbitrary threshold
			Serial.print(on ? 'X' : '_');
			Serial.print(" ");
		}
		Serial.println();
	}
}
