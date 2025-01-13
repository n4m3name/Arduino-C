/* a4.c
 * CSC Fall 2022
 * 
 * Student name:
 * Student UVic ID:
 * Date of completed work:
 *
 *
 * Code provided for Assignment #4
 *
 * Author: Mike Zastre (2022-Nov-22)
 *
 * This skeleton of a C language program is provided to help you
 * begin the programming tasks for A#4. As with the previous
 * assignments, there are "DO NOT TOUCH" sections. You are *not* to
 * modify the lines within these section.
 *
 * You are also NOT to introduce any new program-or file-scope
 * variables (i.e., ALL of your variables must be local variables).
 * YOU MAY, however, read from and write to the existing program- and
 * file-scope variables. Note: "global" variables are program-
 * and file-scope variables.
 *
 * UNAPPROVED CHANGES to "DO NOT TOUCH" sections could result in
 * either incorrect code execution during assignment evaluation, or
 * perhaps even code that cannot be compiled.  The resulting mark may
 * be zero.
 */


/* =============================================
 * ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
 * =============================================
 */

#define __DELAY_BACKWARD_COMPATIBLE__ 1
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY1 0.000001
#define DELAY3 0.01

#define PRESCALE_DIV1 8
#define PRESCALE_DIV3 64
#define TOP1 ((int)(0.5 + (F_CPU/PRESCALE_DIV1*DELAY1))) 
#define TOP3 ((int)(0.5 + (F_CPU/PRESCALE_DIV3*DELAY3)))

#define PWM_PERIOD ((long int)500)

volatile long int count = 0;
volatile long int slow_count = 0;


ISR(TIMER1_COMPA_vect) {
	count++;
}


ISR(TIMER3_COMPA_vect) {
	slow_count += 5;
}

/* =======================================
 * ==== END OF "DO NOT TOUCH" SECTION ====
 * =======================================
 */


/* *********************************************
 * **** BEGINNING OF "STUDENT CODE" SECTION ****
 * *********************************************
 */

void led_state(uint8_t LED, uint8_t state) {
	// For each if, check LED # and state (on or off).
	// AND / OR to PORTL's current val accordingly to display
	 if (LED == 0 && state >= 1){
		PORTL |= 0b10000000;
	}
	else if (LED== 0 && state == 0){
		PORTL &= 0b01111111;
	}
	else if (LED == 1 && state >= 1){
		PORTL |= 0b00100000;
	}
	else if (LED == 1 && state == 0){
		PORTL &= 0b11011111;
	}
	else if (LED == 2 && state >= 1){
		PORTL |= 0b00001000;
	}
	else if (LED == 2 && state == 0){
		PORTL &= 0b11110111;
	}
	else if (LED == 3 && state >= 1){
		PORTL |= 0b00000010;
	}
	else if (LED == 3 && state == 0){
		PORTL &= 0b11111101;
	}
	else {
		PORTL |= 0b00000000;
	}
}



void SOS() {
    uint8_t light[] = {
        0x1, 0, 0x1, 0, 0x1, 0,
        0xf, 0, 0xf, 0, 0xf, 0,
        0x1, 0, 0x1, 0, 0x1, 0,
        0x0
    };

    int duration[] = {
        100, 250, 100, 250, 100, 500,
        250, 250, 250, 250, 250, 500,
        100, 250, 100, 250, 100, 250,
        250
    };
	// Length for for loop
	int length = 19;
	for (int i = 0; i<length; i++){
		// If value is ox<>, display desired led for duration[i]
		if (light[i] == 0x1){
			led_state(0, 1);
			_delay_ms(duration[i]);
			led_state(0, 0);
			_delay_ms(duration[i]);
		} // ...
		else if (light[i] == 0xf){
			led_state(0, 1);
			led_state(1, 1);
			led_state(2, 1);
			led_state(3, 1);
			_delay_ms(duration[i]);
		}
		// If 0, turn off all LED's for duration duration[i]
		else if (light[i] == 0){
			led_state(0, 0);
			led_state(1, 0);
			led_state(2, 0);
			led_state(3, 0);
			_delay_ms(duration[i]);
		}
	}
}

void glow(uint8_t LED, float brightness) {
	// Create threshold
	float thrsh = PWM_PERIOD * brightness;
	// Turns on/off LED repeatedly according to threshold and PWM_PERIOD
	for (;;){
		// If count < threshold, turn on LED
		if (count<thrsh){
			led_state(LED, 1); // Turn on
		}
		// If count < threshold (between threshold and PWM_PERIOD), turn off LED
		else if (count<PWM_PERIOD){
			led_state(LED, 0);
		}
		// If above threshold, reset count
		else {
			count = 0;
		}
	}
}



