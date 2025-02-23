#include <Arduino.h>
#include <Keypad.h>

#define ROWS_NUM 3
#define COLS_NUM 3

byte rows[ROWS_NUM] = {D19, D5, A9};
byte cols[COLS_NUM] = {A3, A2, A1};

char keys[ROWS_NUM][COLS_NUM] = {
    {'3', '6', '9'},
    {'2', '5', '8'},
    {'1', '4', '7'}};

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
  int keys = keypad.getKeys();
  int key = keypad.getState();

  if (keys)
  {
    for (int i = 0; i < LIST_MAX; i++)
    {
      if (keypad.key[i].kstate == PRESSED)
      {
        Serial.print("Key Pressed: ");
        Serial.println(keypad.key[i].kchar);
      }
    }
  }
}

