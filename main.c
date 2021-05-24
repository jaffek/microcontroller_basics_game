#include "serial_com.h"
#include <ADUC834.H>

// Autorzy: Julianna Lachowicz, Jaroslaw Affek

unsigned char menu;   								// zmienna zapamietujaca wybor trybu gry
unsigned short int timer = 0;					// zmienna pomocnicza do przechowywania wartosci czasu
unsigned short int game_time = 0;			// zmienna przechowujaca czas gry z wielokrotnym wyborem (3s)
short int last_t = -1;								// ostatni czas
short int best_t = -1;								// najlepszy czas
short int last_s = -1;								// ostatnia liczba skutecznych prób w wielokrotnej próbie
short int best_s = -1;								// max liczba skutecznych prób w serii gier
short int shots = 0;									// zmienna pomocnicza do przechowywania liczby udanych prób w grze
unsigned short int random_timer = 0;	// przechowuje wartosc czasu od momentu wyswietlenia menu programu do wybrania trybu (element losowy) 
unsigned short int measure_time=0;		// przechowje czas 0-6 s do wcisniecia przycisku po wyborze trybu
char result[3];

// funkcja wyswietlajaca menu gry wraz z wynikami (jesli jakies sa zapisane)
void menu_write(short int last_time,short int best_time,short int last_shots,short int best_shots)
{
	measure_time = 0;
	send_string("\n\n\nWybierz tryb rozgrywki\n");
	send_string("Rozgrywka jednokrotna: j\nRozgrywka wielokrotna: w\n\n");
	if(last_time != -1)
	{
		send_string("Wyniki rozgrywki jednokrotnej:\nOstatni czas: ");
		text(last_time);
		send_string("	Najlepszy czas: ");
		text(best_time);
		send_string("\n");
	}
	if(last_shots != -1)
	{
		send_string("Wyniki rozgrywki wielokrotnej:\nOstatnio trafione: ");
		tostring(result, last_shots,0);
		send_string(result);
		send_string("	Najlwiecej trafionych: ");
		tostring(result, best_shots,0);
		send_string(result);
		send_string("\n\n");
	}
}
//*************************************************************************************************************
void main (void)
{
  setup_UART();																													// konfiguracja transmisji UART
	send_string("Autorzy: Julianna Lachowicz, Jaroslaw Affek");
	setup_counter();																											// konfiguracja licznik
	start:
	menu_write(last_t,best_t,last_s,best_s);
	send_string("\n");
	while(!RI)
		random_timer = overflow(random_timer);															// pomiar czasu: wyswietlenie menu > wybor trybu (element losowy)
	menu = receive_char();																								// wczytanie znaku
	send_string("Wcisnij przycisk\n");
	measure_time=0;
	shots=0;
	setup_counter();	
	while(1)
	{
		game_time = 0;
		switch(menu)										
		{
			case 'j': 								
				 if(!(P2 & (1<<7)))
					 {
						  send_string("Czekaj na sygnal\n");
							timer = count(1,random_timer);														// losowy czas od wcisniecia przycisku do wyswietlenia sygnalu
							send_string("Puszczaj guzik\n");													// sygnal od ktorego liczony jest czas reakcji
							timer = count(0,random_timer);														// czas reakcji
							if(timer == 0)
							{																													// jesli timer = 0 to znaczy, ze przycisk puszczony przed sygnalem
								send_string("Falstart!\n");
								czekaj_ms(3000);
								goto start;
							}
							else
							{
								last_t = timer;
								if(last_t < best_t || best_t == -1)											// przypisywanie danych do zmiennych globalnych
									best_t = last_t;
								send_string("OK\n");
								czekaj_ms(3000);
								goto start;
							}
					 }
					 else
					 {
						 measure_time = overflow(measure_time);											// 6 sekund na wcisniecie przycisku, potem powrot do menu
						 if(measure_time > 6000)
							 goto start;
					 }
					break;
			case 'w':	
				if(!(P2 & (1<<7)))
				{
				  while(game_time < 3000)
						{
							 if(!(P2 & (1<<7)))
							 {
								  send_string("Czekaj na sygnal\n");
									timer = count(1,random_timer);											// losowy czas od wcisniecia przycisku do wyswietlenia sygnalu
									send_string("Puszczaj guzik!\n");										// sygnal od ktorego liczony jest czas reakcji
									timer = count(0,random_timer);											// czas reakcji
									game_time = game_time + timer;											// sumowanie czasu gry (do 3s)
									if(timer == 0)
									{																										// jesli timer = 0 to znaczy, ze przycisk puszczony przed sygnalem
										send_string("Falstart!\n");
										czekaj_ms(3000);
										goto start;
									}
									else
										if(game_time <= 3000)
										{
											shots++;
											last_s = shots;
											if(last_s > best_s || best_s == -1)						// przypisywanie danych do zmiennych globalnych
												best_s = last_s;	
											send_string("Reakcja poprawna\nPozostaly czas: ");
											text(3000-game_time);
											send_string("\n");
											czekaj_ms(3000);
										}
							 }
						 }
						send_string("Czas uplynal!\n");
						czekaj_ms(3000);
						goto start;
					 }
					else
					{
						measure_time = overflow(measure_time);									// 6 sekund na wcisniecie przycisku, potem powrot do menu
						if(measure_time > 6000)
							 goto start;
					}
				break;
		}
	}
}