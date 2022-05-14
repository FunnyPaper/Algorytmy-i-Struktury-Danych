#include <iostream>
#include <ctime>
#include <cstdlib>

// UWAGA
// Przedrostek i- oznacza zmienna przychodzaca, przedrostek o- oznacza zmienna wychodzaca,
// przedrostek io- oznacza ze pracujemy na oryginalnej zmiennej (np z glownej petli programu),
// przedrostek m- oznacza zmienna nalezaca do klasy, brak przedrostka oznacza zmienna lokalna,
// krotsze definicje metod/ funkcji znajduja sie zaraz przy ich deklaracjach - reszta pod cialem funkcji main

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

// Algorytm szukania mediany
float Mediana(float* i_table, unsigned int i_size);

// Zegar do pomiaru czasu
Timer czas;

int main()
{
    std::srand(std::time(NULL));

    // Dane
    bool choice;
    int tableSize = 0, lowerBound = 0, upperBound = 0;
    float *table, mediana;

    // Wejscie 1 -> rozmiar tablicy, wybor opcji:
    std::cout << "Podaj rozmiar tablicy: "; std::cin >> tableSize;
    std::cout << "Czy liczby maja zostac wygenerowane losowo? (1 - tak, 0 - nie): "; std::cin >> choice;

    // Tworzymy tablice dynamiczna na elementy i przypisujemy do wskaznika
    table = new float[tableSize];

    if(choice)
    {
        // Wejscie 2 -> podanie granic:
        std::cout << "Z jakiego przedzialu [granice rowniez sie wliczaja]?\nDolna granica: ";
        std::cin >> lowerBound;
        std::cout << "Gorna granica: ";
        std::cin >> upperBound;
        // Wypelnianie tablicy
        for(unsigned int i = 0; i < tableSize; i++)
            table[i] = (std::rand() % (upperBound - lowerBound + 1)) + lowerBound;
    }
    else
    {
        std::cout << "Wypelnij tablice " << tableSize << " elementami [wprowadzonymi po spacji lub enterze]:\n";
        // Wypelnianie tablicy liczbami wprowadzonymi z klawiatury
        for(unsigned int i = 0; i < tableSize; i++)
            std::cin >> table[i];
    }

    // Rozpocznij pomiar czasu
    czas.Start();

    // Wyszukaj wartosc mediany w tablicy
    mediana = Mediana(table, tableSize);

    // Zakoncz pomiar czasu
    czas.Stop();

    // Wyjscie -> posortowana tablica oraz mediana:
    // Wypisz zawartosc posortowanej tablicy
    std::cout << std::string(20, '-') << "\nZawartosc tablicy po sortowaniu:\n";
    for(unsigned int i = 0; i < tableSize; i++)
        std::cout << table[i] << ' ';
    std::cout << '\n' << std::string(20, '-');

    // Wypisz mediane
    std::cout << "\nMediana: " << mediana << std::endl;

    // Wypisz czas szukania dominanty -> dopiero przy wiekszych tablicach widac czas rzedu milisekund
    czas.Display();

    // Usuwamy dynamiczna tablice
    delete[] table;
    return 0;
}

// Definicje klasy Timer:
void Timer::Display()
{
    clock_t diff = m_Stop - m_Start;
    // reprezentacja czasu w roznych jednostkach
    int miliseconds = ((double)diff / CLOCKS_PER_SEC) * 1000;
    int seconds = miliseconds / 1000;
    int minutes = seconds / 60;
    int hours = minutes / 60;

    // podzial czasu na odpowiednie jednostki
    minutes -= hours * 60; seconds -= minutes * 60; miliseconds -= seconds * 1000;

    std::cout << "Wyszukiwanie Mediany zajelo: "
    << hours << "h : " << minutes << "m : " << seconds << "s : " << miliseconds << "ms" << std::endl;
}

float Mediana(float* i_table, unsigned int i_size)
{
    // dane dodatkowe
    float temp;
    unsigned int nr = i_size;

    // sortuj babelkowo
    do
    {
        for(unsigned int i = 0; i < i_size-1; i++)
            if(i_table[i] > i_table[i+1])
            {
                temp = i_table[i];
                i_table[i] = i_table[i+1];
                i_table[i+1] = temp;
            }
    } while(--nr > 1);

    // Zwroc mediane
    return (i_size % 2) ? i_table[int(i_size/2)] : (i_table[int(i_size/2)] + i_table[int(i_size/2)-1]) / 2;
}
