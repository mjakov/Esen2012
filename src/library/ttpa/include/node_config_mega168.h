#ifndef NODE_CONFIG_ATMEGA168
#define NODE_CONFIG_ATMEGA168
/* Copyright (c) 2004, 2005, Christian Trödhandl
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
 
   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.
 
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */
 

/* include/node_config_mega168.h
 *      Node configuration file for board Mega168
 *
 * $Id: node_config_mega168.h,v 1.3 2007-01-18 19:18:47 bernd Exp $
 *
 * Revision History:
 *   2004/11/16 : S. Krywult <stefan@krywults.at>
 *     - adapted for ATmega8
 *   2005/08/24 (0.4): C. Trödhandl <chtr@vmars.tuwien.ac.at>
 *   2005/12/05 (0.5): C. Trödhandl <chtr@vmars.tuwien.ac.at>
 *     - adapted for ATmega168
 *
 */

// SW configurable options
#define SWCONF_IOPORT PORTB
#define SWCONF_IODDR DDRB
#define SWCONF_SWUART PB2
#define SWCONF_AVCC PB3

#undef node_swconf_swuart
#define node_swconf_swuart() node_swconf_highen(SWCONF_SWUART) 
#undef node_swconf_hwuart
#define node_swconf_hwuart() node_swconf_lowen(SWCONF_SWUART) 
#undef node_swconf_avcc_a
#define node_swconf_avcc_a() node_swconf_lowen(SWCONF_AVCC) 
#undef node_swconf_avcc_d
#define node_swconf_avcc_d() node_swconf_highen(SWCONF_AVCC) 

#if !defined (CLOCKRATE)
#  define CLOCKRATE 7372800
#endif /* CLOCKRATE */

#if !defined (MAXSPDUP)
#  define MAXSPDUP 3
#endif /* MAXSPDUP */

#if defined (SW_UART)
#  if !defined (SW_UART_RXPORT)
#    define SW_UART_RXIOPORT PORTB
#    define SW_UART_RXIODDR DDRB
#    define SW_UART_RXPORT PB0
#    define SW_UART_RXIOPIN PINB
#  endif /* SW_UART_RXPORT */
#  if !defined (BUS_SIG_IC)
#    define BUS_SIG_IC SIG_INPUT_CAPTURE1
#  endif /* BUS_SIG_IC */
#  if !defined (SW_UART_TXUSEOC)
#    define SW_UART_SENDCORR -79
#    if !defined (SW_UART_TXUSEPORT)
#      define SW_UART_TXUSEPORT
#    endif /* SW_UART_TXUSEPORT */
#  endif /* SW_UART_TXUSEOC */
#  define SW_UART_RECVCORR -77
#  if defined (SW_UART_TXUSEPORT)
#    if !defined (SW_UART_TXPORT)
#      define SW_UART_TXIOPORT PORTB
#      define SW_UART_TXIODDR DDRB
#      define SW_UART_TXPORT PB1
#    endif /* SW_UART_TXPORT */
#  endif /* SW_UART_TXUSEPORT */
#  define bus_transcvr_init() bus_transcvr_isok_init()
#  define bus_transcvr_send() bus_transcvr_isok_send()
#  define bus_transcvr_recv() bus_transcvr_isok_recv()
#endif /* UART_SW */

#if !defined (TTPA_SIG_OC)
#  define TTPA_SIG_OC SIG_OUTPUT_COMPARE1A
#  define TTPA_COM0 COM1A0
#  define TTPA_COM1 COM1A1
#  define TTPA_OCR OCR1A
#  define TTPA_OCIE OCIE1A
#  define TTPA_FOC FOC1A
#  if defined (SW_UART_TXUSEOC)
#    if !defined (SW_UART_TXOCPORT)
#      define SW_UART_TXOCIOPORT PORTB
#      define SW_UART_TXOCIODDR DDRB
#      define SW_UART_TXOCPORT PB1
#    endif /* SW_UART_TXOCPORT */
#  endif /* SW_UART_TXUSEOC */
#endif /* TTPA_SIG_OC */

#if !defined (TTPA_TCNT)
#  define TTPA_TCNT TCNT1
#  define TTPA_TCCRA TCCR1A
#  define TTPA_TCCRB TCCR1B
#  define TTPA_TCCRC TCCR1C
#  define TTPA_CS0 CS10
#  define TTPA_ICES ICES1
#  define TTPA_ICR ICR1
#  define TTPA_TIMSK TIMSK1
#  define TTPA_TIFR TIFR1
#  define TTPA_TICIE ICIE1
#  define TTPA_TOIE TOIE1
#  define TTPA_SIG_TOV SIG_OVERFLOW1
#endif /* TTPA_TCNT */

#define LS_SINGLE
#define LS_PORT0 PORTB
#define LS_DDR0 DDRB
#define LS_LED0 PB4

#if !defined(EEWE)
#define EEWE EEPE
#endif /* EEWE */


#endif /* NODE_CONFIG_ATMEGA168 */
