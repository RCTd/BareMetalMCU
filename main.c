#include "usart.h"
#include <wdt.h>
#include <interrupt.h>

void delay(volatile long time)
{
	while(time>0)
	{
		time--;
	}
}
void usart(void)
{
	USART_initialize(19200);
	unsigned char string[]="Hello world!", aux;
	unsigned int i=0;
	while(1)
	{
		aux=USART_Receive();
		i=0;
		if(aux=='s'||aux=='S')
		{
			while(string[i]!='\0')
			{
				USART_transmit(string[i]);
				i++;
			}
		}
	}
}

void blink(void)
{
	DDRB|=(1<<PB5);
	while(1)
	{
		PORTB^=(1<<PB5);
		delay(100000);
	}
}
void timer(unsigned int freqHz)
{
	DDRB|=(1<<PB1);
	TCCR1A|=(4<<WGM10)|(1<<COM1A0);
	long pre=F_OSC/65536/freqHz;
	TCCR1B=0;
	TCCR1B|=(1<<CS10);
	if(pre>256)
	{
		TCCR1B+=4;
		pre=1024;
	}else
	if(pre>64)
	{
		TCCR1B+=3;
		pre=256;
	}else
	if(pre>8)
	{
		TCCR1B+=2;
		pre=64;
	}else
	if(pre>0)
	{
		TCCR1B++;
		pre=8;
		OCR1A=F_OSC/freqHz/pre;
	}else
	OCR1A=F_OSC/freqHz;
	//while(1);
}

void timerP(unsigned int periodms)
{
	//1...4194ms
	DDRB|=(1<<PB1);
	TCCR1A|=(4<<WGM10)|(1<<COM1A0);
	long pre=(F_OSC/100)*periodms/65536;
	TCCR1B=0;
	TCCR1B|=(1<<CS10);
	if(pre>2560)
	{
		TCCR1B+=4;
		pre=1024;
	}else
	if(pre>640)
	{
		TCCR1B+=3;
		pre=256;
	}else
	if(pre>80)
	{
		TCCR1B+=2;
		pre=64;
	}else
	if(pre>0)
	{
		TCCR1B++;
		pre=8;
		OCR1A=(F_OSC/1000)*periodms/pre;
	}else
		OCR1A=(F_OSC/1000)*periodms;
	//while(1);
}

void usartPeriodrecive(void)
{
	USART_initialize(19200);
	unsigned char aux;
	unsigned int p=0,i;
	while(1)
	{
		p=0;
		do{
			aux=USART_Receive();
			USART_transmit(aux);
			if(aux!='!')
				p=p*10+(aux-'0');
		}while(aux!='!');
		i=p;
		USART_transmit('S');
		while(i)
		{
			aux=i%10;
			i/=10;
			USART_transmit(aux+'0');
		}
		timerP(p);
	}
}

//__no_init unsigned int state;
int state __attribute__ ((section (".noinit")));
//up=wdp0=0
//down=wdp0=1
void WDtimer20Hz33(void)
{
	//__watchdog_reset();
	//asm("wdr");
	//WDTCR |= (1<<WDCE)|(1<<WDE);
	//(state&1)
	//WDTCR = (1<<WDP2) |(1<<WDE);

	//.....................
	// cli();
	// wdt_reset();
	// WDTCSR |= (1 << WDCE) | (1 << WDE);
	// WDTCSR =(1<<WDE) | (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0);
	// sei();
	wdt_enable(WDTO_1S);
	PORTC=(!(state&1)<<PC0);
	DDRC|=1<<(PC0);
	state=state^1;
	
	//USART_initialize(19200);
	//USART_transmit(state+'0');
	
	while(1);
}
//o perioada de ceas este 0.0625us(16MHz)
int main(void)
{
	WDtimer20Hz33();
}