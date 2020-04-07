/* Teensy CAN-Bus receiver
 *  
 * www.skpang.co.uk
 * 
 * V1.0 Aug 2018
 *  
 * For use with Teensy CAN-Bus Breakout board:
 * http://skpang.co.uk/catalog/teensy-canbus-breakout-board-include-teensy-32-p-1507.html
 * requires OLED display
 * http://skpang.co.uk/catalog/oled-128x64-display-for-teensy-breakout-board-p-1508.html
 * 
 * *  
 */
 
#include <FlexCAN.h>
//#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

static CAN_message_t msg,rxmsg;
volatile uint32_t count = 0;
IntervalTimer data_timer;
String CANStr(""); 
volatile uint32_t can_msg_count = 0;
float converted_data;
uint16_t  CAN_ID =  0x700;
unsigned char data[4];

uint8_t no_data1 = 0;   // No data counter for channel 1
uint8_t no_data2 = 0;   // No data counter for channel 2


void setup(){
  Can0.begin(500000); //Set CAN speed to 500kbps --> Adjust bit rate in RPi interface
  Serial.println(F("CAN Bus test"));
  pinMode(13,OUTPUT);
  delay(1000);
  data_timer.begin(data_count, 1000000);   // Start no data interrupt counter at 1s interval
 
}

/* From Timer Interrupt */
void data_count(void)
{
  no_data1++;
  no_data2++;
}

void loop() {
  int i;
  //Serial.println(Can0.read(rxmsg)); 
  while(Can0.read(rxmsg)) //Read data from CAN-bus
  { 
     //Serial.println("HOLA"); 
     String CANStr(""); 
     for (int i=0; i < 8; i++) {     

         CANStr += String(rxmsg.buf[i],HEX);
         CANStr += (" ") ;
     }
     
     if(rxmsg.id == CAN_ID)
     {
      data[0] = rxmsg.buf[0];
      data[1] = rxmsg.buf[1];
      data[2] = rxmsg.buf[2];
      data[3] = rxmsg.buf[3];
      
      memcpy(&converted_data, data, 4);   //Convert data back to float
      Serial.print(rxmsg.id,HEX); 
      Serial.print(' '); 
      Serial.print(rxmsg.len,HEX); 
      Serial.print(' ');
      Serial.print(CANStr); 
      Serial.print("Ch1 Temperature : ");
      Serial.println(converted_data,4);     
      no_data1 = 0;  //Clear no data counter
    }

    if(rxmsg.id == CAN_ID + 1)  // Indoor temperature
    {
      data[0] = rxmsg.buf[0];
      data[1] = rxmsg.buf[1];
      data[2] = rxmsg.buf[2];
      data[3] = rxmsg.buf[3];

      memcpy(&converted_data, data, 4);     //Convert data back to float
      Serial.print(rxmsg.id,HEX); 
      Serial.print(' '); 
      Serial.print(rxmsg.len,HEX); 
      Serial.print(' ');
      Serial.print(CANStr); 
      Serial.print("Ch2 Humidity : ");
      Serial.println(converted_data,4);      
      no_data2 = 0;   //Clear no data counter
    }

     
  }
   if(no_data1 >2)  //Check data still coming in within 2 second
   {
      no_data1 = 3;  // Prevent counter rollover
    }
   
   if(no_data2 >2)  //Check data still coming in within 2 second
   {
      no_data2 = 3;  // Prevent counter rollover
    }

    
}
