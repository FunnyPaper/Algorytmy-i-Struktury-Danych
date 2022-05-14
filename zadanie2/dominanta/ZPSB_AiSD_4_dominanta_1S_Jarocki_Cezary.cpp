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

// Algorytm szukania dominanty w tablicy dynamicznej (rozbity na cztery mniejsze bardziej uniwersalne funkcje i jedna glowna)
// Rozbity na sortowanie, szukanie wartosci maksymalnej oraz wlasciwe szukanie dominanty

// Funkcja testujaca czy pierwsza wartosc jest wieksza
bool Greater(double i_first, double i_second) { return i_first < i_second; }

// Funkcja tetujaca czy pierwsza wartosc jest mniejsza
bool Lesser(double i_first, double i_second) { return i_first > i_second; }

// Funkcja imitujaca sortowanie babelkowe
void BubbleSort(float* i_tab, unsigned int i_size, bool (*i_sortingOrder)(double, double));

// Funkcja szukajaca maksymalnej wartosci w tablicy
unsigned int MaxVal(unsigned int* i_tabToSearch, unsigned int i_tableSize);

// Glowna funkcja szukajaca dominanty ->
void Dominanta(float* i_tab, unsigned int i_size, float*& io_newTab, unsigned int& io_newTabSize, unsigned int& io_count);

// Zegar do pomiaru czasu
Timer czas;

