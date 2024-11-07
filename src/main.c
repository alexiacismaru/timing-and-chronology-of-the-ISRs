#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PORT PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define LED_DDR DDRB
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define NUMBER_OF_LEDS 4
#define INTERVAL 4000
#define INTERVAL_ISR 10000

void enableLed ( int lednumber ) 
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));    
}

void lightUpLed ( int lednumber ) 
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));  
}

void lightDownLed ( int lednumber )
{
  if ( lednumber < 0 || lednumber > 3 ) return;
  PORTB |= ( 1 << ( PB2 + lednumber ));   
}

ISR( PCINT1_vect )
{
  while ( bit_is_clear( BUTTON_PIN, BUTTON1) )
  {
    lightDownLed(3);
    lightUpLed(1);
    lightUpLed(2);
    _delay_ms( INTERVAL / 2 );
    lightDownLed(1);
    lightDownLed(2);
    _delay_ms( INTERVAL / 2 );
  }
}


int main(){
  enableLed(1);
  enableLed(2);
  enableLed(3);

  BUTTON_DDR &= ~_BV( BUTTON1 );
  BUTTON_PORT |= _BV( BUTTON1 );

  PCICR |= _BV( PCIE1 );
  PCMSK1 |= _BV( BUTTON1 );
  sei();

  while ( 1 )
    {
      lightUpLed(3);
      lightUpLed(2);
      _delay_ms( INTERVAL_ISR / 2 );
      lightDownLed(3);
      lightDownLed(2);
      _delay_ms( INTERVAL_ISR / 2 );
    }
  return 0;
}

/* 
QUESTIONS
-LED D1 remains on if you don't specify it turning off
-yes, they do
-yes
-yes
-yes
*/