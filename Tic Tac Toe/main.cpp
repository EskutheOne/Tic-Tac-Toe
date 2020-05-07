#include<stdlib.h>
#include<stdio.h>
#include"winbgi2.h"
#include<time.h>
#include<math.h>


int GRA[10] = { 0,0,0,0,0,0,0,0,0,0 };  //tablica w ktorej zapisywane sa ruchy "2" dla gracza "1" dla komputera
int BOT[10];						//tablica do ktorej kopiowana jest tablica gra, na potrzeby funkcji obliczajacej najlepszy ruch dla PC
int ruch = 0;					//ilosc wykonanych ruchow, maksymalnie 9
int start;				//zmienna losowa od 0 do 1, determinujaca kto zaczyna gre w trybie trudnym


int gracz(int typ);			//prototypy funkcji, dzieki nim funkcja napisana w kodzie wyzej moze wywolywac funkcje napisana nizej (jeszcze nie zaadowan¹ przez kompilator)
int trudny(int typ);
int znajdzruch();


int max(int a, int b)
{
	if (a > b) return a;
	else return b;
}

int min(int a, int b)
{
	if (a < b) return a;
	else return b;
}

void wyczyscbufor()			//funkcja czysczaca bufor, potrzebna do obslugi bledow np gdy ktos wpisze znak zamiast liczby
{
	int n;
	while ((n = getchar()) != EOF && n != '\n');
}

void plansza()			// funkcja rysujaca plansze do gry
{
	graphics(300, 300);
	line(0, 0, 300, 0);
	line(0, 0, 0, 300);
	line(0, 300, 300, 300);
	line(300, 0, 300, 300);

	line(100, 0, 100, 300);
	line(200, 0, 200, 300);
	line(0, 100, 300, 100);
	line(0, 200, 300, 200);
}

void iks(int x)			//funkcja przyjumujaca zmienn¹ od 1 do 9 i rysujaca w tym polu na planszy X
{
	if (x < 4)
	{
		line(10 + 100 * ((x + 2) % 3), 10, 90 + 100 * ((x + 2) % 3), 90);
		line(10 + 100 * ((x + 2) % 3), 90, 90 + 100 * ((x + 2) % 3), 10);
	}
	else if (x > 3 && x < 7)
	{
		line(10 + 100 * ((x + 2) % 3), 110, 90 + 100 * ((x + 2) % 3), 190);
		line(10 + 100 * ((x + 2) % 3), 190, 90 + 100 * ((x + 2) % 3), 110);
	}
	else
	{
		line(10 + 100 * ((x + 2) % 3), 210, 90 + 100 * ((x + 2) % 3), 290);
		line(10 + 100 * ((x + 2) % 3), 290, 90 + 100 * ((x + 2) % 3), 210);
	}
}

void kolo(int k)		//funkcja przymujaca zmienna od 1 do 9 i rysujaca w tym polu na planszy O
{
	if (k < 4)
	{
		circle(50 + 100 * ((k + 2) % 3), 50, 40);
	}
	else if (k > 3 && k < 7)
	{
		circle(50 + 100 * ((k + 2) % 3), 150, 40);
	}
	else
	{
		circle(50 + 100 * ((k + 2) % 3), 250, 40);
	}
}

int wygrana(int* GRA1)		//funckja sprawdzajaca czy przeslana tablica spelnia warunki wygranej dla gracza
{
	for (int i = 0; i < 3; i++)
	{
		if ((GRA1[1 + 3 * i] == 2 && GRA1[2 + 3 * i] == 2 && GRA1[3 + 3 * i] == 2) || (GRA1[1 + i] == 2 && GRA1[4 + i] == 2 && GRA1[7 + i] == 2) || (GRA1[1] == 2 && GRA1[5] == 2 && GRA1[9] == 2) || (GRA1[3] == 2 && GRA1[5] == 2 && GRA1[7] == 2))
		{	//printf("WYGRALES! \n"); 
			return 1;
		}
	}
	return 0;
}

int przegrana(int* GRA1)			//funckja sprawdzajaca czy przeslana tablica spelnia warunki przegranej dla gracza
{
	for (int i = 0; i < 3; i++)
	{
		if ((GRA1[1 + 3 * i] == 1 && GRA1[2 + 3 * i] == 1 && GRA1[3 + 3 * i] == 1) || (GRA1[1 + i] == 1 && GRA1[4 + i] == 1 && GRA1[7 + i] == 1) || (GRA1[1] == 1 && GRA1[5] == 1 && GRA1[9] == 1) || (GRA1[3] == 1 && GRA1[5] == 1 && GRA1[7] == 1))
		{	//printf("PRZEGRALES \n");
			return 1;
		}
	}
	return 0;
}

