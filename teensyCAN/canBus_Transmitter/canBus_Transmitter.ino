/* Sensors measurement. Data transmitted over CAN-Bus
 * 
 * www.skpang.co.uk
 * 
 * 1.0 Aug 2018
 * For use with Teensy CAN-Bus Breakout board:
 * http://skpang.co.uk/catalog/teensy-canbus-breakout-board-include-teensy-32-p-1507.html
 * 
 * and BME280 breakout:
 * http://skpang.co.uk/catalog/adafruit-bmp280-i2c-or-spi-barometric-pressure-altitude-sensor-p-1454.html
 * 
 * Require BME280 library from:
 * https://github.com/bolderflight/BME280
 */
 
#include <FlexCAN.h>

static CAN_message_t msg;
uint16_t  CAN_ID =  0x700;    // CAN-Bus ID
float temp;
float hum;
uint8_t bytes[sizeof(float)];

void setup() {
  Can0.begin(500000); //Set CAN speed to 500kbps
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(1000);
  Serial.println(F("Sensor CAN Bus Tx test"));
}

void loop() {  
  temp = 25.31416;
  memcpy(bytes, &temp, sizeof (temp)); //Convert the temperature in float into 4 bytes
  Serial.print(temp,2);
  Serial.print("\t");
  msg.len = 8;
  msg.id = CAN_ID;
  msg.buf[0] = bytes[0];
  msg.buf[1] = bytes[1];
  msg.buf[2] = bytes[2];
  msg.buf[3] = bytes[3];
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);                      //Transmit the CAN data out
  hum = 0;
  memcpy(bytes, &hum, sizeof (hum)); //Convert the humidity in float into 4 bytes
  Serial.print(hum,2);
  Serial.print("\t");
  msg.len = 8;
  msg.id = CAN_ID +1;
  msg.buf[0] = bytes[0];
  msg.buf[1] = bytes[1];
  msg.buf[2] = bytes[2];
  msg.buf[3] = bytes[3];
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0; 
  Can0.write(msg);                       //Transmit the CAN data out
  Serial.println(" ");
  delay(1000);
}
