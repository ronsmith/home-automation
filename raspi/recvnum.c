// recvnum.c
// Receive a number transmitted on the 433MHz radio
// Author:  Ron Smith
// Copyright (C)2015, That Ain't Working, All Rights Reserved

#include <stdio.h>
#include <time.h>
#include <wiringPi.h>


#define FALSE 0
#define TRUE (!FALSE)

#define FIND_LEAD 0
#define LEAD1 1
#define LEAD2 2
#define LEAD3 3
#define IN_MSG 4


int main(void) {
	int lastState = LOW;
	int curState = LOW;
	int state = FIND_LEAD;
	long t, d;

	printf("Setup wiringPi to read radio on pin 4.\n");
	wiringPiSetupGpio();
	pinMode(4, INPUT);
	pullUpDnControl(4, PUD_DOWN);

//	if (!piHiPri(2)) {
//		printf("Enabled high priority mode.\n");
//	} else {
//		printf("Unable to set high priority mode.\n");
//	}
	
	printf("Begin main loop.\n");
	t = 0L;
	d = 0L;
	for(;;) {  // outer loop
		curState = digitalRead(4);
		if (lastState == LOW && curState  == HIGH) {
			lastState = HIGH;
			t = millis();
		} else if (lastState == HIGH && curState == LOW) {
			lastState = LOW;
			d = millis() - t;

			if (d < 9L) continue;

			switch(state) {
				case FIND_LEAD:
					if (d > 25L && d < 35L) {
						state = LEAD1;
						//printf("Begin lead-in (%ld).\n", d);
					}
					break;

				case LEAD1:
					if (d > 15 && d < 25L) {
						state = LEAD2;
					} else {
						state = FIND_LEAD;
						//printf("Misled on 1 (%ld).\n", d);
					}
					break;

				case LEAD2:
					if (d < 15L) {
						state = LEAD3;
					} else {
						state = FIND_LEAD;
						//printf("Mislead on 2 (%ld).\n", d);
					}
					break;

				case LEAD3:
					if (d > 25L && d < 35L) {
						state = IN_MSG;
						//printf("Begin message.\n");
					} else {
						state = FIND_LEAD;
						//printf("Mislead on 3 (%ld).\n", d);
					}
					break;
				
				case IN_MSG:
					printf(" (%ld)", d);
					if (d < 15L) {
						printf("0");
					} else if (d > 15 && d < 25L) {
						printf("1");
					} else if (d >= 25L) {
						//printf("\n");
						printf(" End message.\n");
						state = FIND_LEAD;
					}
					break;
			}
		}
	}
}

