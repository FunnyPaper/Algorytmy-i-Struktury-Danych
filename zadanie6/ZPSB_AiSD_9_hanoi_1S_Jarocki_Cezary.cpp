#include <iostream>
#include <ctime>
#include <cstdlib>

// struktura calej listy
class Stos
{
	public:
	// struktura pojedynczego elementu stosu
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
	Node* m_TopElement;
	// metody operujace stosem (wlacznie z przeladowaniami)
	void push(const std::string& i_key, int i_pendant);
	void push(int i_pendant);
	void push(const std::string& i_key);
	void push(Node* i_element);
	Node* pop();
	bool empty();
	bool full();
	unsigned int size();
	Node* top();
	void display();
	// konstruktor i destruktor
	Stos() : m_TopElement(NULL) {}
	~Stos()
	{
		if(m_TopElement)
		delete m_TopElement;
	}
};

// funkcje generujace losowe klucze i breloki
std::string RandomKey(); // zwraca napis o dlugosci 5 znakow
int RandomInt();

// Sortowanie za pomoca algorytmu Hanoi -> Stos* i_stacks wystepuje tylko w celu poprawnego wypisywania stosow (podpisywania ktory stos jest ktory)
void HanoiSort(unsigned int i_elements_number, unsigned int& io_ilosc_krokow,
               Stos& i_left_column, Stos& i_right_column, Stos& i_middle_column, Stos* i_stacks);

int main(int argc, char** argv) {

    // ustawienie std::rand() przed jakimkolwiek wywolaniem tej funkcji -> std::time(0) oznacza aktualny czas
    // (w praktyce najlepiej umiescic takie wezwanie raz na poczatku programu)
	std::srand(std::time(0));

	// 1. Inicjalizujemy 3 puste stosy
	Stos stosy[3];
	unsigned int kroki = 0;

	// 2. Dodajemy do pierwszego stosu 10 elementow o losowych kluczach (znaki alfanumeryczne)  i brelokach o wartosciach w kolejnosci malejacej 9-0.
	for(int i = 9; i >= 0; i--)
	{
		stosy[0].push(i);
	}

	// *Przerywnik w konsoli -> lepsza czytelnosc
	std::cout << std::string(10, '-') << "ZAWARTOSC STOSOW-PRZED HANOI: \n\n";

	// 3. Wyswietlamy stosy wraz z iloscia elementow na kazdym z nich
	std::cout << "Stos 1:"; stosy[0].display();
	std::cout << "Stos 2:"; stosy[1].display();
	std::cout << "Stos 3:"; stosy[2].display();
	std::cout << "\nKroki: " << kroki << '\n';

	// 4. Przenosimy dane z 1 stosu na 3 (wieze Hanoi) za kazdym razem wyswietlajac zawartosc stosow wraz z iloscia elementow wraz z iloscia wykonanych krokow.
	HanoiSort(stosy[0].size(), kroki, stosy[0], stosy[2], stosy[1], stosy);

	// *Przerywnik w konsoli -> lepsza czytelnosc
    std::cout << std::string(10, '-') << "ZAWARTOSC STOSOW-PO HANOI: \n\n";

    // 5. Wyswietlamy stosy po przeniesieniu danych wraz z iloscia elementow wraz z iloscia wykonanych krokow.
	std::cout << "Stos 1:"; stosy[0].display();
	std::cout << "Stos 2:"; stosy[1].display();
	std::cout << "Stos 3:"; stosy[2].display();
	std::cout << "\nKroki: " << kroki << '\n';

	return 0;
}
Stos::Node* Stos::pop()
{
    if(m_TopElement)
    {
        Stos::Node* element = m_TopElement;
        m_TopElement = m_TopElement->m_Next;
        element->m_Next = NULL;
        return element;
    }
    return NULL;
}
void Stos::push(const std::string& i_key, int i_pendant)
{
    Stos::Node *element = new Stos::Node();
    element->m_Key = i_key;
    element->m_Pendant = i_pendant;
    element->m_Next = m_TopElement;
    m_TopElement = element;
}
void Stos::push(int i_pendant)
{
    Stos::Node *element = new Stos::Node();
    element->m_Key = RandomKey();
    element->m_Pendant = i_pendant;
    element->m_Next = m_TopElement;
    m_TopElement = element;
}
void Stos::push(const std::string& i_key)
{
    Stos::Node *element = new Stos::Node();
    element->m_Key = i_key;
    element->m_Pendant = RandomInt();
    element->m_Next = m_TopElement;
    m_TopElement = element;
}
void Stos::push(Node* i_element)
{
    i_element->m_Next = m_TopElement;
    m_TopElement = i_element;
}
bool Stos::empty()
{
    return m_TopElement == NULL;
}
bool Stos::full()
{
    try
    {
        Stos::Node* element = new Stos::Node();
    }
    catch(const std::bad_alloc& e)
    {
        return true;
    }
    return false;
}
unsigned int Stos::size()
{
    unsigned int i = 0;
    Stos::Node* element = m_TopElement;
    while(element)
    {
        element = element->m_Next; i++;
    }
    return i;
}
Stos::Node* Stos::top()
{
    return m_TopElement;
}
void Stos::display()
{
    Stos::Node* stackend = NULL;
    Stos::Node* element = m_TopElement;
    std::cout << size() << ':' << (size()? "" : "EMPTY");
    while(stackend != m_TopElement)
    {
        element = m_TopElement;
        while(element->m_Next != stackend) element = element->m_Next;
        stackend = element;
        std::cout << ' ' << element->m_Pendant;
    }
    std::cout << '\n';
}
std::string RandomKey()
{
	std::string o_key = "";
	for(unsigned int i = 0; i < 10; i++)
	{
		int	num;
		do
		{
			num = (std::rand() % 75) + 48;
		} while((num > 'Z' && num < 'a') || (num > '9' && num < 'A'));
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
void HanoiSort(unsigned int i_elements_number, unsigned int& io_ilosc_krokow, Stos& i_left_column, Stos& i_right_column, Stos& i_middle_column, Stos* i_stacks)
{
    if(i_elements_number == 0) return;
    HanoiSort(i_elements_number-1, io_ilosc_krokow, i_left_column, i_middle_column, i_right_column, i_stacks);
    // zamiana
    i_right_column.push(i_left_column.pop());
    io_ilosc_krokow++;
	std::cout << std::string(10, '-') << "ZAWARTOSC STOSOW-HANOI: \n\n";

	std::cout << "Stos 1:"; i_stacks[0].display();
	std::cout << "Stos 2:"; i_stacks[1].display();
	std::cout << "Stos 3:"; i_stacks[2].display();
	std::cout << "\nKroki: " << io_ilosc_krokow << '\n';

    HanoiSort(i_elements_number-1, io_ilosc_krokow, i_middle_column, i_right_column, i_left_column, i_stacks);
}
