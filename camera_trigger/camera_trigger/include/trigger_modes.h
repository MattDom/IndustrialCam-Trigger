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

const uint8_t number_leds = 3;

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

void low_freq()
{		  
	if (num_leds < number_leds)
	{
	if (time_delay_0 == low_value)
	{
		PORTB |= (1 << DDB4);
		num_leds++;
	}
	else if (time_delay_0 == low_value + 10)
	{
		PORTB &= ~(1 << DDB4);
		PORTB |= (1 << DDB0);
		num_leds++;
	}
	else if (time_delay_0 == low_value + 20)
	{
		PORTB &= ~(1 << DDB0);
		PORTB |= (1 << DDB2);
		num_leds++;
	}
	else if (time_delay_0 == low_value + 30)
	{
		PORTB &= ~(1 << DDB2);
		PORTD |= (1 << DDD7);
		num_leds++;
	}
	else if (time_delay_0 == low_value + 40)
	{
		PORTD &= ~(1 << DDD7);
		PORTB |= (1 << DDB3);
		num_leds++;
	}
	else if (time_delay_0 == low_value + 50)
	{
		PORTB &= ~(1 << DDB3);
		PORTD |= (1 << DDD5);
		num_leds++;
	}
	else if (time_delay_0 == low_value + 60)
	{
		PORTD &= ~(1 << DDD5);
		PORTD |= (1 << DDD4);
		num_leds++;
	}
	else if (time_delay_0 == low_value + 70)
	{
		PORTD &= ~(1 << DDD4);
	}
	}
}

void med_freq()
{		  
	if (num_leds < number_leds)
	{	
	if (time_delay_0 == med_value)
	{
		PORTB |= (1 << DDB4);
		num_leds++;
	}
	else if (time_delay_0 == med_value + 5)
	{
		PORTB &= ~(1 << DDB4);
		PORTB |= (1 << DDB0);
		num_leds++;
	}
	else if (time_delay_0 == med_value + 10)
	{
		PORTB &= ~(1 << DDB0);
		PORTB |= (1 << DDB2);
		num_leds++;
	}
	else if (time_delay_0 == med_value + 15)
	{
		PORTB &= ~(1 << DDB2);
		PORTD |= (1 << DDD7);
		num_leds++;
	}
	else if (time_delay_0 == med_value + 20)
	{
		PORTD &= ~(1 << DDD7);
		PORTB |= (1 << DDB3);
		num_leds++;
	}
	else if (time_delay_0 == med_value + 25)
	{
		PORTB &= ~(1 << DDB3);
		PORTD |= (1 << DDD5);
		num_leds++;
	}
	else if (time_delay_0 == med_value + 35)
	{
		PORTD &= ~(1 << DDD5);
		PORTD |= (1 << DDD4);
		num_leds++;
	}
	else if (time_delay_0 == med_value + 40)
	{
		PORTD &= ~(1 << DDD4);
	}    
	}
}

void high_freq()
{			  
	if (num_leds < number_leds)
	{
	if (time_delay_0 == high_value)
	{
		PORTB |= (1 << DDB4);
		num_leds++;
	}
	else if (time_delay_0 == high_value + 1)
	{
		PORTB &= ~(1 << DDB4);
		PORTB |= (1 << DDB0);
		num_leds++;
	}
	else if (time_delay_0 == high_value + 2)
	{
		PORTB &= ~(1 << DDB0);
		PORTB |= (1 << DDB2);
		num_leds++;
	}
	else if (time_delay_0 == high_value + 3)
	{
		PORTB &= ~(1 << DDB2);
		PORTD |= (1 << DDD7);
		num_leds++;
	}
	else if (time_delay_0 == high_value + 4)
	{
		PORTD &= ~(1 << DDD7);
		PORTB |= (1 << DDB3);
		num_leds++;
	}
	else if (time_delay_0 == high_value + 5)
	{
		PORTB &= ~(1 << DDB3);
		PORTD |= (1 << DDD5);
		num_leds++;
	}
	else if (time_delay_0 == high_value + 6)
	{
		PORTD &= ~(1 << DDD5);
		PORTD |= (1 << DDD4);
		num_leds++;
	}
	else if (time_delay_0 == high_value + 7)
	{
		PORTD &= ~(1 << DDD4);
	}	
	}
}

void loop_mode()
{
	switch(loop_mode_select % 3)
	{
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

	if (loop_count == 100)
	{
		loop_count = 0;
		loop_mode_select++;
	}
	
}

#endif /* TRIGGER_MODES_H_ */