#define _CRT_SECURE_NO_WARNINGS//dyrektywa do eliminowania ostrzezen o zaniechaniu - potrzebna jest dla funkcji localtime(&czas);
#include <iostream>
#include <time.h>
#include <windows.h>
#include <cmath>

using namespace std;

//struktura - drzewo BST, zawiera wskaznik na rodzica, lewego syna, prawego syna oraz do zmiennej przechowujacej date
struct BTnode
{
	BTnode* up;
	BTnode* left_son;
	BTnode* right_son;
	tm* value;
};

//funkcja dodajaca element do drzewa, jako argumenty otrzymuje drzewo do ktorego chcemy wstawic date, ktora rowniez jest podana(zmienna tm-wskaznik)
void Add(BTnode*& tree, tm* x)
{
	//towrzymy wskaznik do zmiennej pomocniczej typu BTnode i dodajemy do niego date przekazana jako argument, reszte elementow zerujemy
	BTnode* p=new BTnode;
	p->value = x;
	p->up = NULL;
	p->left_son = NULL;
	p->right_son = NULL;

	//kolejna zmienna pomocnicza, ktora ma takie same elementy jak drzewo przekazane jako argument
	BTnode* p1=tree;

	//kolejna zmienna pomocnicza, wszytskie skladniki sa rowne zero
	BTnode* p2=new BTnode;
	p2->value = NULL;
	p2->up = NULL;
	p2->left_son = NULL;
	p2->right_son = NULL;
	
	//przeksztalcamy wskaznik x(typ tm*) na zmienna typu time_t za pomoca funkcji mktime() by moc uzyc funkcji difftime(porownuje czas) 
	time_t y = mktime(x);

	//jesli drzewo do ktorego chcemy wstawic element jest puste to wstawiamy element na roota
	if (tree == nullptr)
	{
		tree = p;
	}
	else
	{
		//jesli drzewo nie jest puste to szukamy miejsca gdzie mamy wstawic nowy element
		//petla trwajaca dopoki p1 nie jest nullem(dopoki jest tam element)
		while (p1)
		{
			//p1 sluzy do wyszukiwania miejsca, a p2 bedzie mialo ostateczny wynik czyli element gdzie ma byc wstawiony nowy element
			p2 = p1;
			//przeksztalcamy wskaznik p2->value(typ tm*) na zmienna typu time_t za pomoca funkcji mktime() by moc uzyc funkcji difftime(porownuje czas) 
			time_t b = mktime(p2->value);

			if (difftime(b, y) > 0)//jesli znaleziony element jest wiekszy(pozniejsza data) to zmieniamy p1 na lewego sna p1
			{
				p1 = p1->left_son;
			}
			else//jesli znaleziony element jest mniejszy(wczesniejsza data) to zmieniamy p1 na prawego sna p1
				p1 = p1->right_son;
		}

		//jako rodzica naszego elementu ktory chcemy wstawic ustawiamy element znaleziony we wczesniejszej petli
		p->up = p2;
		//przeksztalcamy wskaznik p2->value(typ tm*) na zmienna typu time_t za pomoca funkcji mktime() by moc uzyc funkcji difftime(porownuje czas)
		time_t g = mktime(p2->value);
		//sprawdzamy czy roznica miedzy data ktora chcemy wstawic i data znaleziona(najblizsza data wstawianemu elementowi) jest jest wieksza od 3minut
		if(abs(difftime(g, y)) > 180)
		{
			//jesli tak to...
			if (difftime(g, y) > 0)
			{
				//sprawdzamy czy element, ktory chcemy wstawic ma dziadka
				if (p2->up)
				{
					//jesli ma to konwerujemy wskaznik na zmienna tm* na zmienna time_t
					time_t h = mktime(p2->up->value);
					//jesli rozncia czasu miedzy elementem wstawianym a jego dziadkiem jest wieksza od 3 minut to wstawiamy go do lewego syna swojego rodzica, a jesli nie to wyswietlamy komunikat o braku
					//mozliwosci wstawienia rezerwacji na ta godzine
					if (abs(difftime(h, y)) > 180)
					{
						p2->left_son = p;
					}
					else
					{
						cout << "Nie mozna zarezerwowac tej godziny!" << endl;
						Sleep(2500);//program sie zatrzymuje na 2,5s by moc zauwazyc powyzszy komunikat
					}
				}
				//gdy element wstawiany nie ma dziadka to dodajemy go do lewego syna swojego rodzica bez sprawdzania jakichkolwiek warunkow
				else
					p2 -> left_son = p;
			}
			//jesli nie to wstawiamy do prawego syna
			else
				p2->right_son = p;
		}
		//gdy roznica jest mniejsze od 3 minut to wyswietlamy komunikat ze nie mozna zarezerwowac tej godziny
		else
		{
			cout << "Nie mozna zarezerwowac tej godziny!" << endl;
			Sleep(2500);//program sie zatrzymuje na 2,5s by moc zauwazyc powyzszy komunikat
		}
	}
}

