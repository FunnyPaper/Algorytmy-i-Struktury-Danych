#include <iostream>
#include <ctime>
#include <cstdlib>

// struktura calej listy
class Lista
{
	public:
	// struktura pojedynczego elementu listy
	struct Node
	{
		public:
		Node* m_Next;
		std::string m_Key;
		int m_Pendant;
		~Node()
		{
			if(m_Next)
			delete m_Next;
		}
	};
	// dostep do poczatku listy;
	Node* m_Head;
	// metody operujace lista
	Node* Find(const std::string& i_key);
	void Delete(const std::string& i_key);
	void Insert(const std::string& i_key);
	// konstruktor i destruktor
	Lista() : m_Head(NULL) {}
	~Lista()
	{
		if(m_Head)
		delete m_Head;
	}
};

// funkcje generujace losowe klucze i breloki
std::string RandomKey(); // zwraca napis o dlugosci 5 znakow
int RandomInt();


int main(int argc, char** argv) {

    // ustawienie std::rand() przed jakimkolwiek wywolaniem tej funkcji -> std::time(0) oznacza aktualny czas
    // (w praktyce najlepiej umiescic takie wezwanie raz na poczatku programu)
	std::srand(std::time(0));

	// *Znane klucze -> zautomatyzowanie niektorych czesci zadania
	std::string ZnaneKlucze[] = {"Cezary", "AiSD", "Jarocki"};

	// 1. Inicjalizujemy pusta liste (przypisujemy null pierwszemu lementowi listy w konstruktorze)
	Lista lista;

	// 2. Dodajemy element o ustalonym kluczu (insert) - imiê autora (dane autentyczne)
	lista.Insert(ZnaneKlucze[0]);

	// 3. Dodajemy 5 elementów o kluczach losowych o d³ugoœci 5 znaków (insert)
	for(unsigned int i = 0; i < 5; i++)
	{
		lista.Insert(RandomKey());
	}

	// 4. Dodajemy element o ustalonym kluczu (insert) - "AiSD"
	lista.Insert(ZnaneKlucze[1]);

	// 5. Dodajemy 5 elementów o kluczach losowych o d³ugoœci 5 znaków (insert)
	for(unsigned int i = 0; i < 5; i++)
	{
		lista.Insert(RandomKey());
	}

	// 6. Dodajemy element o ustalonym kluczu (insert) - nazwisko autora (dane autentyczne)
	lista.Insert(ZnaneKlucze[2]);

	// *Przerywnik w konsoli -> lepsza czytelnosc
	std::cout << std::endl << std::string(10, '-') << "ZAWARTOSC LISTY: \n\n";
	
	// 7. wyœwietlamy listê w postaci Klucz: "xxxxx" Brelok: "xxxxx"
	for(Lista::Node* it = lista.m_Head; it; it = it->m_Next)
	{
		std::cout << "Klucz: \"" << it->m_Key << "\" Brelok: \"" << it->m_Pendant << "\"\n";
	}

	// *Przerywnik w konsoli -> lepsza czytelnosc
	std::cout << std::endl << std::string(10, '-') << "ZNANE KLUCZE: \n\n";

	// 8. Wyszukujemy elementy o kluczach ustalonych (find) i wyœwietlamy je
	for(unsigned int i = 0; i < 3; i++)
	{
		Lista::Node* it = lista.Find(ZnaneKlucze[i]);
		std::cout << "Klucz: \"" << it->m_Key << "\" Brelok: \"" << it->m_Pendant << "\"\n";
	}

	// 9. Usuwamy elementy z kluczami ustalonymi
	for(unsigned int i = 0; i < 3; i++)
	{
		lista.Delete(ZnaneKlucze[i]);
	}
	
	// *Przerywnik w konsoli -> lepsza czytelnosc
	std::cout << std::endl << std::string(10, '-') << "ZAWARTOSC LISTY: \n\n";

	// 10. wyœwietlamy listê w postaci Klucz: "xxxxx" Brelok: "xxxxx"
	for(Lista::Node* it = lista.m_Head; it; it = it->m_Next)
	{
		std::cout << "Klucz: \"" << it->m_Key << "\" Brelok: \"" << it->m_Pendant << "\"\n";
	}

	return 0;
}
Lista::Node* Lista::Find(const std::string& i_key)
{
	// uzyskaj dostep do pierwszego elementu listy
	Lista::Node* o_current = m_Head;

	// dostan sie do interesujacej nas pozycji za pomoca klucza
	// (w przypadku kiedy takiego klucza nie ma ustaw wskaznik current na null -> pierwszy warunek petli)
	while(o_current && o_current->m_Key != i_key)
		o_current = o_current->m_Next;

	// zwroc szukany element listy
	return o_current;
}
void Lista::Delete(const std::string& i_key)
{
    // ustaw wskaznik na element do usuniecia
	Lista::Node* current = Find(i_key);

	// sprawdz czy szukany klucz jest pierwszym elementem
	if(current == m_Head)
	{
		// usuwanie pierwszego elementu listy pod warunkiem ze nie jest on nullem
		if(current)
		{
			// przestaw glowe na druga pozycje listy
			m_Head = current->m_Next;

			// odczep stara glowe od ciala listy (druga pozycja staje sie pierwsza)
			current->m_Next = NULL;

			// usun niegdys pierwsza pozycje listy
			delete current;
		}
	}
	else if(current)
	{
		// sprawdz czy szukany klucz jest ostatnim elementem (wskaznik m_Next w takim wypadku zwraca wartosc null)
		if(!(current->m_Next))
		{
			// usuwanie ostatniego elementu listy

			// uzyskaj dostep do pierwszego elementu listy
			Lista::Node* previous = m_Head;

			// dostan sie na przedostatnia pozycje listy
			while(previous->m_Next->m_Next)
				previous = previous->m_Next;

			// usun ostatnia pozycje listy
			delete current;

			// ustaw dla wskaznika przedostatniego elementu listy wartosc null
			// (po usunieciu ostatniego elementu listy wskaznik m_Next poprzedniego elementu wskazuje na usuniety element dlatego trzeba przypisac mu nowa wartosc)
			previous->m_Next = NULL;
		}
		else
		{
			// usuwanie elementu znajdujacego sie pomiedzy poczatkiem a koncem listy

			// uzyskaj dostep do pierwszego elementu
			Lista::Node* previous = m_Head;

			// dostan sie do elementu poprzedzajacego szukany klucz
			while(previous->m_Next != current)
				previous = previous->m_Next;

			// ustaw wskaznik elementu poprzedzajacego tak by wskazywal na ten sam element co wskaznik elementu usuwanego
			previous->m_Next = current->m_Next;

			// odczep usuwany element od ciala listy
			current->m_Next = NULL;

			// usun pozycje listy
			delete current;
		}
	}
}
void Lista::Insert(const std::string& i_key)
{
	// dodawanie elementu na koniec listy (pod warunkiem ze taki klucz nie znajduje sie juz w liscie)
	if(!Find(i_key))
	{
		Lista::Node *key, *previous;

		// stworz nowy element listy
		key = new Lista::Node();
		key->m_Pendant = RandomInt(); // generator liczb
		key->m_Key = i_key;
		key->m_Next = NULL;

		// uzyskaj dostep do pierwszego elementu listy
		previous = m_Head;

		// sprawdz czy previous jest nullem
		if(previous)
		{
			// jesli previous nie jest nullem to dostan sie na ostatnia pozycje listy
			while(previous->m_Next)
				previous = previous->m_Next;

            // ustaw wskaznik ostatniego elementu na nowy klucz (dodawany element staje sie czescia listy)
			previous->m_Next = key;
		}
		else m_Head = key;
	}
}
std::string RandomKey()
{
	std::string o_key = "";
	for(unsigned int i = 0; i < 5; i++)
	{
		int	num;
		do
		{
		    // w ascii litery A-z zajmuja 57 pozycji (z drobnym wyjatkiem miedzy Z-a)
		    // nastepnie dodajemy 65 poniewaz litery zaczynaja sie dopiero od 65 pozycji
			num = (std::rand() % 58) + 65;
		} while(num > 'Z' && num < 'a'); // numerycznie w ascii pomiedzy 90 (Z) a 97 (a) znajduja sie znaki inne od liter

		// zmien typ z int na char i dodaj do napisu
		o_key += static_cast<char>(num);
	}

	// zwroc wygenerowany klucz
	return o_key;
}
int RandomInt()
{
    // wygeneruj i zwroc liczbe z przedzialu 0-100
	return std::rand() % 101;
}
