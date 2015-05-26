void setup() {
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);    // power on the transmitter
    delay(10);                // give it some time to settle
}


void write_lead(int pin) {
    digitalWrite(pin, HIGH);
    delay(30);                
    digitalWrite(pin, LOW);
    delay(10);
}

void write0(int pin) {
//    for (int x = 0; x < 3; x++) {
        digitalWrite(pin, HIGH);
        delay(10);
        digitalWrite(pin, LOW);
        delay(10);
//    }
}

void write1(int pin) {
//    for (int x = 0; x < 3; x++) {
        digitalWrite(pin, HIGH);
        delay(20);
        digitalWrite(pin, LOW);
        delay(10);
//    }
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
    write_lead_in(3);
    write1(3);
    write0(3);
    write0(3);
    write1(3);   
    write_lead_out(3); 
    delay(3000);
}

