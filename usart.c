#include "usart.h"
void USART_initialize(unsigned short int baud_rate)
{
	/* setează baud rate */
	baud_rate=(F_OSC/16/baud_rate - 1);
	UBRR0H = (unsigned char)(baud_rate >> 8);
	UBRR0L = (unsigned char)(baud_rate);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); /* activează transmisia și recepția la ieșire */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	/* setează pinul TXD: ieșire */
	DDRD |= (1 << PD1);
	/* setează pinul RXD: intrare */
	DDRD &= ~(1 << PD0);
	/* activează întreruperea */
	//UCSR0B |= (1 << RXCIE);
}
void USART_transmit(unsigned char data)
{
	/* așteaptă până ce se termină de transmis toate datele și după trece la următoarele informații */
	while (!( UCSR0A & (1 << UDRE0)))
	{
		;
	}
	UDR0 = data;
}
unsigned char USART_Receive( void )
{
	/* Așteaptă recepționarea datelor */
	while ( !(UCSR0A & (1<<RXC0)) )
	{
		;
	}
	/* Preia și returnează datele recepționate din buffer */
	return UDR0;
}
