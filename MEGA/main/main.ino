#include <SPI.h>
#include "TF.h"
#include "Adafruit_MCP23017.h"
#include <Adafruit_MCP4725.h>
#include "Motor.h"
#include"cytron.h"
#include <Encoder.h>
#include <LiquidCrystal_I2C.h>
Adafruit_MCP4725 dac;
Adafruit_MCP23017 mcp;
Motor pipe_motor(35, 37, 7);

int buttonstate = 0;
int lastbuttonstate = 0;

Encoder enc(2, 3);

bool r3 = 0;
bool circle = 0;
bool Square = 0;
bool cross = 0;
bool triangle = 0;
bool start = 0;
bool select = 0;
bool pipe_pnumetic = 0;
byte pipe;
uint8_t button = 7;
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
const long interval = 500;
int pressure;
unsigned long currentMillis;
unsigned long currentMillis1;

LiquidCrystal_I2C lcd(0x27, 16, 2);

ISR(SPI_STC_vect) {
  button = SPDR;

}

void setup() {
  pinMode(39, INPUT);
  Serial.begin(115200);
  Serial.println("low");
  mcp.begin();
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();
  Serial.println("high");
  for (uint8_t i = 1; i < 16; i++) {
    mcp.pinMode(i, OUTPUT);
    //        mcp.digitalWrite(i, HIGH);
  }
  dac.begin(0x60);
  dac.setVoltage(0, false);
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
}


void loop() {
  Serial.println(button);
  if (button == 55 && !r3) {
    r3 = 1;
    pipe_pnumetic = !pipe_pnumetic;
    mcp.digitalWrite(7, pipe_pnumetic);
    mcp.digitalWrite(8, !pipe_pnumetic);
  }
  else if (button == 55);
  else if (button == 1 && !triangle) {
    triangle = 1;
    high(2);
    delay(500);
    low(2);
  }
  else if (button == 1);
  else if (button == 5 && !circle) {
    circle = 1;
    high(3);
    delay(500);
    low(3);
  }
  else if (button == 5);
  else if (button == 10 && !cross) {
    cross = 1;
    high(4);
    delay(500);
    low(4);
  }
  else if (button == 10);
  else if (button == 15 && !Square) {
    Square = 1;
    high(5);
    delay(500);
    low(5);
  }
  else if (button == 15);
  else if (button == 60 && !r3) {
    r3 = 1;
    high(6);
    delay(500);
    low(6);
  }
  else if (button == 60);

  else if (button == 55 && !start) {
    pressure = pressure + 15;
    dac.setVoltage(pressure, false);
    start = 1;
    currentMillis = millis();
    previousMillis = currentMillis;
  }
  else if (button == 55) {
    currentMillis = millis();
    if ((currentMillis - previousMillis) >= interval) {
      pressure = pressure + 5;
      pressure = constrain(pressure , 0, 4095);
      dac.setVoltage(pressure, false);
      Serial.println(pressure);
      delay(10);
    }
  }
  else if (button == 50 && !select) {
    pressure = pressure - 15;
    dac.setVoltage(pressure, false);
    select = 1;
    currentMillis1 = millis();
    previousMillis1 = currentMillis1;
  }
  else if (button == 50) {
    currentMillis1 = millis();
    if ((currentMillis1 - previousMillis1) >= interval) {
      pressure = pressure - 5;
      pressure = constrain(pressure , 0, 4095);
      dac.setVoltage(pressure, false);
      Serial.println(pressure);
      delay(10);
    }
  }
  else if (button == 30 && !digitalRead(A6)) {
    pipe_motor.anti_clockwise(50);
  }
  else if (button == 35) {
    pipe_motor.clockwise(50);
  }
  else {
    pipe_motor.brake();
    r3 = 0;
    cross = 0;
    triangle = 0;
    Square = 0;
    circle = 0;
    select = 0;
    start = 0;
  }
  if (digitalRead(A6)) {
    enc.write(0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(enc.read());
  }
}



void high(uint8_t pin) {
  mcp.digitalWrite(pin, HIGH);
}
void low(uint8_t pin) {
  mcp.digitalWrite(pin, LOW);
}
void dac_set_voltage(float y) {
  int z = (y * 4095) / 5.07;
  dac.setVoltage(z, false);
}
void print_angle() {
  byte x;
  x  = (enc.read() * 108) / 3500;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angle    - ");
  lcd.setCursor(11, 0);
  lcd.print(x);
  lcd.setCursor(0, 1);
  lcd.print("Enc_Read - ");
  lcd.setCursor(11, 1);
  lcd.print(enc.read());
}
//void throw_arrow() {
//  high(pipe + 1);
//  delay(800);
//  low(pipe + 1);
//  if (pipe == 5 )
//    pipe = 0;
//}
