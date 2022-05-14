#include <iostream>
#include <cstdlib>
#include <ctime>

// klasa zajmujaca sie pomiarem czasu
class Timer
{
public:

    // Konstruktor
    Timer() { Start(); }

    // Ustaw punkt poczatkowy (punkt koncowy rowna sie punktowi poczatkowemu w razie uzycia metod w zlej kolejnosci)
    void Start() { m_Start = std::clock(); m_Stop = m_Start; }

    // Ustaw punkt koncowy (nie ma potrzeby ustawiania punktu startowego -> ustawiamy go w konstruktorze na wszelki wypadek)
    void Stop() { m_Stop = std::clock(); }

    // Wyswietl w konsoli sformatowany czas
    void Display();

private:
    // Zmienne czlonkowskie
    clock_t m_Start{}, m_Stop{};
};

// Funkcja sortujaca + pomocnicze funkcje do wykorzystania jako parametr
void quickSort(float** tab, int first_item, int last_item, unsigned int criteria, unsigned int criteria_num, bool (*logic)(float, float));
bool asc(float a, float b) { return a > b; };
bool desc(float a, float b) { return a < b; };

// Glowne funkcje -> kazda reprezentuje inna strategie
void weightBackpack(float backpack_capacity, float** backpack, unsigned int item_count);
void valueBackpack(float backpack_capacity, float** backpack, unsigned int item_count);
void profitBackpack(float backpack_capacity, float** backpack, unsigned int item_count);

// Zegarek
Timer czas;

int main()
{
	// Dane (criteria_num okresla ilosc kryteriow opisujacych przedmiot: 0-index/ 1-wartosc/ 2-waga/ 3-ilosc/ 4-miejsce na dodatkowe dane):
    unsigned int item_count, criteria_num = 5;
    float backpack_capacity = 0;
    
    // Wejscie + wyjscie 1:
    std::cout << "Pojemnosc plecaka: "; std::cin >> backpack_capacity;
    std::cout << "Ilosc przedmiotow: "; std::cin >> item_count;

	// Stworzenie dynamicznej tablicy na przedmioty:
    float** item_description = new float*[item_count];
    for(unsigned int i = 0; i < item_count; i++)
    {
        item_description[i] = new float[criteria_num];
    }

	// Wejscie + wyjscie 2:
    std::cout << "Dla kazdego przedmiotu po kolei podaj cene, wage, ilosc szt. oddzielone spacjami\n";
    for(unsigned int i = 0; i < item_count; i++)
    {
    	// uzupelnianie indexu przedmiotu (numeracja od 1/ pozycja 0 w tablicy)
        item_description[i][0] = i+1;
        std::cout << "Przedmiot " << item_description[i][0] << ": ";
        // uzupelnianie ceny, wagi i ilosci poszczegolnego przedmiotu (pozycja 1,2 i 3 w tablicy)
        for(unsigned int j = 1; j < 4; j++)
        {
            std::cin >> item_description[i][j];
        }
    }

	// Strategia 1 + odmierzanie czasu
    czas.Start();
    valueBackpack(backpack_capacity, item_description, item_count);
    czas.Stop(); czas.Display();

	// Strategia 2 + odmierzanie czasu
    czas.Start();
    weightBackpack(backpack_capacity, item_description, item_count);
    czas.Stop(); czas.Display();

	// Strategia 3 + odmierzanie czasu
    czas.Start();
    profitBackpack(backpack_capacity, item_description, item_count);
    czas.Stop(); czas.Display();

	// Usun tablice dynamiczna
    for(unsigned int i = 0; i < item_count; i++)
        delete[] item_description[i];

    delete[] item_description;
    return 0;
}

