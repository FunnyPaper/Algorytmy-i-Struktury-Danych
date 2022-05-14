#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>

void quicksort(float* tab, int begin, int end);
void iterSwap(float*& a, float*& b);
void valueSwap(float& a, float& b);
void printTable(float* tab, unsigned int size, unsigned int begin = 0, int first = -1, int second = -1, int pivot = -1);

unsigned int ilosc_zamian = 0, ilosc_porownan = 0;

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
    // Dane
    unsigned int tabSize;
    int choice;
    std::cout << "Podaj rozmiar tablicy: "; std::cin >> tabSize;

    float* tab = new float[tabSize];

    // Wejscie
    std::cout << "Czy tablica ma zostac wygenerowana losowo? Dla tak \"1\", dla nie \"0\":";
    std::cin >> choice;

    if(choice)
    {
        std::srand(std::time(0));
        for(unsigned int i = 0; i < tabSize; i++)
           tab[i] = std::rand() % 2001 - 1000;
    }
    else
    {
        std::cout << "Podaj " << tabSize << " liczb:\n";
        for(unsigned int i = 0; i < tabSize; i++)
            std::cin >> tab[i];
    }
    // Wyjscie
    std::cout << "Tablica przed sortowaniem:\n";
    printTable(tab, tabSize);

    // Zacznij mierzyc czas
    czas.Start();

    //Sortowanie
    quicksort(tab, 0, tabSize - 1);

    // Przerwij pomiar czasu
    czas.Stop();

    std::cout << "Posortowana tablica:\n";
    printTable(tab, tabSize);

    // Wypisz czas szukania dominanty
    czas.Display();

    std::cout << "Dokonano " << ilosc_porownan << " porownan oraz " << ilosc_zamian << " zmian.\n";

    delete[] tab;

    return 0;
}

void quicksort(float* tab, int begin, int end)
{
    if(begin < end)
    {
        // Ustaw pierwszy wskaznik (pivot) na srodkowy element kazdej podtablicy
        float *pivot = tab + ((end-begin)/2 + begin);

        // Ustaw drugi wskaznik na koniec kazdej podtablicy
        float *tableEnd = tab + end;

        std::cout << "PODTABLICA:\n";
        printTable(tab, end+1, begin, (end-begin)/2 + begin, end, (end-begin)/2 + begin);

        // Wrzucamy pivot na koniec w dwoch krokach
        // 1. Wymieniamy wartosci tablicy tak by fizycznie wartosci byly zmienione
        valueSwap(*pivot, *tableEnd);

        // 2. Zamieniamy miejscami wskazniki wskazujace na srodkowy element podtablicy i jej koniec
        iterSwap(pivot, tableEnd);

        // Pivot znajduje sie w tab[end] !!!

        // Zadeklaruj zmienna pelniaca funkcje indeksu dla wartosci ktora nalezy zmienic i ustaw na odpowiednie pole
        int index = begin;
        while(tab[index] <= *pivot && index != end) index++; // Wybieramy pierwszy element wiekszy od pivotu

        // Glowna petla sluzaca do porownywania i zamieniania miejscami
        unsigned int porownan = 2;
        for(unsigned int i = index+1; i < end; i++, porownan++)
        {
            if(tab[i] <= *pivot)
            {
                printTable(tab, end+1, begin, index, i, end);
                valueSwap(tab[index++], tab[i]);
                // Dodajemy skumulowana ilosc porownan (w przypadku kiedy wskaxnik sie przesuwa ale nie dochodzi do zamiany)
                ilosc_porownan += porownan;
                // Resetujemy ilosc porownan
                porownan = 1;
            }
        }

        // Wypisujemy tablice po ostatniej iteracji -> to wypisanie jest scisle powiazane z petla wyzej
        printTable(tab, end+1, begin, index, end, end);

        // Pivot wraca na swoje miejsce, dodajemy pozostala ilosc porownan -> decyduje sie zrobic to poza petla poniewaz chce miec pelna kontrole
        // nad tym by pivot wrocil na swoje miejsce
        valueSwap(tab[index], tab[end]);

        // Dodajemy pozostala liczbe porownan skumulowanych w petli
        ilosc_porownan += porownan;

       // std::cout << "Zaraz po powrocie pivota:\n";
        printTable(tab, end+1, begin, -1, -1, index);

        // Kolejne odwolania
        quicksort(tab, begin, index - 1);
        quicksort(tab, index + 1, end);
    }
}
void iterSwap(float*& a, float*& b)
{
    float* temp;
    temp = a;
    a = b;
    b = temp;
}
void valueSwap(float& a, float& b)
{
    float temp;
    temp = a;
    a = b;
    b = temp;
    ilosc_zamian++;
}
// first, second i pivot to wartosci indexow ktore maja zostac wyroznione w konsoli
// W przypadku gdyby pivot pokrywal sie z first lub second to zostanie on pokolorowany tak jak w kazdej iteracji
void printTable(float* tab, unsigned int size, unsigned int begin, int first, int second, int pivot)
{
    for(unsigned int i = begin; i < size; i++)
    {
        if(pivot == i)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
            std::cout << tab[i];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << ' ';
        }
        else if(first == i || second == i)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
            std::cout << tab[i];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << ' ';
        }
        else std::cout << tab[i] << ' ';
    }
    std::cout <<std::endl;
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

    std::cout << "Sortowanie zajelo: " << hours << "h : " << minutes << "m : " << seconds << "s : " << miliseconds << "ms" << std::endl;
}