void pulse_glow(uint8_t LED) {
	float brightness = 0; // Init brightness
	float mult = 0.0002; // Determines speed of modulation
	for(;;){
		float thrsh = PWM_PERIOD * brightness; //... See above
		if (count<thrsh){
			led_state(LED, 1); //...
		}
		else if (count<PWM_PERIOD){
			led_state(LED, 0); //...
		}
		else {
			count = 0; //...
		}
		// Added: Continually increase brightness until
		// slow_count*mult >= 1
		if (slow_count*mult < 1){
			brightness = slow_count*mult;
		}
		// While 1 < slow_count*mult < 2, decrease brightness
		else if (slow_count*mult < 2){
			brightness = 2 - slow_count*mult;
		}
		// Once slow_count*mult >= 2, reset count
		else {
			slow_count = 0;
		}
	}
}


void light_show() { // Same code as SOS()
    uint8_t light[] = {
	    1, 0, 1, 0, 1, 0, 3, 0, 2, 0, 
		1, 0, 1, 0, 1, 0, 2, 0, 3, 0, 
		4,
		1, 0, 1, 0, 3, 0, 3, 0, 
    };

    int duration[] = {
	    200, 100, 200, 100, 200, 100, 100, 100, 100, 100,
		200, 100, 200, 100, 200, 100, 100, 100, 100, 100,
		0,
		200, 100, 200, 100, 300, 100, 300, 100
    };
	
	int n = 29;
	for (int i = 0; i<n; i++){
		if (light[i] == 1){
			led_state(0, 1);
			led_state(1, 1);
			led_state(2, 1);
			led_state(3, 1);
			_delay_ms(duration[i]);
		}
		else if (light[i] == 2){
			led_state(0, 1);
			led_state(1, 0);
			led_state(2, 0);
			led_state(3, 1);
			_delay_ms(duration[i]);
		}
		else if (light[i] == 3){
			led_state(0, 0);
			led_state(1, 1);
			led_state(2, 1);
			led_state(3, 0);
			_delay_ms(duration[i]);
		}
		else if (light[i] == 4){
			led_state(3,1);
			_delay_ms(80);
			led_state(2,1);
			_delay_ms(80);
			led_state(3,0);
			led_state(1,1);
			_delay_ms(80);
			led_state(2,0);
			led_state(0,1);
			_delay_ms(80);
			led_state(1,0);
			_delay_ms(80);
			led_state(1,1);
			_delay_ms(80);
			led_state(0,0);
			led_state(2,1);
			_delay_ms(80);
			led_state(1,0);
			led_state(3,1);
			_delay_ms(80);
			led_state(2,0);
			_delay_ms(80);
			led_state(2,1);
			_delay_ms(80);
			led_state(3,0);
			led_state(1,1);
			_delay_ms(80);
			led_state(2,0);
			led_state(0,1);
			_delay_ms(80);
			led_state(1,0);
			_delay_ms(80);
			led_state(1,1);
			_delay_ms(80);
			led_state(0,0);
			led_state(2,1);
			_delay_ms(80);
			led_state(1,0);
			led_state(3,1);
			_delay_ms(80);
			led_state(2,0);
			_delay_ms(80);
			led_state(3,0);
			_delay_ms(100);
		}
		else {
			led_state(0, 0);
			led_state(1, 0);
			led_state(2, 0);
			led_state(3, 0);
			_delay_ms(duration[i]);
		}
	}	
}


/* ***************************************************
 * **** END OF FIRST "STUDENT CODE" SECTION **********
 * ***************************************************
 */


/* =============================================
 * ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
 * =============================================
 */

int main() {
    /* Turn off global interrupts while setting up timers. */

	cli();

	/* Set up timer 1, i.e., an interrupt every 1 microsecond. */
	OCR1A = TOP1;
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12);
    /* Next two lines provide a prescaler value of 8. */
	TCCR1B |= (1 << CS11);
	TCCR1B |= (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);

	/* Set up timer 3, i.e., an interrupt every 10 milliseconds. */
	OCR3A = TOP3;
	TCCR3A = 0;
	TCCR3B = 0;
	TCCR3B |= (1 << WGM32);
    /* Next line provides a prescaler value of 64. */
	TCCR3B |= (1 << CS31);
	TIMSK3 |= (1 << OCIE3A);


	/* Turn on global interrupts */
	sei();

/* =======================================
 * ==== END OF "DO NOT TOUCH" SECTION ====
 * =======================================
 */


/* *********************************************
 * **** BEGINNING OF "STUDENT CODE" SECTION ****
 * *********************************************
 */

// Note: All tests commented out when handed in.

// This code could be used to test your work for part A.
/*
	led_state(0, 1);
	_delay_ms(1000);
	led_state(2, 1);
	_delay_ms(1000);
	led_state(1, 1);
	_delay_ms(1000);
	led_state(2, 0);
	_delay_ms(1000);
	led_state(0, 0);
	_delay_ms(1000);
	led_state(1, 0);
	_delay_ms(1000);
*/


// This code could be used to test your work for part B.
//	SOS();

 


// This code could be used to test your work for part C.
//	glow(2, 0.1);



// This code could be used to test your work for part D.
//	pulse_glow(3);


// This code could be used to test your work for the bonus part.
//	light_show();

/* ****************************************************
 * **** END OF SECOND "STUDENT CODE" SECTION **********
 * ****************************************************
 */
}
