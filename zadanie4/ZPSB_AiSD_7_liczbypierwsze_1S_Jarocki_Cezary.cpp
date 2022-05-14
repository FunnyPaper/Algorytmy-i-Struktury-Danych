#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

void divisibility_test(unsigned int granica);
void eratosthenes_sieve(unsigned int granica);
void linear_sieve(unsigned int granica);

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

// Zegar do pomiaru czasu
Timer czas;

int main()
{
    unsigned int tabSize;

    std::cout << "Podaj gorna granice poszukiwan liczb pierwszych: "; std::cin >> tabSize;

    std::cout << "\n1. Sprawdzenie podzielnosci\n";
    czas.Start();
    divisibility_test(tabSize);
    czas.Stop();
    std::cout << '\n';
    czas.Display();
    std::cout << std::string(40,'-') << '\n';

    std::cout << "2. Sito Eratostenesa\n";
    czas.Start();
    eratosthenes_sieve(tabSize);
    czas.Stop();
    std::cout << '\n';
    czas.Display();
    std::cout << std::string(40,'-') << '\n';

    std::cout << "3. Sito Liniowe\n";
    czas.Start();
    linear_sieve(tabSize);
    czas.Stop();
    std::cout << '\n';
    czas.Display();
    std::cout << std::string(40,'-') << '\n';

    return 0;
}
void divisibility_test(unsigned int granica)
{
    bool isprime; // Z pomoca tej zmiennej mozemy na biezaco wypisywac liczby pierwsze
    for(unsigned int i = 2; i <= granica; i++)
    {
        isprime = true;
        for(unsigned int j = 2; j < i; j++)
        {
            if(i % j == 0)
            {
                isprime = false; break;
            }
        }
        if(isprime) std::cout << i << ' ';
    }
    std::cout << '\n';
}
void eratosthenes_sieve(unsigned int granica)
{
    // tworzymy ograniczenie by przeszukiwac tylko do pierwiastka z n komorek wlacznie
    unsigned int ograniczenie = static_cast<unsigned int>(sqrt(granica));

    // stworz dynamiczna tablice na liczby pierwsze
    // by przyspieszyc dzialanie algorytmu opieramy sie na zasadzie false-> liczba pierwsza, true->liczba zlozona
    // dzieki temu nie musimy tworzyc petli ustawiajacej wartosci tablicy na true, zaoszczedzajac troche na czasie
    bool *isNotPrim = new bool[granica+1]{}; // domyslna wartosc {} to 0, a w tym wypadku False dla calej tablicy
    for(unsigned int i = 2; i <= ograniczenie; i++)
    {
        if(!isNotPrim[i])
        {
            for(unsigned int j = i * i; j <= granica; j+= i)
            {
                isNotPrim[j] = true;
            }
        }
    }
    // odczytaj te komorki tablicy ktore maja przypisana wartosc logiczna 0
    for(unsigned int i = 2; i < granica+1; i++)
    {
        if(!isNotPrim[i]) std::cout << i << ' ';
    }
    std::cout << '\n';

    // usun tablice dynamiczna
    delete[] isNotPrim;
}
void linear_sieve(unsigned int granica)
{
    // stworz dynamiczna tablice na liczby pierwsze
    // by przyspieszyc dzialanie algorytmu opieramy sie na zasadzie false-> liczba pierwsza, true->liczba zlozona
    // dzieki temu nie musimy tworzyc petli ustawiajacej wartosci tablicy na true, zaoszczedzajac troche na czasie
    bool* sieve = new bool[granica+1]{}; // domyslna wartosc {} to 0, a w tym wypadku False dla calej tablicy

    // m_1, m_2 -> mnozniki
    // liczba -> liczba zlozona
    for(unsigned int m_1 = 2, m_2 = m_1; m_1 * m_1 <= granica; m_2 = m_1)
    {
        for(unsigned int liczba = m_1 * m_2; m_1 * m_2 <= granica; liczba = m_1 * m_2)
        {
            while(liczba <= granica)
            {
                // "wylacz" odpowiedni index z przedzialu oraz przestaw go o mnozkin dalej
                sieve[liczba] = true; liczba *= m_1;
            }
            // ustaw drugi czynnik na wartosc "pozostajaca" w tablicy
            while(sieve[++m_2]);
        }
        // ustaw pierwszy czynnik na wartosc "pozostajaca" w tablicy
        while(sieve[++m_1]);
    }
    // odczytaj te komorki tablicy ktore maja przypisana wartosc logiczna 0
    for(unsigned i = 2; i <= granica; i++)
        if(!sieve[i]) std::cout << i << " ";
    std::cout << '\n';

    // usun tablice dynamiczna
    delete[] sieve;
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
