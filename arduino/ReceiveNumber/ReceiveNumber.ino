/************************************************************
 * ReceiveNumber
 * A sketch to receive number from the GenerateNumber sketch.
 * Author: Ron Smith
 * Copyright (c)2015, That Ain't Working, All Rights Reserved
 ************************************************************/

#if defined(ARDUINO_AVR_YUN)
#include <Console.h>
#define SERIAL Console
#define SERIAL_BEGIN Bridge.begin(); Console.begin()
#else
#define SERIAL Serial
#define SERIAL_BEGIN Serial.begin(9600)
#endif

#include <stdio.h>
char sp_buff[255];
#define SERIAL_PRINTF(format, ...) sprintf(sp_buff, format, __VA_ARGS__); SERIAL.print(sp_buff)

#define RX_DATA_PIN 2
#define RX_DATA_INT 0

#define TX_DATA_PIN 3
#define TX_POWER_PIN 4

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
volatile int last_state;

void receive_data() {
    int cur_state = digitalRead(RX_DATA_PIN);
    if (last_state == LOW && cur_state == HIGH) {
        last_state = HIGH;
        rising_time = millis();
    } else if (last_state == HIGH && cur_state == LOW) {
        last_state = LOW;
        unsigned long d = millis() - rising_time;

        if (d < 9L) return;

        switch (rx_state) {
            case FIND_LEAD:
                if (d > 25L && d < 35L) {
                    rx_state = LEAD1;
                    SERIAL_PRINTF("Begin lead-in (%ld).\n", d);
                }
                break;

            case LEAD1:
                if (d > 15 && d < 25L) {
                    rx_state = LEAD2;
                } else {
                    rx_state = FIND_LEAD;
                    SERIAL_PRINTF("Misled on 1 (%ld).\n", d);
                }
                break;
            
            case LEAD2:
                if (d < 15L) {
                    rx_state = LEAD3;
                } else {
                    rx_state = FIND_LEAD;
                    SERIAL_PRINTF("Mislead on 2 (%ld).\n", d);
                }
                break;
            
            case LEAD3:
                if (d > 25L && d < 35L) {
                    rx_state = IN_MSG;
                    SERIAL.println("Begin message.\n");
                } else {
                    rx_state = FIND_LEAD;
                    SERIAL_PRINTF("Mislead on 3 (%ld).\n", d);
                }
                break;
            
            case IN_MSG:
                if (d < 15L) {
                    SERIAL.print("0");
                } else if (d > 15 && d < 25L) {
                    SERIAL.print("1");
                } else if (d > 25L && d < 35L) {
                    SERIAL.println();
                    SERIAL.println("End message.\n");
                    rx_state = FIND_LEAD;
                }
                break;
        }
    }
}


void setup() {
  SERIAL_BEGIN;
  pinMode(TX_POWER_PIN, OUTPUT);
  digitalWrite(TX_POWER_PIN, LOW);
  rx_state = FIND_LEAD;
  rising_time = 0L;
  last_state = LOW;
  //pinMode(RX_DATA_PIN, INPUT);
  attachInterrupt(RX_DATA_INT, receive_data, CHANGE);
  SERIAL.println("Setup complete.");
}

void loop() {
  // The code is all in the interrupt handler
}
