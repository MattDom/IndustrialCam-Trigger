/*
 * IndustrialCam-trigger
 *
 * Created: 13.01.2019 10:00:23
 * Author : Matthias
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

//#define coarse
//#define fine
#define  super_fine

uint8_t time_delay_0 = 0;
uint8_t time_delay_1 = 0;
uint8_t trigger = 0;

bool  button =  false;
bool shot_done = false;
bool init_shot_ = true;

void single_shot();

int main(void)
{
 	DDRB |= (1 << DDB0) |(1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4);
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	DDRD &= ~(1 << DDD2) & ~(1 << DDD3);
		
	// Timer 1 konfigurieren
	TCCR1A |= (1<<COM1A1); // Toggle OC1A/OC1B on Compare Match
	TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10);
	TCCR1B |= (1<<WGM12); // CTC-Mode
	OCR1A = 249; // Plus eins für den ÜberlauF!

	// Compare Interrupt erlauben
	TIMSK1 |= (1<<OCIE1A);
	
	EICRA |= ((1 << ISC01) | (1 << ISC00));	// Enable INT0
	
	// Global Interrupts aktivieren
	sei();
	
	TCNT1H = 0x0;
	TCNT1L = 0x0; 
	TCCR1B |= (1<<CS11) | (1<<CS10); // Timer 1 aktivieren (Prescaler auf 64)

    while (1) 
    {
 		if (PIND & (1 << DDD3))
 		{						
			if (button)
			{
				if (shot_done)
				{
					TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10);
					button = false;
					shot_done = false;
					TCNT1H = 0x0;
					TCNT1L = 0x0;
					time_delay_0 = 0;					
					PORTD &= 0x4F;
					PORTB &= 0x1F;			
				}
			}
			else if (init_shot_)
			{
				TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10);
				PORTD |= (1 << DDD6);
				EIMSK |= (1 << INT0);
				PORTD &= 0x4F;
				PORTB &= 0x1F;					
				init_shot_ = false;			
			}							
		}
		else
		{		
			EIMSK &= ~(1 << INT0);	
			TCCR1B |= (1<<CS11) | (1<<CS10);
			init_shot_ = true;					
		}
    }
}

ISR (TIMER1_COMPA_vect)
{	
	// ####### 10ms Periode #######
	time_delay_0++;
	
	#ifdef coarse
		switch(time_delay_0) {
			case 10: 
				PORTB |= (1 << DDB4); 
				break;
			case 20: 
				PORTB &= ~(1 << DDB4);
				PORTB |= (1 << DDB0); 			
				break;
			case 30: 
				PORTB &= ~(1 << DDB0);
				PORTB |= (1 << DDB2);			
				; break;
			case 40:
				PORTB &= ~(1 << DDB2);
				PORTD |= (1 << DDD7);
				break;
			case 50:
				PORTD &= ~(1 << DDD7);
				PORTB |= (1 << DDB3);
				break;
			case 60:
				PORTB &= ~(1 << DDB3);
				PORTD |= (1 << DDD5);
				break;
			case 70:
				PORTD &= ~(1 << DDD5);
				PORTD |= (1 << DDD4);
				break;
			case 80:
				PORTD &= ~(1 << DDD4);
			default: 
				break;
		}
	#elif defined fine
			switch(time_delay_0) {
				case 5:
				PORTB |= (1 << DDB4);
				break;
				case 10:
				PORTB &= ~(1 << DDB4);
				PORTB |= (1 << DDB0);
				break;
				case 15:
				PORTB &= ~(1 << DDB0);
				PORTB |= (1 << DDB2);
				; break;
				case 20:
				PORTB &= ~(1 << DDB2);
				PORTD |= (1 << DDD7);
				break;
				case 25:
				PORTD &= ~(1 << DDD7);
				PORTB |= (1 << DDB3);
				break;
				case 30:
				PORTB &= ~(1 << DDB3);
				PORTD |= (1 << DDD5);
				break;
				case 35:
				PORTD &= ~(1 << DDD5);
				PORTD |= (1 << DDD4);
				break;
				case 40:
				PORTD &= ~(1 << DDD4);
				default:
				break;
			}
	#elif defined super_fine
			switch(time_delay_0) {
				case 9:
				PORTB |= (1 << DDB4);
				break;
				case 10:
				PORTB &= ~(1 << DDB4);
				PORTB |= (1 << DDB0);
				break;
				case 11:
				PORTB &= ~(1 << DDB0);
				PORTB |= (1 << DDB2);
				; break;
				case 12:
				PORTB &= ~(1 << DDB2);
				PORTD |= (1 << DDD7);
				break;
				case 13:
				PORTD &= ~(1 << DDD7);
				PORTB |= (1 << DDB3);
				break;
				case 14:
				PORTB &= ~(1 << DDB3);
				PORTD |= (1 << DDD5);
				break;
				case 15:
				PORTD &= ~(1 << DDD5);
				PORTD |= (1 << DDD4);
				break;
				case 16:
				PORTD &= ~(1 << DDD4);
				default:
				break;
			}
	#else
		#error "no mode provided"	
	#endif
	
	// ####### 100ms Periode #######	
	if (time_delay_0 <= 10)
	{
		PORTD &= ~(1 << DDD6);
	}
	else
	{
		PORTD |= (1 << DDD6);		
		if (time_delay_0 == 100)
		{
			time_delay_0 = 0;
			shot_done = true;
		}
	}		
}          

ISR(INT0_vect)
{		
	while(PIND & (1 << DDD2))
	{
		_delay_ms(1);
	};		
		if (!button){					
			button = true;	
			single_shot();
		}
}

void single_shot()
{
		shot_done = false;
		TCNT1H = 0x0;
		TCNT1L = 0x0;		
		time_delay_0 = 0;
		TCCR1B |= (1<<CS11) | (1<<CS10);		
}