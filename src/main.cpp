#include <Arduino.h>
#include <Keypad.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// region Keypad

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

// endregion Keypad

// region OLED 0.91 Inch display with SSD1306 driver

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// endregion OLED 0.91 Inch display with SSD1306 driver

// region KY-040 Rotary Encoder

#define ENCODER_CLK A0
#define ENCODER_DT D15
#define ENCODER_SW MISO

// region KY-040 Rotary Encoder

// region setup
void setup() {

	// region Serial
	Serial.begin(9600);

	while (!Serial) {
		;
	}
	// endregion Serial

	Serial.println("Starting: OLED 0.91 Inch display with SSD1306 driver");

	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			;
	}

	display.display();
	delay(2000);

	display.clearDisplay();
	display.setRotation(1);
	display.setTextSize(2);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0, 0);
	display.println("Hello, world!");
	display.display();

	Serial.println(
	    "Started successfully: OLED 0.91 Inch display with SSD1306 driver");

	// region KY-040 Rotary Encoder
	pinMode(ENCODER_CLK, INPUT);
	pinMode(ENCODER_DT, INPUT);
	pinMode(ENCODER_SW, INPUT_PULLUP);

	Serial.println("Starting: KY-040 Rotary Encoder");
	// endregion KY-040 Rotary Encoder
}
// endregion setup
int encoderPos = 0;     // Global variable to store the encoder position
int lastCLK;            // Last state of the CLK pin

// region loop
void loop() {
	int keys = keypad.getKeys();

	if (keys) {
		for (int i = 0; i < LIST_MAX; i++) {
			if (keypad.key[i].kstate == PRESSED) {
				Serial.print("Key Pressed: ");
				Serial.println(keypad.key[i].kchar);
			}
		}
	}
	// Read the current state of the CLK pin
  int currentCLK = digitalRead(ENCODER_CLK);

  // If the CLK state has changed, then a rotation has occurred
  if (currentCLK != lastCLK) {
    // Compare the state of DT to determine rotation direction
    if (digitalRead(ENCODER_DT) != currentCLK) {
      encoderPos++;   // Clockwise rotation
    } else {
      encoderPos--;   // Counterclockwise rotation
    }
    Serial.print("Encoder Position: ");
    Serial.println(encoderPos);
  }
  
  // Update lastCLK for the next iteration
  lastCLK = currentCLK;
  
  // Check if the switch (button) is pressed (active low)
  if (digitalRead(ENCODER_SW) == LOW) {
    Serial.println("Encoder Button Pressed");
    delay(300);  // Simple debounce delay
  }
  
  delay(1); // Small delay to improve stability
}

// endregion loop