//funkcja porownujaca dwa czasy, arugmentami sa dwa wskazniki do zmiennych typu tm, zwraca prawde jesli pierwszy z argumentow jest wczesniejszy od drugeigo, falsz jesli
//pierwszy jest pozniejszy od drugiego
bool porwnanie_czasow(tm* z, tm* a)
{
	time_t x = mktime(z);
	time_t y = mktime(a);
	if (difftime(x, y) < 0)
	{
		return true;
	}
	else
		return false;
}

//funkcja wyswietlajaca elementy drzewa BST od najwczesniejszego do najpozniejszego - wykorzystuje przejscie inorder
void inorder(BTnode* x) 
{
	//jesli drzewno nei jest puste
	if (x != NULL)
	{
		//jesli element ma lewgo syna to korzystamy z rekurencji
		if (x->left_son)
			inorder(x->left_son);

		//wyswietlamy wartosci elementu(date), do roku dodajemy 1900, do miesiaca 1 poniewaz przy podawaniu daty to odjelismy(ze wzgledu na to ze liczy czas od 1900 oraz
		//mieisace sa liczone od 0)
		cout << x->value->tm_year+1900 << "." << x->value->tm_mon+1 << "." << x->value->tm_mday << " - " << x->value->tm_hour << ":" << x->value->tm_min << endl;

		//jesli element ma prawego syna to korzystamy z rekurencji
		if (x->right_son)
			inorder(x->right_son);
	}
	//jesli drzewo puste to wyswietlamy taki komunikat
	else
		cout << "Brak rezerwacji ladowan!" << endl;
}

//funkcja znajdujaca najmniejszy element w drzewie(dopoki istnieje lewy syn to przechodzimy do niego, a nastepnie zwracamy wskaznik do elementu)
BTnode* minimalna(BTnode* root) 
{
	while (root->left_son) //blad
		root = root->left_son;
	return root;
}

//funkcja znajudujaca nastepnika danego elementu
BTnode* next(BTnode* x)
{
	if (x->right_son) // je¿eli jest prawe poddrzewo
		return minimalna(x->right_son); // najmniejszy na prawo
	BTnode* y = x->up; // brak prawego poddrzewa, do zmiennej pomocniczej zapisuajemy rodzica x
	while ( y && x==y->right_son) //dopoki istnieje rodzic x oraz x jest prawym synem swojego rodzica
	{ // cofamy siê do góry
		x = y;
		y = y->up;
	}
	return y;
}

//funkcja usuwajaca elementy z drzewa x mniejsze od aktualnego czasu
void remove(BTnode*& x, tm* aktualny_czas)
{
	//tworzymy trzy zmienne pomocnicze
	BTnode* pom;
	BTnode* pom1;
	BTnode* pom2;

	//jesli drzewo nie jest puste
	if (x != NULL)
	{
		//zmienna pomocnicza pom jest najmniejszym elementem drzewa x
		pom = minimalna(x);
		//dopoki istnieje pom oraz ten czas(najmniejzy) jest mniejszy od czasu aktualnego
		while (pom && porwnanie_czasow(pom->value, aktualny_czas))
		{
			//druga zmienna pomocnicza zawiera nastepnika najmniejszego czasu
			pom1 = next(pom);
			//jesli najmniejszy element jest rootem
			if (x == pom)
			{
				//jesli pom ma prawego syna to ustawimy roota na prawego syna
				if (pom->right_son)
					x = pom->right_son;
				//jesli pom nie ma prawego syna to ustawiamy roota na zero
				else
					x = NULL;
			}
			//wyswieetlamy komunikat ktory element zostal usuniety(rowniez dodajemy 1900 do roku i 1 do miesiaca tak jak wczesniej)
			cout << "Usunieto rezerwacje: " << pom->value->tm_year+1900 << "." << pom->value->tm_mon+1 << "." << pom ->value->tm_mday << " - " << pom->value->tm_hour << ":" << pom->value->tm_min << endl;
			//ustawiamy kolejna zmienna pomocnicza na rodzica najmniejszego elementu
			pom2 = pom->up;
			//jesli nie mial rodzica
			if (pom2 != NULL)
				//jesli wartosc najmniejsza jest lewym synem swojego rodzica to ustawiamy lewego syna na zero
				if (pom2->left_son == pom)
					pom2->left_son = NULL;
				//jesli wartosc najmniejsza jest prawym synem swojego rodzica to ustawiamy prawego syna na zero
				else
					pom2->right_son = NULL;
			//usuwamy element
			delete pom;
			//teraz najmniejszym elementem bedzie nastepnik usunietego elementu
			pom = pom1;
		}
	}
	//jesli drzewo jest puste to wyswietlamy taki komunikat i wychodzimy z funkcji
	else
		cout << "Drzewo puste!" << endl;
}