int remis()			//funckja sprawdzajaca czy spelnione sa warunki remisu
{
	if (ruch == 9)
	{	//printf("REMIS \n"); 
		return 1;
	}
	return 0;
}

int gracz(int typ)		//funkcja odpowiadajaca za ruch gracza i sprawdzanie jego poprawnosci
{
	int pole = 0;
	if (wygrana(GRA) || przegrana(GRA) || remis()) return 1;
	do
	{
		printf("WYBIERZ POLE DO ZAGRANIA \n");
		printf("[1][2][3]\n[4][5][6]\n[7][8][9]\n");
		scanf("%i", &pole);
		wyczyscbufor();
	} while ((GRA[pole] == 1 || GRA[pole] == 2) && ruch <= 9 || (pole > 9 || pole < 1));		//sprawdza czy podane pole jest poprawne
	GRA[pole] = 2;

	if (typ == 1)
	{
		iks(pole);
	}
	else if (typ == 2) kolo(pole);
	ruch++;
	if (wygrana(GRA) || przegrana(GRA) || remis()) return 1;
	return 0;
}


int latwy(int typ)			//funkcja odpowiadajaca za losowy ruch komputera i wywolujaca funckje "gracz" do czasu spelnienia warunkow konca gry
{
	srand(time(NULL));
	int x;
	while (ruch <= 9 && (wygrana(GRA) == 0 || przegrana(GRA) == 0))
	{
		if (typ == 2)
		{
			do
			{
				x = rand() % 9 + 1;
			} while (GRA[x] == 1 || GRA[x] == 2);
			iks(x);
			GRA[x] = 1;
			ruch++;
		}
		else if (typ == 1)
		{
			do
			{
				x = rand() % 9 + 1;
			} while (GRA[x] == 1 || GRA[x] == 2);
			kolo(x);
			GRA[x] = 1;
			ruch++;
		}
		if (przegrana(GRA) || wygrana(GRA) || remis()) return 0;
		if (gracz(typ) == 1) return 0;
	}

	return 0;
}

int trudny(int typ)			//funckja odpowiadajaca za algorytm komputera w trybie trudnym
{
	if (start == 1)				//zaczyna komputer
	{
		if (typ == 1) kolo(5);
		else iks(5);
		GRA[5] = 1;					//zawsze zaczyna stawiajac w srodku bo to najlepsze pole do wygrania
		ruch++;
		gracz(typ);
	}
	else if (start == 0)				//zaczyna gracz
	{
		gracz(typ);
		if (GRA[5] == 2)				//sprawdza czy gracz zaczal w srodku, jezeli tak to losuje jeden z naroznikow i tam zagrywa
		{
			int L;
			L = rand() % 4 + 1;
			if (typ == 1 && L == 1) { kolo(1); GRA[1] = 1; }
			if (typ == 1 && L == 2) { kolo(3); GRA[3] = 1; }
			if (typ == 1 && L == 3) { kolo(7); GRA[7] = 1; }
			if (typ == 1 && L == 4) { kolo(9); GRA[9] = 1; }
			if (typ == 2 && L == 1) { iks(1); GRA[1] = 1; }
			if (typ == 2 && L == 2) { iks(3); GRA[3] = 1; }
			if (typ == 2 && L == 3) { iks(7); GRA[7] = 1; }
			if (typ == 2 && L == 4) { iks(9); GRA[9] = 1; }

		}
		else if (typ == 1) { kolo(5); GRA[5] = 1; }
		else { iks(5); GRA[5] = 1; }
		ruch++;
	}


	if (start == 1)				//gra od trzeciego ruchu, gdy zaczynal komputer
	{
		do
		{
			for (int i = 0; i < 10; i++)				//przepisywanie tablicy do symulowanej tablicy "backupu" o nazwie BOT
			{
				BOT[i] = GRA[i];
			}
			int m = znajdzruch();			//komputer szuka najlepszego ruchu
			GRA[m] = 1;
			if (typ == 1) kolo(m);
			else iks(m);
			ruch++;
			if (wygrana(GRA) || przegrana(GRA) || remis()) break;		//spradzanie czy gra sie skonczyla
			gracz(typ);
		} while (przegrana(GRA) == 0 && ruch <= 9 && wygrana(GRA) == 0);
	}

	if (start == 0)					//gra od trzeciego ruchu, gdy zaczynal gracz
	{
		do
		{
			gracz(typ);
			if (przegrana(GRA) || wygrana(GRA) || remis()) break;			//spradzanie czy gra sie skonczyla
			for (int j = 0; j < 10; j++)						//przepisywanie tablicy do symulowanej tablicy "backupu" o nazwie BOT
			{
				BOT[j] = GRA[j];
			}
			int n = znajdzruch();				//komputer szuka najlepszego ruchu
			GRA[n] = 1;
			if (typ == 1) kolo(n);
			else iks(n);
			ruch++;
		} while (przegrana(GRA) == 0 && ruch <= 9 && wygrana(GRA) == 0);

	}

	return 0;
}

