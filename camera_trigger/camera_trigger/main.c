/*
 * IndustrialCam-trigger
 *
 * Created: 13.01.2019 10:00:23
 * Author : Matthias
 */

#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "trigger_modes.h"

enum { loop_, low_, medium_, high_ } mode;

uint8_t trigger = 0;
uint8_t pin_mode_ = 0;

uint8_t offset_ = 0;

bool button = false;
bool shot_done = false;
bool init_shot_ = true;

void single_shot();
/*void loop_mode();
void low_freq();
void med_freq();
void high_freq();*/

int main(void) {
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4);
  DDRC &= ~(1 << DDC0) | ~(1 << DDC1) | ~(1 << DDC2) | ~(1 << DDC3) |
          ~(1 << DDC4) | ~(1 << DDC4);
  DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
  DDRD &= ~(1 << DDD2) & ~(1 << DDD3);

  // Timer 1 konfigurieren
  TCCR1A |= (1 << COM1A1);  // Toggle OC1A/OC1B on Compare Match
  TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);
  TCCR1B |= (1 << WGM12);  // CTC-Mode
  OCR1A = 249;             // Plus eins für den ÜberlauF!

  // Compare Interrupt erlauben
  TIMSK1 |= (1 << OCIE1A);

  EICRA |= ((1 << ISC01) | (1 << ISC00));  // Enable INT0

  // Global Interrupts aktivieren
  sei();

  TCNT1H = 0x0;
  TCNT1L = 0x0;
  TCCR1B |= (1 << CS11) | (1 << CS10);  // Timer 1 aktivieren (Prescaler auf 64)

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
      pin_mode_ |= ((PINC & (1 << DDC1)) << 1);
      pin_mode_ |= ((PINC & (1 << DDC0)));

      // PC3 - PC5
      offset_ |= ((PINC & (1 << DDC5)) << 3);
      offset_ |= ((PINC & (1 << DDC4)) << 2);
      offset_ |= ((PINC & (1 << DDC3)) << 1);
      offset_ |= (PINC & (1 << DDC2));

      // Switch to selcted mode
      switch (pin_mode_) {
        case (0):
          mode = loop_;
          break;
        case 1:
          mode = low_;
          switch (offset_) {
            case 0:
              low_temp_ = -20;
              break;
            case 1:
              low_temp_ = -10;
              break;
            case 2:
              break;
            case 3:
              low_temp_ = 10;
              break;
            case 4:
              low_temp_ = 20;
              break;
            default:
              break;
          }

          low_value = low_base_ + low_temp_;
          break;
        case 2:
          mode = medium_;
          med_value += low_value;
          switch (offset_) {
            case 0:
              med_temp_ = -10;
              break;
            case 1:
              med_temp_ = -5;
              break;
            case 2:
              break;
            case 3:
              med_temp_ = 5;
              break;
            case 4:
              med_temp_ = 10;
              break;
            default:
              break;
          }

          med_value = med_base_ + med_temp_ + low_value;
          break;
        case 3:
          mode = high_;
          high_value += med_value;
          switch (offset_) {
            case 0:
              high_temp_ = -2;
              break;
            case 1:
              high_temp_ = -1;
              break;
            case 2:
              break;
            case 3:
              high_temp_ = 1;
              break;
            case 4:
              high_temp_ = 2;
              break;
            default:
              break;
          }

          high_value = high_base_ + high_temp_ + med_value;
          break;
        default:
          break;
      }  // switch-case
    }    // if...else
  }      // while()
}  // main()

ISR(TIMER1_COMPA_vect) {
  time_delay_0++;

  if (mode == loop_) {
    loop_mode();
  } else if (mode == low_) {
    low_freq();
  } else if (mode == medium_) {
    med_freq();
  } else if (mode == high_) {
    high_freq();
  }

  // ####### 100ms Periode #######
  if (time_delay_0 <= 10) {
    PORTD &= ~(1 << DDD6);
  } else {
    PORTD |= (1 << DDD6);  // Fire trigger
    if (time_delay_0 == 100) {
      time_delay_0 = 0;
      shot_done = true;
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