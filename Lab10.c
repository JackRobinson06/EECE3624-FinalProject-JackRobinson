#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include "UART0Library.h"

#define BaudRate 9600
#define DataBits 8
#define StopBits 1
#define NoParity 0

#define UP PB3
#define DOWN PB1
#define CENTER PB4
#define LEFT PB2
#define RIGHT PB0

int main(void)
{
    unsigned char wiggle = 0;
    unsigned char lastButton = 0;
    unsigned char button;
    char command;

    // Set joystick pins as inputs
    DDRB &= ~((1 << UP) | (1 << DOWN) | (1 << CENTER) | (1 << LEFT) | (1 << RIGHT));

    // Turn on pull-up resistors
    PORTB |= (1 << UP) | (1 << DOWN) | (1 << CENTER) | (1 << LEFT) | (1 << RIGHT);

    uart0_init(BaudRate, DataBits, StopBits, NoParity);

    while (1)
    {
        // Check center button
        button = !(PINB & (1 << CENTER));

        if (button == 1 && lastButton == 0)
        {
            wiggle = !wiggle;
        }

        lastButton = button;

        // Choose command to send
        if (wiggle == 1)
            command = 'A';
        else if (!(PINB & (1 << UP)))
            command = 'U';
        else if (!(PINB & (1 << DOWN)))
            command = 'D';
        else if (!(PINB & (1 << LEFT)))
            command = 'L';
        else if (!(PINB & (1 << RIGHT)))
            command = 'R';
        else
            command = 'S';

        uart0_tx(command);

        _delay_ms(50);
    }
}