int main()
{
	//drzewo ladawan, na poczatku jest nullem
	BTnode* ladowania = NULL;
	//zmienna do zachowania wyboru uzytkownika
	int wybor;
	//napis wstepny
	cout << "WITAJ W OBSLUDZE LOTOW NA NASZYM LOTNISKU!" <<  endl;
	//petla wykonujaca sie nieskonczenie wiele razy
	for (;;)
	{
		//glowne menu porgramu od ktorego zalezy co uzytkownik chce zrobic
		cout << "__________________________" << endl;
		cout << "|          MENU          |" << endl;
		cout << "|1.Dodaj rezerwacje.     |" << endl;
		cout << "|2.Wyswietl plan ladowan.|" << endl;
		cout << "|3.Wyjscie.              |" << endl;
		cout << "|________________________|" << endl;
		cin >> wybor;
		system("cls");

		//jesli wybral 1 to wstawiamy nowa rezerwacje
		if (wybor == 1)
		{
			//tworzymy wskaznik to zmiennej tm, ustawiamy te pola z ktorych nie bedziemy korzystac na zero
			tm* wsk = new tm;
			wsk->tm_sec = 0;
			wsk->tm_wday = 0;
			wsk->tm_yday = 0;
			wsk->tm_isdst = 0;

			cout << "Date prosze podawac w formacie: Rok - XXXX, Miesiac: X(1-12), Dzien: X(1-31), Godzina: X(0-23), Minuta: X(0-59)." << endl << endl;

			//pobieramy aktualny czas do zmiennej time_t
			time_t czas = time(0);
			//konwetujemy zmienna time_t na wskaznik do zmiennej tm* za pomoca funkcji localtime(&czas), a elementy z kotrych nie bedziemy korzystac ustawiamy na null
			tm* aktualny_czas = localtime(&czas);
			aktualny_czas->tm_sec = 0;
			aktualny_czas->tm_wday = 0;
			aktualny_czas->tm_yday = 0;
			aktualny_czas->tm_isdst = 0;

			//pobieramy dane od uzytkownika
			cout << "Podaj rok rezerwacji: ";
			cin >> wsk->tm_year;
			wsk->tm_year -= 1900;
			cout << "Podaj miesiac rezerwacji: ";
			cin >> wsk->tm_mon;
			wsk->tm_mon -= 1;
			cout << "Podaj dzien rezerwacji: ";
			cin >> wsk->tm_mday;
			cout << "Podaj godzine rezerwacji: ";
			cin >> wsk->tm_hour;
			cout << "Podaj minute rezerwacji: ";
			cin >> wsk->tm_min;
			//jesli podano date i czas, ktory bedzie w przyszlosci to mozna dodac elementu
			if (porwnanie_czasow(aktualny_czas, wsk))
			{
				//dodajemy element do naszego drzewa jesli spelnia warunki(nie ma ladaowan w ciagu 3 minut, warunki sa w funkcji Add)
				Add(ladowania, wsk);
			}
			//jesli podano date i czas, ktory byl w przeszlosci to nie mozna dodac elementu
			else
			{
				cout << "Nie mozna zarezerowac tego ladowania! Jest za pozno..." << endl;
				Sleep(2500);//program sie zatrzymuje na 2,5s by moc zauwazyc powyzszy komunikat
			}
			//czysimy terminal
			system("cls");
			//usuwamy ladowania, ktore juz sie odbyly
			remove(ladowania, aktualny_czas);

		}
		//jesli wybrano 2 to wyswietlamy rezerwacje za pomoca funkcji inorder
		else if (wybor == 2)
		{
			cout << "Plan ladawan lotniska: " << endl;
			inorder(ladowania);
			cout << endl;
		}
		//jesli wybrane jest cokolwiek innego to wychodzimy z porgramu
		else
			return 0;
	}
}