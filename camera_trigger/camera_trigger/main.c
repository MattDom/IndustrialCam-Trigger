/*
* IndustrialCam-trigger
*
* Created: 13.01.2019 10:00:23
* Author : Matthias Domnik
*/

#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include <util/delay.h>

#include <trigger_modes.h>

enum { loop_, low_, medium_, high_ } mode;

uint8_t trigger = 0;
uint8_t pin_mode_ = 0;
uint8_t offset_ = 0;
uint8_t framerate = 100; // Time-intervall in ms --> framerate = 1/FPS*1000

bool button = false;
bool shot_done = false;
bool init_shot_ = true;

void single_shot();

int main(void) {
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4);
    DDRC = 0x00;
    PORTC = 0xFF;
    DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
    DDRD &= ~(1 << DDD2) & ~(1 << DDD3);
    
    // Configure Timer 1
    TCCR1A |= (1 << COM1A1);  // Toggle OC1A/OC1B on Compare Match
    TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10); // Clear prescaler (just to be save)
    TCCR1B |= (1 << WGM12);  // CTC-Mode
    OCR1A = 249;             // Set compare-value

    TIMSK1 |= (1 << OCIE1A); // Activate compare interrupt
    EICRA |= ((1 << ISC01) | (1 << ISC00));  // Enable INT0

    sei();

    TCNT1H = 0x0; // Initialize timer counter...
    TCNT1L = 0x0; // ... to be save
    TCCR1B |= (1 << CS11) | (1 << CS10);  // Set the prescaler to 64

    while (1) {
        if (PIND & (1 << DDD3))  // Check for single-shot-mode
        {
            if (button) {
                if (shot_done) {
                    TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);
                    button = false;
                    shot_done = false;
                    TCNT1H = 0x0;
                    TCNT1L = 0x0;
                    time_delay_0 = 0;
                    PORTD &= 0x4F;
                    PORTB &= 0x1F;
                }
                } else if (init_shot_) {
                TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);
                PORTD |= (1 << DDD6);
                EIMSK |= (1 << INT0);
                PORTD &= 0x4F;
                PORTB &= 0x1F;
                init_shot_ = false;
            }
            } else {
            EIMSK &= ~(1 << INT0);                // Disable EXT0
            TCCR1B |= (1 << CS11) | (1 << CS10);  // Enable Timer1
            init_shot_ = true;                    // Reset flag

            // PC0 - PC1
            pin_mode_ = 0;
            pin_mode_ |= (PINC & (1 << DDC1));
            pin_mode_ |= (PINC & (1 << DDC0));

            // PC3 - PC5
            offset_ = 0;
            offset_ |= ((PINC & (1 << DDC5))>>2) | ((PINC & (1 << DDC4))>>2) | ((PINC & (1 << DDC3))>>2) | ((PINC & (1 << DDC2))>>2);

            // Switch to selected mode
            switch (pin_mode_) {
                case 0:
                mode = loop_;
                break;
                case 1:
                mode = low_;
                switch (offset_) {
                    case 0:
                    low_temp_ = -10;
                    break;
                    case 1:
                    low_temp_ = 0;
                    break;
                    case 2:
                    low_temp_ = 10;
                    break;
                    case 3:
                    low_temp_ = 20;
                    break;
                    case 4:
                    low_temp_ = 30;
                    break;
                    case 5:
                    low_temp_ = 40;
                    break;
                    case 6:
                    low_temp_ = 50;
                    break;
                    case 7:
                    low_temp_ = 60;
                    break;
                    case 8:
                    low_temp_ = 70;
                    break;
                    default:
                    low_temp_ = 0;
                    break;
                }
                
                low_value = low_base_ + low_temp_;
                             
                break;
                
                case 3:
                mode = medium_;
                //med_value += low_value;
                switch (offset_) {
                    case 0:
                    med_temp_ = 0;
                    break;
                    case 1:
                    med_temp_ = 5;
                    break;
                    case 2:
                    med_temp_ = 10;
                    break;
                    case 3:
                    med_temp_ = 15;
                    break;
                    case 4:
                    med_temp_ = 20;
                    break;
                    default:
                    med_temp_ = 25;
                    break;
                }

                med_value = med_base_ + med_temp_;
                
                break;
                case 2:
                mode = high_;
                //high_value += med_value;
                switch (offset_) {
                    case 0:
                    high_temp_ = 0;
                    break;
                    case 1:
                    high_temp_ = -2;
                    break;
                    case 2:
                    high_temp_ = -1;
                    break;
                    case 3:
                    high_temp_ = 1;
                    break;
                    case 4:
                    high_temp_ = 2;
                    break;
                    default:
                    high_temp_ = 0;
                    break;
                }
                
                high_value = high_base_ + high_temp_;
                
                break;
                
                default:
                break;
            }  // switch-case
        }    // if...else
        //sleep_mode(); // Take a nap
    }   // while()
}  // main()

ISR(TIMER1_COMPA_vect)
{
      if (mode == loop_) {
        //loop_mode();
        } else if (mode == low_) {
        low_freq();
        } else if (mode == medium_) {
        med_freq();
        } else if (mode == high_) {
        high_freq();
    }
    time_delay_0++;
    // ####### 100ms Periode #######
    if (time_delay_0 <= 10) {
        PORTD &= ~(1 << DDD6);
        } else {
        PORTD |= (1 << DDD6);  // Fire trigger
        if (time_delay_0 == framrate) {
            time_delay_0 = 0;
            shot_done = true;
            num_leds = 0;
        }
    }
}

ISR(INT0_vect) {
    while (PIND & (1 << DDD2)) {
        _delay_ms(1);
    };
    if (!button) {
        button = true;
        single_shot();
    }
}

void single_shot() {
    shot_done = false;
    TCNT1H = 0x0;
    TCNT1L = 0x0;
    time_delay_0 = 0;
    TCCR1B |= (1 << CS11) | (1 << CS10);
}
