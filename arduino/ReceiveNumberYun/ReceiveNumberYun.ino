/************************************************************
 * ReceiveNumber
 * A sketch to receive number from the GenerateNumber sketch.
 * Author: Ron Smith
 * Copyright (c)2015, That Ain't Working, All Rights Reserved
 ************************************************************/

#include <Bridge.h>
#include <Console.h>
#include <stdio.h>

char sp_buff[255];

#if defined(ARDUINO_AVR_YUN)
#define RX_DATA_PIN 3
#define RX_DATA_INT 0
#define TX_DATA_PIN 2
#define TX_POWER_PIN 4
#define PRINTF(format, ...) sprintf(sp_buff, format, __VA_ARGS__); Console.print(sp_buff)
#else
#define RX_DATA_PIN 2
#define RX_DATA_INT 0
#define TX_DATA_PIN 3
#define TX_POWER_PIN 4
#define PRINTF(format, ...) sprintf(sp_buff, format, __VA_ARGS__); Serial.print(sp_buff)
#endif

/** NOTE:
 ** I thought it would be good to be able to turn the
 ** transmitter off when I wasn't transmitting.  Turns out,
 ** turning it on/off creates a lot of noise on the channel.
 **/

#define FIND_LEAD    0
#define LEAD1        1
#define LEAD2        2
#define LEAD3        3
#define IN_MSG       4



volatile int rx_state;
volatile unsigned long rising_time;
volatile unsigned long last_d;
volatile int last_state;

void setup() {
  Bridge.begin(); 
  Console.begin(); 
  while (!Console);

  pinMode(TX_POWER_PIN, OUTPUT);
  digitalWrite(TX_POWER_PIN, LOW);

  rx_state = FIND_LEAD;
  rising_time = 0L;
  last_d = 0L;
  last_state = LOW;

  pinMode(RX_DATA_PIN, INPUT);
//  attachInterrupt(RX_DATA_INT, receive_data, CHANGE);

  Console.println("Setup complete.");
}

void receive_data() {
    int cur_state = digitalRead(RX_DATA_PIN);
    
    if (last_state == LOW && cur_state == HIGH) {
        last_state = HIGH;
        rising_time = millis();
        return;
    }
    
    if (last_state == HIGH && cur_state == LOW) {
        last_state = LOW;
        unsigned long d = millis() - rising_time;

        if (d > 7L) {
        
            switch (rx_state) {
                case FIND_LEAD:
                    if (d >= 15L && d <= 35L) {
                        rx_state = LEAD1;
                        //PRINTF("(%ld) Begin lead-in.\n", d);
                    }
                    break;
    
                case LEAD1:
                    if (d > 15 && d < 25L) {
                        rx_state = LEAD2;
                    } else if (d + last_d > 15 && d + last_d < 25L) {
                        rx_state = LEAD2;
                    } else {
                        rx_state = FIND_LEAD;
                       //PRINTF("(%ld) Misled on 1.\n", d);
                    }
                    break;
                
                case LEAD2:
                    if (d <= 20L) {
                        rx_state = LEAD3;
                    } else {
                        rx_state = FIND_LEAD;
                        //PRINTF("(%ld) Mislead on 2.\n", d);
                    }
                    break;
                
                case LEAD3:
                    if (d > 25L && d < 35L) {
                        rx_state = IN_MSG;
                        //PRINTF("(%ld) Begin message.\n", d);
                    } else {
                        rx_state = FIND_LEAD;
                        //PRINTF("(%ld) Mislead on 3.\n", d);
                    }
                    break;
                
                case IN_MSG:
                    PRINTF(" (%ld)", d);
                    if (d <= 15L) {
                        Console.print("0");
                    } else if (d > 15 && d < 22L) {
                        Console.print("1");
                    } else if (d >= 22L) {
                        //Console.println();
                        Console.print(" End message.\n");
                        rx_state = FIND_LEAD;
                    }
                    break;
            }
        }
        last_d = d;
    }
}

void loop() {
  receive_data();
}