int minimax(bool isMax)				//funkcja liczaca wartosc danego ruchu i symulujaca gre (wszystkie mozliwosci) w tablicy BOT az do konca gry
{
	int wynik;

	if (wygrana(BOT)) { wynik = -10; return wynik; }				//najpierw sprawdzamy czy gra sie nie skonczyla
	if (przegrana(BOT)) { wynik = 10; return wynik; }
	if (remis()) { return 0; }

	if (isMax)		//gdy podklada ruchy szukajac najbardziej oplacalnego dla gracza
	{
		int best = -100;
		for (int q = 1; q < 10; q++)
		{
			if (BOT[q] == 0)
			{
				BOT[q] = 1;						//podkladanie ruchu by sprawdzic czy jest dobry
				ruch++;
				best = max(best, minimax(!isMax));				//rekurecyjne wywolywanie funkcji zeby sprawdzic wszystkie mozliwosci
				BOT[q] = 0;						//cofanie ruchu
				ruch--;
			}
		}
		return best;
	}

	else			//gdy podklada ruchy szukajac najbardziej oplacalnego dla komputera
	{
		int best = 100;
		for (int w = 1; w < 10; w++)
		{
			if (BOT[w] == 0)
			{
				BOT[w] = 2;				//podkladanie ruchu
				ruch++;
				best = min(best, minimax(!isMax));				//rekurecyjne wywolywanie funkcji zeby sprawdzic wszystkie mozliwosci
				BOT[w] = 0;				//cofanie ruchu
				ruch--;
			}
		}
		return best;
	}
}

int znajdzruch()			//funkcja szukajaca najlepszego nastepnego ruchu dla komputera (uzywa funkcji minimax)
{
	int najruch = -100;			//poczatkowa wartosc najlepszego ruchu
	int najpozycja = 0;			//poczatkowa najlepsza pozycja do ruchu
	int wartruch;
	for (int i = 1; i < 10; i++)
	{
		if (BOT[i] == 0)
		{
			BOT[i] = 1;			//symulacja ruchu
			ruch++;

			wartruch = minimax(false); //zmienna wartosci danego ruchu
			BOT[i] = 0;				//cofanie ruchu
			ruch--;

			if (wartruch > najruch)
			{
				najpozycja = i;		//przypisywanie pola jako najlepszej pozycji do ruchu
				najruch = wartruch;
			}
		}
	}
	return najpozycja;			//zwracanie najlepszej pozycji do zagrania
}

void main()
{
	int poziom = 0, rodzaj = 0;

	while (poziom != 1 && poziom != 2)		//wybieranie poziomu trudnosci komputera
	{
		printf("Wybierz poziom trudnosci: \n 1. - LATWY \n 2. - TRUDNY \n");
		scanf("%i", &poziom);
		wyczyscbufor();			//w przypadku gdy uzytkownik wpisalby znak zamiast liczby
		if (poziom != 1 && poziom != 2) printf("WYBIERZ PONOWNIE \n");
	}

	while (rodzaj != 1 && rodzaj != 2)			//wybieranie jak¹ figur¹ chcemy graæ
	{
		printf("Czym chcesz grac? \n X - iksami, wcisnij 1 \n O - kolkami, wcisnij 2 \n");
		scanf("%i", &rodzaj);
		wyczyscbufor();					//w przypadku gdy uzytkownik wpisalby znak zamiast liczby
		if (rodzaj != 1 && rodzaj != 2) printf("WYBIERZ PONOWNIE \n");
	}

	plansza();
	if (poziom == 1)
	{
		printf("ZACZYNA KOMPUTER \n");
		latwy(rodzaj);		//w trybie latwym zawsze zaczyna komputer, to zwieksza mu szanse na wygrana
	}
	else
	{
		srand(time(NULL));
		start = rand() % 2;					//losowanie kto zaczyna w trybie trudnym
		if (start == 0) printf("TY ZACZYNASZ \n");
		else printf("ZACZYNA KOMPUTER \n");
		trudny(rodzaj);

	}
	if (wygrana(GRA)) printf("WYGRALES \n");
	if (przegrana(GRA)) printf("PRZEGRALES \n");
	if ((przegrana(GRA) == 0 && wygrana(GRA) == 0) && remis()) printf("REMIS \n");

	system("PAUSE");
}
