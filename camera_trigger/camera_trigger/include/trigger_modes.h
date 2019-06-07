/*
 * trigger_modes.h
 *
 * Created: 28.04.2019 16:06:02
 *  Author: Matthias
 */

#ifndef TRIGGER_MODES_H_
#define TRIGGER_MODES_H_

const int8_t low_base_ = 10;
const int8_t med_base_ = 0;
const int8_t high_base_ = 0;

const uint8_t number_leds = 4;

int8_t low_temp_ = 0;
int8_t med_temp_ = 0;
int8_t high_temp_ = 0;

int8_t low_value = 0;
int8_t med_value = 0;
int8_t high_value = 0;

uint8_t time_delay_0 = 0;
uint8_t loop_mode_select = 0;
uint8_t loop_count = 0;
uint8_t num_leds = 0;
uint8_t time_ms_ = 0;
uint8_t offset_med[7] = {0, 5, 10, 15, 20, 25, 30};
uint8_t offset_high[7] = {0, 1, 2, 3, 4, 5, 6};

void low_freq() {
  PORTD &= ~(0xBF);
  PORTB &= ~(0x1F);

  time_ms_ = time_delay_0 + low_value;

  if (num_leds < number_leds) {
    if (time_ms_ == 0) {
      PORTB |= (1 << DDB4);
      num_leds++;
    } else if (time_ms_ == 10) {
      PORTB |= (1 << DDB0);
      num_leds++;
    } else if (time_ms_ == 20) {
      PORTB |= (1 << DDB2);
      num_leds++;
    } else if (time_ms_ == 30) {
      PORTD |= (1 << DDD7);
      num_leds++;
    } else if (time_ms_ == 40) {
      PORTB |= (1 << DDB3);
      num_leds++;
    } else if (time_ms_ == 50) {
      PORTD |= (1 << DDD5);
      num_leds++;
    } else if (time_ms_ == 60) {
      PORTD |= (1 << DDD4);
      num_leds++;
    }
  }
}

void med_freq() {
  PORTD &= ~(0xBF);
  PORTB &= ~(0x1F);

  time_ms_ = time_delay_0 + med_value;

  if (num_leds < number_leds) {
    if (time_ms_ == offset_med[0]) {
      PORTB |= (1 << DDB4);
      num_leds++;
    } else if (time_ms_ == (offset_med[1] + low_value)) {
      PORTB |= (1 << DDB0);
      num_leds++;
    } else if (time_ms_ == (offset_med[2] + low_value)) {
      PORTB |= (1 << DDB2);
      num_leds++;
    } else if (time_ms_ == (offset_med[3] + low_value)) {
      PORTD |= (1 << DDD7);
      num_leds++;
    } else if (time_ms_ == (offset_med[4] + low_value)) {
      PORTB |= (1 << DDB3);
      num_leds++;
    } else if (time_ms_ == (offset_med[5] + low_value)) {
      PORTD |= (1 << DDD5);
      num_leds++;
    } else if (time_ms_ == (offset_med[6] + low_value)) {
      PORTD |= (1 << DDD4);
      num_leds++;
    }
  }
}

void high_freq() {
  PORTD &= ~(0xBF);
  PORTB &= ~(0x1F);

  time_ms_ = time_delay_0 + high_value;  

  if (num_leds < number_leds) {
    if (time_ms_ == (offset_high[0] + low_value + med_value)) {
      PORTB |= (1 << DDB4);
      num_leds++;
    } else if (time_ms_ == (offset_high[1] + low_value + med_value)) {
      PORTB |= (1 << DDB0);
      num_leds++;
    } else if (time_ms_ == (offset_high[2] + low_value + med_value)) {
      PORTB |= (1 << DDB2);
      num_leds++;
    } else if (time_ms_ == (offset_high[3] + low_value + med_value)) {
      PORTD |= (1 << DDD7);
      num_leds++;
    } else if (time_ms_ == (offset_high[4] + low_value + med_value)) {
      PORTB |= (1 << DDB3);
      num_leds++;
    } else if (time_ms_ == (offset_high[5] + low_value + med_value)) {
      PORTD |= (1 << DDD5);
      num_leds++;
    } else if (time_ms_ == (offset_high[6] + low_value + med_value)) {
      PORTD |= (1 << DDD4);
      num_leds++;
    }
  }
}

void loop_mode() {
  switch (loop_mode_select % 3) {
    case 0:
      low_freq();
      loop_count++;
      break;
    case 1:
      med_freq();
      loop_count++;
      break;
    case 2:
      high_freq();
      loop_count++;
      break;
  }

  if (loop_count == 100) {
    loop_count = 0;
    loop_mode_select++;
  }
}

#endif /* TRIGGER_MODES_H_ */