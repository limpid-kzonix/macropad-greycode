#include <Arduino.h>
#include <Keypad.h>

#define ROWS_NUM 3
#define COLS_NUM 3

byte rows[ROWS_NUM] = {D19, D5, A9};
byte cols[COLS_NUM] = {A3, A2, A1};

char keys[ROWS_NUM][COLS_NUM] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}};

Keypad keypad = Keypad(makeKeymap(keys), rows, cols, ROWS_NUM, COLS_NUM);

void setup()
{

  Serial.begin(9600);

  while (!Serial)
  {
    /* code */
  }
}

void loop()
{
  int val = keypad.getKey();

  if (val != NO_KEY)
  {
    Serial.print("Key pressed: ");
    Serial.println(val);
  }
}
