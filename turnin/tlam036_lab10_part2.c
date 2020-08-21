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
unsigned char door = 0x00;
unsigned char led = 0x00;

enum Keypad_States { keypad_display };

int keypadTick(int state){
	unsigned char x;
	x = GetKeypadKey();

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
				case '\0': output = 0x1F; break; // All 5 LEDs on
				case '1': output = 0x01; break; // hex equivalent
				case '2': output = 0x02; break;
				case '3': output = 0x03; break;
				case '4': output = 0x04; break;
				case '5': output = 0x05; break;
				case '6': output = 0x06; break;
				case '7': output = 0x07; break;
				case '8': output = 0x08; break;
				case '9': output = 0x09; break;
				case 'A': output = 0x0A; break;
				case 'B': output = 0x0B; break;
				case 'C': output = 0x0C; break;
				case 'D': output = 0x0D; break;
				case '*': output = 0x0E; break;
				case '0': output = 0x00; break;
				case '#': output = 0x0F; break;
				default: output = 0x1B; break; // Should never occur. Middle LED off.
			}
			break;
		}
		return state;
}

enum Sequence_States { wait, poundPress, poundRelease, 
			waitSeq1, seq1Press, seq1Release, 
			waitSeq2, seq2Press, seq2Release, 
			waitSeq3, seq3Press, seq3Release, 
			waitSeq4, seq4Press, seq4Release, 
			waitSeq5, seq5Press, seq5Release,
			waitLock, lock};

int sequenceTick(int state){
	unsigned char button = ~PINB & 0x80;

	switch(state){

		case wait:
			state = (output == 0x0F) ? poundPress : wait;
			break;
		case poundPress:
			state = poundRelease;
			break;
		case poundRelease:
			state = (output == 0x0F) ? poundPress : waitSeq1;
			break;


		case waitSeq1:
			if(output == 0x1F) {
				state = waitSeq1;
			}
			else if(output == 0x01){
				state = seq1Press;
			}
			else{
				state = wait;
			}
			break;
		case seq1Press:
			state = seq1Release;
			break;
		case seq1Release:
			state = (output == 0x01) ? seq1Press : waitSeq2;
			break;

		case waitSeq2:
                        if(output == 0x1F) {
                                state = waitSeq2;
                        }
                        else if(output == 0x02){
                                state = seq2Press;
                        }
                        else{
                                state = wait;
                        }
                        break;
                case seq2Press:
                        state = seq2Release;
                        break;
                case seq2Release:
                        state = (output == 0x02) ? seq2Press : waitSeq3;
                        break;
		
		 case waitSeq3:
                        if(output == 0x1F) {
                                state = waitSeq3;
                        }
                        else if(output == 0x03){
                                state = seq3Press;
                        }
                        else{
                                state = wait;
                        }
                        break;
                case seq3Press:
                        state = seq3Release;
                        break;
                case seq3Release:
                        state = (output == 0x03) ? seq3Press : waitSeq4;
                        break;

		 case waitSeq4:
                        if(output == 0x1F) {
                                state = waitSeq4;
                        }
                        else if(output == 0x04){
                                state = seq4Press;
                        }
                        else{
                                state = wait;
                        }
                        break;
                case seq4Press:
                        state = seq4Release;
                        break;
                case seq4Release:
                        state = (output == 0x04) ? seq4Press : waitSeq5;
                        break;

                 case waitSeq5:
                        if(output == 0x1F) {
                                state = waitSeq5;
                        }
                        else if(output == 0x05){
                                state = seq5Press;
                        }
                        else{
                                state = wait;
                        }
                        break;
                case seq5Press:
                        state = seq5Release;
                        break;
                case seq5Release:
                        state = (output == 0x05) ? seq5Press : waitLock;
                        break;

		case waitLock:
			state = button ? lock : waitLock;
			break;

		case lock:
			state = wait;
			break;

		default:
			state = wait;
			break;
	}

	switch(state){

		case wait:
			break;
		case poundPress:
			led = 0x40;
			break;
		case poundRelease:
			led = 0x00;
			break;

		case waitSeq1:
			break;
		case seq1Press:
			led = 0x40;
			break;
		case seq1Release:
			led = 0x00;
			break;

		case waitSeq2:
                        break;
                case seq2Press:
                        led = 0x40;
                        break;
                case seq2Release:
                        led = 0x00;
                        break;

		case waitSeq3:
                        break;
                case seq3Press:
                        led = 0x40;
                        break;
                case seq3Release:
                        led = 0x00;
                        break;

		case waitSeq4:
                        break;
                case seq4Press:
                        led = 0x40;
                        break;
                case seq4Release:
                        led = 0x00;
                        break;

                case waitSeq5:
                        break;
                case seq5Press:
                        led = 0x40;
                        break;
                case seq5Release:
                        led = 0x00;
			door = 1;
                        break;
		
		case waitLock:
			break;

		case lock:
			door = 0;
			break;

		default:
			break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x7F; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;

	static task task1, task2 ;
	task *tasks[] = { &task1, &task2 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 1;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypadTick;

	task2.state = start;
        task2.period = 1;
        task2.elapsedTime = task2.period;
        task2.TickFct = &sequenceTick;

	TimerSet(1);
	TimerOn();

	unsigned short i;
    /* Insert your solution below */
	while (1) {
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		PORTB = led | door;	
		while(!TimerFlag){
			TimerFlag = 0;
		}
	}

    	return 0;
}
