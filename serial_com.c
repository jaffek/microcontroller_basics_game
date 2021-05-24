#include <ADUC834.H>
#include <stdio.h> 
#include <stdlib.h>

int time_to_signal;										// element losowy (kompilator sugerowal umieszczenie w przestrzeni globalnej)
// konfiguracja uart
void setup_UART(void)
{
	T3CON = 0x82;				
	T3FD = 0x12;
	SCON = 0x52;	
}
// wysylanie znaku do uart
void send_char(char c)
{
	while(!TI);
	TI=0;
	SBUF = c;
}
// wysylanie ciagu znakow do uart
void send_string(char *str)
{
   unsigned char c; 
   while((c = *str++)) 
     send_char(c); 
}
// odbieranie znaku z uart
unsigned char receive_char()
{
	unsigned char c;
	while(!RI);
	RI=0;
	c = SBUF;
	return c;
}
// konfiguracja licznik (przepelnienie co 0,001s) 
void setup_counter()
{
	TMOD = 0x10;				
	TL1 = 0x7D;						
	TH1 = 0xFF;
	TCON = 0x40;	
}
// zamiana liczb na ciagi znakow do pozniejszego wyswietlenia
void tostring(char str[], unsigned short int num, unsigned char precision)
{
    unsigned short int i, rem;	
		if(precision == 1)
		{
			for (i = 0; i < 4; i++)
			{
					rem = num % 10;
					num = num / 10;
				if(i == 3)
				{
					str[1] = ','; 
					str[0] = rem + '0';
				}
				else
				{
					str[5 - (i + 1)] = rem + '0';
				}
			}
			str[5] = '\0';
		}
		else
		{
			for (i = 0; i < 2; i++)
			{
					rem = num % 10;
					num = num / 10;
				  str[2 - (i + 1)] = rem + '0';
			}
			str[2] = '\0';
		}
}

// wypisywanie czasu
void text(unsigned short time)
{
	char result[6];
	tostring(result, time,1);
	send_string(result);
	send_string(" s");
}
// funkcja inkrementuje zmienna przechowujaca zliczany czas po przepelnieniu
unsigned short int overflow(unsigned short int timer)
{
	if(TF1 == 1)												
			{
				TF1 = 0;													
				TL1 = 0x7D;												
				TH1 = 0xFF;
				timer = timer + 1;							
			}
			return timer;
}
// funkcja wstrzymujaca program na okreslony czas
void czekaj_ms(unsigned short int t)
{
	unsigned short int timer = 0;
	while(timer < t)
		timer = overflow(timer);
}

// zliczanie czasu
unsigned short int count(unsigned char  random, short int r_time)
{
	
	unsigned short int timer = 0;
	TF1 = 0;													
	TL1 = 0x7D;												
	TH1 = 0xFF;
	if(random == 1)															// liczenie czasu do momentu wyswietlenia sygnalu 
	{
		srand(r_time);														// inicjowanie funkcji generujacej liczby pseudolosowe liczba ms ktore uplynely od wyswietlenia menu do wybrana trybu
		time_to_signal = rand()%2000+1000;				// liczby losowe w zakresie 1000 - 3000 ms 
		while(timer < time_to_signal)														
			timer = overflow(timer);
  }
	else																				// liczenie czasu reakcji
		while(!(P2 & (1<<7)))														
			timer = overflow(timer);
	return timer;
}