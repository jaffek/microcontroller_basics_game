// modul komunikacji szeregowej dla mikrokontrolera ADuC834

#ifndef serial_com_h
#define serial_com_h

// konfiguracja ukladu UART
void setup_UART(void);

// wyslanie znaku
//   c - znak do wyslania
void send_char(char c);

// wyslanie ciagu znaków (zakonczonego 0x00) 
//   *str - wskaznik na pierwszy znak z ciagu
void send_string(char *str);

// odbieranie znaku
unsigned char receive_char();

// konfiguracja licznika T1
void setup_counter();

// zamiana int na string
// str - string do wyswietlenia jako wartosc na stoperz, num - wartosc int, ktora ma byc zamieniona na string
void tostring(char str[], unsigned short int num,unsigned char precision);

// wypisywanie czasu
// time - zmierzony czas do wyswietlenia w oknie terminala
void text(unsigned short time);

// funkcja inkrementuje zmienna przechowujaca zliczany czas po przepelnieniu
unsigned short int overflow(unsigned short int timer);

void czekaj_ms(unsigned short int t);

// zliczanie czasu
// timer - mierzony czas, previous - poprzednio zmierzony czas
// funkcja zwraca aktualna wartosc czasu na stoperze
unsigned short int count(unsigned char  random, short int r_time);

#endif