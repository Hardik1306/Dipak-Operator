#include "main.h"


void setup() {
  pinMode(5, OUTPUT);
  vspi = new SPIClass(VSPI);
  vspi->begin();
  Ps3.begin("44:44:44:44:44:44");

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  Serial.begin(115200);
  //  Serial1.begin(115200, SERIAL_8N1, 13, 14);
  Serial2.begin(115200);

  delay(1000);
  cytron_1.send_dummy_bit();
  delay(1000);
  millis();
  //  bot.callibrate();
  //  pinMode(LIMIT1, INPUT);
  //  pinMode(LIMIT2, INPUT);

  //  tfmini.begin(&Serial);
}

uint8_t pwm = 70;
bool prev_pwm = false;
bool l1 = 0;
bool r1 = 0;
bool can_control = 0;
bool ps = 0;
void loop() {
  mpu6050.update();
  Serial.println(mpu6050.getAngleZ());
  if (Ps3.isConnected()) {
    if (!can_control) {
      if (Ps3.data.button.ps && !ps) {
        ps = 1;
        can_control = 1;
      }
      else if (Ps3.data.button.ps);
      else if (Ps3.data.button.l1 && Ps3.data.button.r1) {
        pwm = 65;
      }
      else if (Ps3.data.button.l1 && !l1)
      {
        pwm = pwm - 5;
        l1 = 1;
      }
      else if (Ps3.data.button.l1);
      else if (Ps3.data.button.r1 && !r1)
      {
        pwm = pwm + 10;
        r1 = 1;
      }
      else if (Ps3.data.button.r1);
      else if (Ps3.data.button.l2)
      {
        bot.base_anti_clockwise(pwm, pwm, pwm, pwm);
      }
      else if (Ps3.data.button.r2)
      {
        bot.base_clockwise(pwm, pwm, pwm, pwm);
      }
      else if (Ps3.data.button.up && Ps3.data.button.right)
      {
        bot.up_right(pwm, pwm, pwm, pwm);
      }
      else if (Ps3.data.button.up && Ps3.data.button.left)
      {
        bot.up_left(pwm, pwm, pwm, pwm);
      }
      else if (Ps3.data.button.down && Ps3.data.button.right)
      {
        bot.down_right(pwm, pwm, pwm, pwm);
      }
      else if (Ps3.data.button.down && Ps3.data.button.left)
      {
        bot.down_left(pwm, pwm, pwm, pwm);
      }
      else if (Ps3.data.button.up)
      {
        bot.forward(pwm, pwm, pwm, pwm);
      }
      else if (Ps3.data.button.down)
      {
        bot.backward(pwm, pwm, pwm, pwm);
      }
      else if (Ps3.data.button.left)
      {
        bot.left(pwm, pwm, pwm, pwm);
      }
      else if (Ps3.data.button.right)
      {
        bot.right(pwm, pwm, pwm, pwm);
      } else if (Ps3.data.button.right) {
        bot.calib_angle();
      }
      else if (Ps3.data.button.right) {
        bot.calib_angle();
      }
      else if (Ps3.data.button.triangle) {
        send_data(1);
      }
      else if (Ps3.data.button.circle) {
        send_data(5);
      }
      else if (Ps3.data.button.cross) {
        send_data(10);
      }
      else if (Ps3.data.button.square) {
        send_data(15);
      }
      else if (Ps3.data.analog.stick.ly < -50 ) {
        send_data(30);
      }
      else if (Ps3.data.analog.stick.ly > 50 ) {
        send_data(35);
      }
      else if (Ps3.data.analog.stick.ly < -50 ) {
        send_data(40);
      }
      else if (Ps3.data.analog.stick.ly > 50 ) {
        send_data(45);
      }
      else if (Ps3.data.button.select) {
        send_data(50);
      }
      else if (Ps3.data.button.start) {
        send_data(55);
      }
      else if (Ps3.data.button.r3) {
        send_data(60);
      }
      else if (Ps3.data.button.l3) {
        send_data(65);
      }
      else
      {
        l1 = 0;
        r1 = 0;
        prev_pwm = false;
        bot.base_free();
        send_data(0);
      }
    } else {
      if (Ps3.data.button.ps && !ps) {
        ps = 1;
        can_control = 0;
        bot.base_free();
      }
      else if (Ps3.data.button.ps);
      else {
        ps = 0;
        bot.base_free();
      }
    }
  } else {
    bot.base_free();
    send_data(0);
  }
}
void send_data(byte x) {
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(5, LOW);
  vspi->transfer(x);
  digitalWrite(5, HIGH);
  vspi->endTransaction();
}
