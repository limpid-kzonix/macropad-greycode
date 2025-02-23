#include <Arduino.h>

#include <Keyboard.h>
#include <Keypad.h>

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>

#include <Wire.h>

// region keypad

#define ROWS_NUM 3
#define COLS_NUM 3

// TODO: need to document the pinout and findings why these pins are used
// MOSI does work as INPUT_PULLUP
//  - previously tried D4, D6, D7, A10
byte rows[ROWS_NUM] = {MOSI, D5, A9};
byte cols[COLS_NUM] = {A3, A2, A1};

// clang-format off
char keys[ROWS_NUM][COLS_NUM] = {
    {'3', '6', '9'}, 
    {'2', '5', '8'}, 
    {'1', '4', '7'},
  };
// clang-format on

Keypad keypad = Keypad(makeKeymap(keys), rows, cols, ROWS_NUM, COLS_NUM);

// Define layers
enum Layer { BASE, FN_LAYER };

Layer currentLayer = BASE;
bool layerToggle = false; // Tracks if we are in the second layer

// Special Key (e.g., hold '5' to switch layer)
#define LAYER_SWITCH_KEY '5'
// endregion keypad

// region encoder

#define ENCODER_CLK SCL
#define ENCODER_DT SDA
#define ENCODER_SW D0

void readEncoder();
// Variables to hold the current and last encoder position
volatile int encoderPos = 0;
int lastEncoderPos = 0;

// Variables to keep track of the state of the pins
int lastCLK;
int currentCLK;
// endregion encoder

// region LED

#define LED_STRIP_PIN A0
#define LED_RING_PIN MISO
#define NUMPIXELS 8

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(NUMPIXELS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ring =
    Adafruit_NeoPixel(NUMPIXELS, LED_RING_PIN, NEO_GRB + NEO_KHZ800);
// endregion LED

// region setup
void setup() {

	Keyboard.begin();

	// region Serial
	Wire.begin();

	// endregion Serial

	// region encoder-setup
	pinMode(ENCODER_CLK, INPUT_PULLUP);
	pinMode(ENCODER_DT, INPUT_PULLUP);
	pinMode(ENCODER_SW, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), readEncoder,
			CHANGE);
	digitalWrite(ENCODER_CLK, HIGH);

	lastCLK = digitalRead(ENCODER_CLK);
	Serial.println("Starting: KY-040 Rotary Encoder");
	// endregion encoder-setup

	// region LED-setup
	strip.begin();
	strip.setBrightness(100);
	strip.show(); // Initialize all pixels to 'off'

	strip.setPixelColor(0, 255, 0, 0);
	strip.setPixelColor(1, 0, 255, 0);
	strip.setPixelColor(2, 0, 0, 255);
	strip.setPixelColor(3, 255, 255, 0);
	strip.setPixelColor(4, 255, 0, 255);
	strip.setPixelColor(5, 0, 255, 255);
	strip.setPixelColor(6, 255, 255, 255);
	strip.setPixelColor(7, 22, 123, 33);
	strip.show();

	ring.begin();
	ring.setBrightness(100);
	ring.show(); // Initialize all pixels to 'off'

	ring.setPixelColor(0, 255, 0, 0);
	ring.setPixelColor(1, 0, 255, 0);
	ring.setPixelColor(2, 0, 0, 255);
	ring.setPixelColor(3, 255, 255, 0);
	ring.setPixelColor(4, 255, 0, 255);
	ring.setPixelColor(5, 0, 255, 255);
	ring.setPixelColor(6, 255, 255, 255);
	ring.setPixelColor(7, 0, 99, 110);

	ring.show();

	// endregion LED-setup
}

void readEncoder() {
	currentCLK = digitalRead(ENCODER_CLK);
	// If the current state of CLK is different from the last state
	// then a pulse occurred
	if (currentCLK != lastCLK) {
		// If the DT state is different from the CLK state
		// then the encoder is rotating clockwise
		if (digitalRead(ENCODER_DT) != currentCLK) {
			encoderPos++;
		} else {
			// Otherwise, it's rotating counterclockwise
			encoderPos--;
		}
	}
	// Update lastCLK with the current state for the next pulse detection
	lastCLK = currentCLK;
}

#define DEBOUNCE_DELAY 50
unsigned long lastKeyPress = 0;

bool isCtrlPressed = false;

// region loop
void loop() {
	int keys = keypad.getKeys();

	if (keys) {
		delay(10);

		for (int i = 0; i < LIST_MAX; i++) {
			if (keypad.key[i].kstate == PRESSED &&
			    keypad.key[i].stateChanged) {
				Serial.print("Key Pressed: ");
				Serial.println(keypad.key[i].kchar);
				Keyboard.press(keypad.key[i].kchar);
				delay(10);
			} else if (keypad.key[i].kstate == RELEASED &&
				   keypad.key[i].stateChanged) {
				Serial.print("Key Released: ");
				Serial.println(keypad.key[i].kchar);
				Keyboard.release(keypad.key[i].kchar);
				delay(10);
			}
		}
	}
	// Check if the encoder has moved
	if (encoderPos != lastEncoderPos) {
		Serial.print("Position: ");
		Serial.println(encoderPos);
		lastEncoderPos = encoderPos;
	}

	// Check if the button is pressed
	if (digitalRead(ENCODER_SW) == LOW) {
		// Reset the encoder position to 0
		encoderPos = 0;
		Serial.println("Reset to 0");
		// Wait for the button to be released
		while (digitalRead(ENCODER_SW) == LOW)
			delay(10);
			if (encoderPos != lastEncoderPos) {
				Serial.print("Held and Position: ");
				Serial.println(encoderPos);
				lastEncoderPos = encoderPos;
			}
	}
}

// endregion loop