int main()
{
    std::srand(std::time(0));

    // Dodatkowe dane
    unsigned int tableSize = 0, lowerBound = 0, upperBound = 0;
    bool choice;

    float* dominantTable, *inputTable;
    unsigned int dominanta = 0, dominantTableSize;

    // Wejscie 1-> rozmiar tablicy i wybor opcji:
    std::cout << "Podaj rozmiar tablicy: "; std::cin >> tableSize;
    std::cout << "Czy liczby maja zostac wygenerowane losowo? (1 - tak, 0 - nie): "; std::cin >> choice;

    // Tworzymy tablice dynamiczna na elementy i przypisujemy do wskaznika
    inputTable = new float[tableSize];

    if(choice)
    {
        // Wejscie 2 -> podanie granic:
        std::cout << "Z jakiego przedzialu [granice rowniez sie wliczaja]?\nDolna granica: "; std::cin >> lowerBound;
        std::cout << "Gorna granica: "; std::cin >> upperBound;
        // Wypelnianie tablicy
        for(unsigned int i = 0; i < tableSize; i++)
            inputTable[i] = std::rand() % (upperBound - lowerBound + 1) + lowerBound;
    }
    else
    {
        std::cout << "Wypelnij tablice " << tableSize << " elementami [wprowadzonymi po spacji lub enterze]:\n";
        // Wypelnianie tablicy liczbami wprowadzonymi z klawiatury
        for(unsigned int i = 0; i < tableSize; i++)
            std::cin >> inputTable[i];
    }

    // Zacznij mierzyc czas
    czas.Start();

    // Wyszukaj dominanty za pomoca algorytmu
    Dominanta(inputTable, tableSize, dominantTable, dominantTableSize, dominanta);

    // Przerwij pomiar czasu
    czas.Stop();

    // Wyjscie -> posortowana tablica, dominanty oraz ich ilosc ich wystapien:
    // Wypisz tablice po sortowaniu
    std::cout << std::string(20, '-') << "\nZawartosc tablicy po sortowaniu:\n";
    for(unsigned int i = 0; i < tableSize; i++)
        std::cout << inputTable[i] << ' ';
    std::cout << '\n' << std::string(20, '-');

    // Wypisz rezultat szukania dominanty
    if(dominanta == 1 || dominantTableSize == tableSize || tableSize == dominantTableSize * dominanta)
    {
        std::cout << "\nBrak dominant!\n";
    }
    else if(dominantTableSize > 1)
    {
        std::cout << "\nDominanta: ";
        for(unsigned int i = 0; i < dominantTableSize; i++)
           std::cout << dominantTable[i] << ", ";

        std::cout << "wystepuje po " << dominanta << " razy.\n";
    }
    else
    {
        // W tym wypadku wystepuje tylko jedna dominanta a rozmiar tablicy dominant to 1 zatem jedyny element ma indeks 0
        std::cout << "\nDominanta: " << dominantTable[0];
        std::cout << ", wystepuje " << dominanta << " razy.\n";
    }

    // Wypisz czas szukania dominanty -> dopiero przy wiekszych tablicach widac czas rzedu milisekund
    czas.Display();

    // Usuwamy zawartosc dynamiczna na ktora wskazuje wskaznik
    delete[] dominantTable;

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

    std::cout << "Wyszukiwanie dominanty zajelo: " << hours << "h : " << minutes << "m : " << seconds << "s : " << miliseconds << "ms" << std::endl;
}
void BubbleSort(float* i_tab, unsigned int i_size, bool (*i_sortingOrder)(double, double))
{
    // Sortowanie babelkowe -> w zaleznosci od podanenej funkcji wytyczajacej kolejnosc
    bool isSorted = true;
    float temp;

    do {
        isSorted = true;
        for(unsigned int j = 0; j < i_size-1; j++)
        {
            if(i_sortingOrder(i_tab[j+1], i_tab[j]))
            {
                temp = i_tab[j];
                i_tab[j] = i_tab[j+1];
                i_tab[j+1] = temp;
                isSorted = false;
            }
        }
    } while(!isSorted);
}
unsigned int MaxVal(unsigned int* i_tabToSearch, unsigned int i_tableSize)
{
    unsigned int o_maxVal = 1;

    // Przeszukaj podana tablice w poszukiwaniu najwiekszej wartosci
    for(unsigned int i = 0; i < i_tableSize; i++)
        if(Greater(o_maxVal, i_tabToSearch[i]))
            o_maxVal = i_tabToSearch[i];

    // zwroc wartosc najwieksza w tablicy
    return o_maxVal;
}
// Definicja glownej funkcji odpowiedzialnej za algorytm szukania dominanty
void Dominanta(float* i_tab, unsigned int i_size, float*& io_newTab, unsigned int& io_newTabSize, unsigned int& io_count)
{
    // Dodatkowe dane:
    unsigned int *frequencyTable = new unsigned int[i_size];
    unsigned int frequency = 1; // Wartosc liczaca ilosc powtorzen w tablicy (1 poniewaz przy przeliczaniu istnieje conajmniej 1 powtorzenie)

    // Sortujemy dane babelkowo
    BubbleSort(i_tab, i_size, Lesser);

    // Uzupelniamy tablice powtorzen odpowiednimi wartosciami (rzeczona wartosc umieszczana jest na pozycji ktora odpowiada ostatniej liczbie przed zmiana)
    for(unsigned int i = 0; i < i_size-1; i++, frequency++)
    {
        if(i_tab[i+1] != i_tab[i])
        {
            frequencyTable[i] = frequency; frequency = 0;
        }
        else frequencyTable[i] = 0;
    }

    // Zapisujemy liczbe powtorzen dla istatniej liczby w tablicy
    // Nie robimy tego w petli powyzej poniewaz musielibysmy sprawdzac warunek dla ostatniej pozycji co iteracje co jest zbedne
    frequencyTable[i_size-1] = frequency;

    // Szukamy najwiekszej wartosci w tablicy potorzen i zapisujemy to do zmiennej wyjsciowej
    io_count = MaxVal(frequencyTable, i_size);

    // Ustawiamy wartosc poczatkowa dla rozmiaru tablic z dominantami
    io_newTabSize = 0;
    for(unsigned int i = 0; i < i_size; i++)
        if(frequencyTable[i] == io_count) io_newTabSize++;

    // Tworzymy nowa tablice dynamiczna na dominanty i przypisujemy do zewnetrznego wskaznika
    io_newTab = new float[io_newTabSize];

    // Dodajemy wartosci do tablicy dominant
    for(unsigned int i = 0, j = 0; i < i_size && j < io_newTabSize; i++)
    {
        if(frequencyTable[i] == io_count)
            io_newTab[j++] = i_tab[i];
    }

    // Usuwamy tablice czestotliwosci
    delete[] frequencyTable;
}
