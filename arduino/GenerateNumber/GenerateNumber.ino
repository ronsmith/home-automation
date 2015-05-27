/************************************************************
 * ReceiveNumber
 * A sketch to receive number from the GenerateNumber sketch.
 * Author: Ron Smith
 * Copyright (c)2015, That Ain't Working, All Rights Reserved
 ************************************************************/

#define RX_DATA_PIN 2
#define RX_DATA_INT 0

#define TX_DATA_PIN 3
#define TX_POWER_PIN 4

/** NOTE:
 ** I thought it would be good to be able to turn the 
 ** transmitter off when I wasn't transmitting.  Turns out, 
 ** turning it on/off creates a lot of noise on the channel.
 **/

void setup() {
    pinMode(TX_POWER_PIN, OUTPUT);
    digitalWrite(TX_POWER_PIN, HIGH);    // power on the transmitter
    delay(10);                           // give it some time to settle
    pinMode(TX_DATA_PIN, OUTPUT);
}


void write_lead(int pin) {
    digitalWrite(pin, HIGH);
    delay(30);                
    digitalWrite(pin, LOW);
    delay(10);
}

void write0(int pin) {
    digitalWrite(pin, HIGH);
    delay(10);
    digitalWrite(pin, LOW);
    delay(10);
}

void write1(int pin) {
    digitalWrite(pin, HIGH);
    delay(20);
    digitalWrite(pin, LOW);
    delay(10);
}

void write_lead_in(int pin) {
    write_lead(pin);
    write1(pin);
    write0(pin);
    write_lead(pin);
}    

void write_lead_out(int pin) {
    write_lead(pin);
}    

void loop() {
    write_lead_in(TX_DATA_PIN);
    write1(TX_DATA_PIN);
    write0(TX_DATA_PIN);
    write0(TX_DATA_PIN);
    write1(TX_DATA_PIN);   
    write_lead_out(TX_DATA_PIN); 
    delay(3000);
}