// Definicje klasy Timer:
void Timer::Display()
{
    clock_t diff = m_Stop - m_Start;
    // reprezentacja czasu w roznych jednostkach
    long long miliseconds = ((long double)diff / CLOCKS_PER_SEC) * 1000;
    long long seconds = miliseconds / 1000;
    long long minutes = seconds / 60;
    long long hours = minutes / 60;
    // podzial czasu na odpowiednie jednostki
    miliseconds -= seconds * 1000; seconds -= minutes * 60; minutes -= hours * 60;
    std::cout << "Czas: " << hours << "h : " << minutes << "m : " << seconds << "s : " << miliseconds << "ms" << std::endl;
}
// Funkcja sortujaca -> sortowanie po osi Y -> zamiana pelnych wierszow miejscami wzgledem wyselekcjonowanej kolumny. *logic odpowiada za sposob porzadkowania (rosnaco/ malejaco/ customowo)
void quickSort(float** tab, int first_item, int last_item, unsigned int criteria, unsigned int criteria_num, bool (*logic)(float, float))
{
	// Warunek przerwania rekurencji
    if(first_item >= last_item) return;
	// Dane
    int i = first_item, j = last_item;
    float pivot = tab[(first_item + last_item)/2][criteria];
	// Glowna petla zmieniajaca
    while(true)
	{
		// Ustaw indexy na odpowiednie miejsca
		while(logic(pivot, tab[i][criteria])) i++;
		while(!logic(pivot, tab[j][criteria]) && pivot != tab[j][criteria]) j--;
		// Sprawdz czy mozliwa jest zamiana i to uczyn
		if(i <= j)
        {
            for(unsigned int k = 0; k < criteria_num; k++)
            {
                float temp = tab[i][k];
                tab[i][k] = tab[j][k];
                tab[j][k] = temp;
            }
            i++; j--;
        }
		if(i > j) break;
	}
	// Wywolywanie w sposob rekurencyjny
	if(j > first_item)
	quickSort(tab, first_item, j, criteria, criteria_num, logic);
	if(i < last_item)
	quickSort(tab, i, last_item, criteria, criteria_num, logic);
}
void weightBackpack(float backpack_capacity, float** backpack, unsigned int item_count)
{
	// Dane -> bc (waga przetrzymywanych przedmiotow), sum (wartosc przetrzymywanych przedmiotow)
    float bc = 0, sum = 0;
	// Posortuj wzgledem wagi
    quickSort(backpack, 0, item_count-1, 2, 5, asc);
	// Kalkulacje -> wartosc laczna + waga laczna + "wsadzanie" przedmiotow do plecaka (zwiekszanie ich ilosci w kolumnie dla dodatkowych informacji)
    for(unsigned int i = 0; i < item_count && bc <= backpack_capacity; i++)
    {
        backpack[i][4] = 0; // zeruj specjalna kolumne 
        for(unsigned int j = 0; j < backpack[i][3] && bc + backpack[i][2] <= backpack_capacity; j++)
        {
            bc += backpack[i][2]; backpack[i][4]++;
        }
        sum += backpack[i][4] * backpack[i][1];
    }
	// Posortuj wzgledem indexu przedmiotu
    quickSort(backpack, 0, item_count-1, 0, 5, asc);
	// Wyjscie - Strategia 2
    std::cout << '\n' << std::string(10, '+') << " Strategia 2 - waga " << std::string(10, '+') << '\n';
    std::cout << std::string(item_count * 4 + 1, '-') << "\n|";
    for(unsigned int i = 0; i < item_count; i++)
    {
        std::cout << ' ' << backpack[i][0] << " |";
    }
    std::cout << "\n|";
    for(unsigned int i = 0; i < item_count; i++)
    {
        std::cout << ' ' << backpack[i][4] << " |";
    }
    std::cout << "\nWartosc plecaka: " << sum;
    std::cout << "\nPoziom upakowania: " << bc << '\n';
}
void valueBackpack(float backpack_capacity, float** backpack, unsigned int item_count)
{
	// Dane -> bc (waga przetrzymywanych przedmiotow), sum (wartosc przetrzymywanych przedmiotow)
    float bc = 0, sum = 0;
	// Posortuj wzgledem wartosci
    quickSort(backpack, 0, item_count-1, 1, 5, desc);
	// Kalkulacje -> wartosc laczna + waga laczna + "wsadzanie" przedmiotow do plecaka (zwiekszanie ich ilosci w kolumnie dla dodatkowych informacji)
    for(unsigned int i = 0; i < item_count && bc <= backpack_capacity; i++)
    {
        backpack[i][4] = 0;
        for(unsigned int j = 0; j < backpack[i][3] && bc + backpack[i][2] <= backpack_capacity; j++)
        {
            bc += backpack[i][2]; backpack[i][4]++;
        }
        sum += backpack[i][4] * backpack[i][1];
    }
	// Posortuj wzgledem indexu przedmiotu
    quickSort(backpack, 0, item_count-1, 0, 5, asc);
	// Wyjscie - Strategia 1
    std::cout << '\n' << std::string(10, '+') << " Strategia 1 - wartosc " << std::string(10, '+') << '\n';
    std::cout << std::string(item_count * 4 + 1, '-') << "\n|";
    for(unsigned int i = 0; i < item_count; i++)
    {
        std::cout << ' ' << backpack[i][0] << " |";
    }
    std::cout << "\n|";
    for(unsigned int i = 0; i < item_count; i++)
    {
        std::cout << ' ' << backpack[i][4] << " |";
    }
    std::cout << "\nWartosc plecaka: " << sum;
    std::cout << "\nPoziom upakowania: " << bc << '\n';
}
void profitBackpack(float backpack_capacity, float** backpack, unsigned int item_count)
{
	// Dane -> bc (waga przetrzymywanych przedmiotow), sum (wartosc przetrzymywanych przedmiotow)
    float bc = 0, sum = 0;
	// Kalkulacje -> policz oplacalnosc kazdego przedmiotu i umiesc ta wartosc w kolumnie dodatkowych informacji (ostatnia w tablicy)
    for(unsigned int i = 0; i < item_count; i++)
    {
        backpack[i][4] = backpack[i][1] / backpack[i][2];
    }
    // Posortuj wzgledem oplacalnosci
    quickSort(backpack, 0, item_count-1, 4, 5, desc);
	// Kalkulacje -> wartosc laczna + waga laczna + "wsadzanie" przedmiotow do plecaka (zwiekszanie ich ilosci w kolumnie dla dodatkowych informacji)
    for(unsigned int i = 0; i < item_count && bc <= backpack_capacity; i++)
    {
        backpack[i][4] = 0;
        for(unsigned int j = 0; j < backpack[i][3] && bc + backpack[i][2] <= backpack_capacity; j++)
        {
            bc += backpack[i][2]; backpack[i][4]++;
        }
        sum += backpack[i][4] * backpack[i][1];
    }
	// Posortuj wzgledem indexu przedmiotu
    quickSort(backpack, 0, item_count-1, 0, 5, asc);
	// Wyjscie - Strategia 3
    std::cout << '\n' << std::string(10, '+') << " Strategia 3 - oplacalnosc " << std::string(10, '+') << '\n';
    std::cout << std::string(item_count * 4 + 1, '-') << "\n|";
    for(unsigned int i = 0; i < item_count; i++)
    {
        std::cout << ' ' << backpack[i][0] << " |";
    }
    std::cout << "\n|";
    for(unsigned int i = 0; i < item_count; i++)
    {
        std::cout << ' ' << backpack[i][4] << " |";
    }
    std::cout << "\nWartosc plecaka: " << sum;
    std::cout << "\nPoziom upakowania: " << bc << '\n';
}
