/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "keypad.h"
#include "scheduler.h"
#endif

unsigned char output = 0x00;

enum Keypad_States { keypad_display };

int keypadTick(int state){
	unsigned char x;
	unsigned char led;
	x = GetKeypadKey();
	led = 0x00;
	switch(state){
		case keypad_display:
			state = keypad_display;
			break;
		default:
			state = keypad_display;
			break;
	}

	switch(state){
		case keypad_display:
			switch (x) {
				case '\0': output = 0x1F; led = 0x00; break; // All 5 LEDs on
				case '1': output = 0x01; led = 0x80; break; // hex equivalent
				case '2': output = 0x02; led = 0x80; break;
				case '3': output = 0x03; led = 0x80; break;
				case '4': output = 0x04; led = 0x80; break;
				case '5': output = 0x05; led = 0x80; break;
				case '6': output = 0x06; led = 0x80; break;
				case '7': output = 0x07; led = 0x80; break;
				case '8': output = 0x08; led = 0x80; break;
				case '9': output = 0x09; led = 0x80; break;
				case 'A': output = 0x0A; led = 0x80; break;
				case 'B': output = 0x0B; led = 0x80; break;
				case 'C': output = 0x0C; led = 0x80; break;
				case 'D': output = 0x0D; led = 0x80; break;
				case '*': output = 0x0E; led = 0x80; break;
				case '0': output = 0x00; led = 0x80; break;
				case '#': output = 0x0F; led = 0x80; break;
				default: output = 0x1B; break; // Should never occur. Middle LED off.
			}
			PORTB = output | led;
			break;
		}
		return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;

	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 10;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypadTick;

	TimerSet(10);
	TimerOn();

	unsigned short i;
    /* Insert your solution below */
	while (1) {
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 10;
		}	
		while(!TimerFlag){
			TimerFlag = 0;
		}
	}
    	return 0;
}
