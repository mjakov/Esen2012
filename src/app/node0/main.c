/** 
 * blinking_leds (N1-3)
 *
 **/


#include "global.h"
#include "main.h"
#include "node0.h"
#include "protocoll.h"
#include "util/delay.h"

void rcv_handler(uint8_t msg_length, uint8_t *msg_body);
void wr_to_bargraph(uint8 val);


int main(void) {
  uint8 i;

  DDRF |= (1 << PF3);
  PORTF &= ~(1 << PF3);
  DDRF |= (1 << PF2);
  PORTF &= ~(1 << PF2);  
  
  protocol_init(255,rcv_handler);

  i = 0;
  while(TRUE) {    
    send_msg(1, &i);
    i++;
    _delay_ms(100);    
  }

  while(TRUE);
  return 1; 
}

int main2(void)
{
	/* setting up hw-uart0 */
	HWUART0_OPORT |= ( _BV(HWUART0_RXD) | _BV(HWUART0_TXD) );
	HWUART0_DDR |= _BV(HWUART0_TXD);
	SWUART_OPORT |= ( _BV(SWUART_RXD) | _BV(SWUART_TXD) );
	
	LED_DDR |= (1<<LED_GREEN) | (1<<LED_RED);
	LED_PORT_REG |= (1<<LED_RED);
	
	while(TRUE)
	{
		if(HWUART0_IPORT & (1<<HWUART0_RXD))
		{
			LED_PORT_REG |= (1<<LED_RED);
			LED_PORT_REG &= ~(1<<LED_GREEN);
		}
		else
		{
			LED_PORT_REG |= (1<<LED_GREEN);
			LED_PORT_REG &= ~(1<<LED_RED);
		}		
	}


	while(TRUE); /* the day will never come when we remove this redundancy */
	return 1;
}

void rcv_handler(uint8_t msg_length, uint8_t *msg_body) {
  /*
  if (msg_length > 0) {
    wr_to_bargraph(msg_body[0]);
  } else {
    //debug
  }
  */
} 

void wr_to_bargraph(uint8 val) {
  LEDS_DDR_ND0 |= val; 
  LEDS_PORT_ND0 &= ~(val);
}
