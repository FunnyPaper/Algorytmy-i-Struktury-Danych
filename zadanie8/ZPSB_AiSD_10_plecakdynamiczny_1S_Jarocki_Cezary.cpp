#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

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
    long long Display();

private:
    // Zmienne czlonkowskie
    clock_t m_Start{}, m_Stop{};
};

class JaggedArray
{		
public:
	struct Element 
	{
		float** weight_table, **item_table;
		unsigned int table_width, table_height, *weight_index, *item_index;
		
		Element() : weight_table(NULL), item_table(NULL), weight_index(NULL), item_index(NULL), table_width(0), table_height(0) {}
		~Element()
		{
			DeleteJaggedArray();
		}
		void CreateJaggedArray(unsigned int width, unsigned int height, const bool* r_list)
		{
			if(width > 0 && height > 0)
			{
				if(table_width && table_height)
					DeleteJaggedArray();
					
				table_width = height; table_height = width;
				// tworzenie tablic
				weight_table = new float*[table_width];
				item_table = new float*[table_width];
				item_index = new unsigned int[table_width];
				weight_index = new unsigned int[table_height];
				for(unsigned int i = 0, offset = 0; i < table_width; i++)
				{
					weight_table[i] = new float[table_height]{};
					item_table[i] = new float[table_height]{};
					if(r_list[i]) offset++;
					item_index[i] = i+1+offset;
				}
				for(unsigned int i = 0; i < table_height; i++)
					weight_index[i] = i+1;
				
			}
			
		}
		void DeleteJaggedArray()
		{
			//usuwanie tablic
			for(unsigned int i = 0; i < table_width; i++)
			{
				delete[] weight_table[i];
				delete[] item_table[i]; 
			}
			delete[] item_index; delete[] weight_index;
			delete[] weight_table; delete[] item_table;
			weight_table = NULL; item_table = NULL;
			table_width = 0; table_height = 0;
		}
		//uzyskaj dostep do konretnego elementu dowolnej tablicy
		float& operator()(int row, int col, unsigned int table_type)
		{
			//przestaw wartosci ujemne na 0
			if(row < 0) row = 0;
			if(col < 0) col = 0;
			//na podstawie table_type wybierz odpowiednia tablice
			if(table_type < 2 && row < table_width && col < table_height)
			{
				float** ptr;
			
				switch(table_type)
				{
					case 0: ptr = weight_table;
						break;
					case 1: ptr = item_table;
						break;
				}
				//zwroc wartosc tablicy uzyskanej pod wplywem switch
				return ptr[row][col];
			}
			//jesli podano niepoprawne dane
			else
			{
				return weight_table[0][0];	
			}
		}
		//zwroc cala tablice na podstawie typu tablicy
		float**& operator[](unsigned int table_type)
		{
			switch(table_type)
			{
				case 0: return weight_table;
				case 1: return item_table;
				default: 
				{
					std::cout << "Cos poszlo nie tak 2!\n";
				}
			}
			return weight_table;
		}
		//wyswietl tablice pomocnicze
		void Print(unsigned int i)
		{
			if(table_width > 0 && table_height > 0)
			{
				std::cout << std::string(20, '+') << " POMOCNICZE_TABLICE " << i  << ' ' << std::string(20, '+') << std::endl;
				std::cout << "Tabelka z wartosciami:\n";
				std::cout << std::setw(16) << std::left  << "udzwig -> ";
				for(unsigned int i = 0; i < table_height; i++)
				{
					std::cout << std::setw(2) << std::left << weight_index[i] << ' ';
				}
				std::cout << "\n\n";
				for(unsigned int i = 0; i < table_width; i++)
				{
					std::cout << "przedmiot nr " << item_index[i] << ": ";
					for(unsigned int j = 0; j < table_height; j++)
					{
						std::cout << std::setw(2) << std::left << weight_table[i][j] << ' ';
					}
					std::cout << '\n';
				}
				std::cout << "\nTabelka z numerami przedmiotow:\n";
				std::cout << std::setw(16) << std::left  << "udziwg -> ";
				for(unsigned int i = 0; i < table_height; i++)
				{
					std::cout << std::setw(2) << std::left << weight_index[i] << ' ';
				}
				std::cout << "\n\n";
				for(unsigned int i = 0; i < table_width; i++)
				{
					std::cout << "przedmiot nr " << item_index[i] << ": ";
					for(unsigned int j = 0; j < table_height; j++)
					{
						std::cout << std::setw(2) << std::left << item_table[i][j] << ' ';
					}
					std::cout << '\n';
				}
				std::cout << '\n';
			}
		}
	};
	
private:
	unsigned int storage_size;
	bool* restrict_list; // dodatkowa lista wskazujaca na to ktore elementy nalezy pominac 
	Element* jagged_elems;

public:
	JaggedArray(unsigned int storage) : storage_size(storage), restrict_list(NULL) 
	{
		jagged_elems = new Element[storage];
		restrict_list = new bool[storage]{};
	}
	~JaggedArray()
	{
		delete[] jagged_elems; delete[] restrict_list;
	}
	//zwroc odpowiedni element zawierajacy tablice pomocnicze
	Element& operator[](unsigned int index)
	{
		if(index >= storage_size) std::cout << "Cos poszlo nie tak 3!\n";
		return jagged_elems[index];
	}
	//funkcje manipulujace restric_list
	void Restrict(unsigned int index)
	{
		restrict_list[index] = true;
	}
	bool IsRestricted(unsigned int index)
	{
		return restrict_list[index];
	}
	unsigned int GetUnrestrictedCount()
	{
		unsigned int i = storage_size;
		for(unsigned j = 0; j < storage_size; j++)
			if(restrict_list[j]) i--;
		return i;
	}
	const bool* GetRestrictedList() const
	{
		return restrict_list;
	}
};
// Metoda zachlanna
void quickSort(float** tab, int first_item, int last_item, unsigned int criteria, unsigned int criteria_num, bool (*logic)(float, float));
bool asc(float a, float b) { return a > b; };
bool desc(float a, float b) { return a < b; };
float profitBackpack(float backpack_capacity, float** backpack, unsigned int item_count, unsigned int& max_capacity);

// Metoda Dynamiczna
float optimalBackpack(float** discr, unsigned int item_count, float backpack_capacity, JaggedArray& info_tables, unsigned int depth, unsigned int& max_capacity);

// Dodatkowe pomocnicze funkcje
float** create2DArray(unsigned int width, unsigned int height);
void delete2DArray(float** tab, unsigned int height);

// Zegarek do odmierzania czasu
Timer czas;

int main()
{
	// Dane
    unsigned int item_count, criteria_num = 5, max_capacity_optimal = 0, max_capacity_profit = 0;
    float backpack_capacity = 0;
    long long time_optimal = 0, time_profit = 0;
    
    // Wyjscie + wejscie 1
    std::cout << "Pojemnosc plecaka: "; std::cin >> backpack_capacity;
    std::cout << "Ilosc przedmiotow: "; std::cin >> item_count;

	// Utworzenie pojemnika na wszystkie mozliwe tablice pomocnicze (w najgorszym wypadku bedzie ich tyle ile przedmiotow
	JaggedArray info_table(item_count);
	// Utworzenie tablicy na przedmioty
    float** item_description = create2DArray(criteria_num, item_count);

	// Wyjscie + wejscie 2
    std::cout << "Dla kazdego przedmiotu po kolei podaj cene, wage, ilosc szt. oddzielone spacjami\n";
    for(unsigned int i = 0; i < item_count; i++)
    {
        item_description[i][0] = i+1;
        std::cout << "Przedmiot " << item_description[i][0] << ": ";
        // cena, waga, ilosc
        for(unsigned int j = 1; j < 4; j++)
        {
            std::cin >> item_description[i][j];
        }
    }

	// Wyjscie -> rezultaty
    czas.Start();
    float weight_profit = profitBackpack(backpack_capacity, item_description, item_count, max_capacity_profit);
    czas.Stop(); time_profit = czas.Display();

	czas.Start();
	float weight_optimal = optimalBackpack(item_description, item_count, backpack_capacity, info_table, 0, max_capacity_optimal);
	czas.Stop(); time_optimal = czas.Display();

	// Formatowanie
	std::cout << std::string(40, '+') << std::endl;

	// Okreslanie ktora metoda jest szybsza/efektywniejsza
	if(time_profit >= time_optimal) std::cout << "Metoda Dynamiczna jest szybsza\n";
	else std::cout << "Metoda Zachlanna jest szybsza\n";

	if(max_capacity_optimal > max_capacity_profit) std::cout << "Metoda Dynamiczna jest efektywniejsza\n";
	else if(max_capacity_optimal < max_capacity_profit) std::cout << "Metoda Zachlanna jest efektywniejsza\n";
	else std::cout << "Obie metody sa tak samo efektywne\n";

	// Czy wyswietlic pomocnicze tablice
	bool choice;
	std::cout << "\nCzy wyswietlic tablice pomocnicze? Tak - \"1\", Nie - \"0\": "; std::cin >> choice;
	if(choice)
	{
		for(unsigned int i = 0; i < item_count; i++)
		{
			info_table[i].Print(i+1);	
		}		
	}

	// Zwolnij pamiec tablicy przedmiotow
	delete2DArray(item_description, item_count);
    return 0;
}

// Definicje klasy Timer:
long long Timer::Display()
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
    return miliseconds;
}
// Sortowanie
void quickSort(float** tab, int first_item, int last_item, unsigned int criteria, unsigned int criteria_num, bool (*logic)(float, float))
{
    if(first_item >= last_item) return;

    int i = first_item, j = last_item;
    float pivot = tab[(first_item + last_item)/2][criteria];

    while(true)
	{
		while(logic(pivot, tab[i][criteria])) i++;
		while(!logic(pivot, tab[j][criteria]) && pivot != tab[j][criteria]) j--;
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

	if(j > first_item)
	quickSort(tab, first_item, j, criteria, criteria_num, logic);
	if(i < last_item)
	quickSort(tab, i, last_item, criteria, criteria_num, logic);
}
// Metoda zachlanna
float profitBackpack(float backpack_capacity, float** backpack, unsigned int item_count, unsigned int& max_capacity)
{
    float bc = 0, sum = 0;

    for(unsigned int i = 0; i < item_count; i++)
    {
        backpack[i][4] = backpack[i][1] / backpack[i][2];
    }
    quickSort(backpack, 0, item_count-1, 4, 5, desc);

    for(unsigned int i = 0; i < item_count && bc <= backpack_capacity; i++)
    {
        backpack[i][4] = 0;
        for(unsigned int j = 0; j < backpack[i][3] && bc + backpack[i][2] <= backpack_capacity; j++)
        {
            bc += backpack[i][2]; backpack[i][4]++;
        }
        sum += backpack[i][4] * backpack[i][1];
    }

    quickSort(backpack, 0, item_count-1, 0, 5, asc);

    std::cout << '\n' << std::string(10, '+') << "Metoda zachlanna" << std::string(10, '+') << '\n';
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
    
    max_capacity = bc;
    return sum;
}
// Metoda Dynamiczna
float optimalBackpack(float** discr, unsigned int item_count, float backpack_capacity, JaggedArray& info_tables, unsigned int depth, unsigned int& max_capacity)
{
	// stworz postrzepiona tablice (wymiary sa zalezne od tego czy dany przedmiot moze wystepowac oraz jaki jest obecnie maksymalny udzwig plecaka)
	info_tables[depth].CreateJaggedArray(backpack_capacity, info_tables.GetUnrestrictedCount(), info_tables.GetRestrictedList());
	// petla wypelniajaca tablice pomocnicze
	for(int i = 0, offset_index = 0; i < item_count; i++)
	{
		// jesli dany przedmiot z tablicy przedmiotow nie jest juz dostepny to przejdz do nastepnej iteracji petli i zwieksz offset
		// postrzepione tablice (o roznych wymiarach) bazuja na tablicy przedmiotow ktora zawsze ma te same wymiary dlatego offset jest przydatny do odpowiedniego odwolywania sie
		if(info_tables.IsRestricted(i))
		{
			offset_index++; continue;
		}
		// glowna petla wypelniajaca tablice pomocnicze
	    for(int j = 0; j < backpack_capacity; j++)
		{
			if(i == 0)
			{
				if(j+1 >= discr[i][2])
				{
					info_tables[depth](i - offset_index, j, 0) = info_tables[depth](i - offset_index, j - (int)discr[i][2], 0) + discr[i][1]; 
					info_tables[depth](i - offset_index, j, 1) = (i+1);
				}
			}
	      	else if(j+1 >= discr[i][2] && info_tables[depth](i - 1 - offset_index, j, 0) <= info_tables[depth](i - offset_index, j - (int)discr[i][2], 0) + discr[i][1]) 
			{
	        	info_tables[depth](i - offset_index, j, 0) = info_tables[depth](i - offset_index, j - (int)discr[i][2], 0) + discr[i][1]; 
	        	info_tables[depth](i - offset_index, j, 1) = (i+1);
	      	} 
			else 
			{
				info_tables[depth](i - offset_index, j, 0) = info_tables[depth](i - 1 - offset_index, j, 0); 
				info_tables[depth](i - offset_index, j, 1) = info_tables[depth](i - 1 - offset_index, j, 1); 
	      	}
		}
	}
	// jesli jest to pierwsze odwolanie funkcji (funkcja jeszcze nie wywolala samej siebie) to ustaw wartosci bazowe
	if(depth == 0)
	{
		for(unsigned int i = 0; i < item_count; i++) discr[i][4] = 0;
		max_capacity = 0;
	};

	unsigned int bc = 0, sum = 0;
	unsigned int i = info_tables.GetUnrestrictedCount() - 1;
	// odczytuj zawartosci tablicy i pakuj plecak
	if(info_tables[depth](i, (int)backpack_capacity - 1, 0) == 0) return 0;

		// przede wszystkim odwolujemy sie przez indeksy przedmiotow zgromadzonych w tabeli pomocniczej dlatego ta petla to nie jest najlepszy widok
		// nasze j to maksymalny udzwig pomniejszony o 1 (zeby sie dalo wziac ostatni element tablicy)
		// warunek to j musi byc wieksze/rowne 0 oraz jednoczesnie waga wlozonych przedmiotow plus nastepny jeszcze nie wlozony przedmiot musi byc mniejsza badz rowna obecnemu maksymalnemu udziwgowi plecaka
		// j zmniejszamy o wartosc przedmiotu ktory wlozylismy ostatnio -> do wybrania przedmiotu znowu poslugujemy sie tablica pomocnicza przez co nie wyglada to najestetyczniej
        for(int j = (int)backpack_capacity - 1; j >= 0 && bc + discr[(int)info_tables[depth](i, j, 1)-1][2] <= backpack_capacity; j -= discr[(int)info_tables[depth](i, j, 1)-1][2])
        {
        	// na tym etapie mozemy w pelni stworzyc jakis skrot odwolujacy sie do danego indexu
        	int index = info_tables[depth](i, j, 1);
        	bool ch = discr[index-1][4] != discr[index-1][3];
        	if(ch)
        	{
        		bc += discr[index-1][2]; discr[index-1][4]++;
				sum += discr[index-1][1];
			}
			
			if((int)discr[index-1][4] >= (int)discr[index-1][3])
			{
				info_tables.Restrict(index-1); break;
			}
			
			//bardzo wazne -> w przypadku kiedy j w nastepnej iteracji wyniesie 0 przerwij petle
			if((int)info_tables[depth](i, j - discr[(int)info_tables[depth](i, j, 1)-1][2], 1) == 0) break;
        }
    // zapisz maksymalny udzwig uzyskany na tej glebokosci rekurencji
    max_capacity += bc;
    // sumuj wartosc oraz tworz podzbiory jesli zajdzie taka koniecznosc (brak wolnych kopii danego rpzedmiotu)
	if(backpack_capacity - bc > 0 && info_tables.GetUnrestrictedCount() > 0 && bc != 0)
	{
		sum += optimalBackpack(discr, item_count, backpack_capacity - bc, info_tables, depth+1, max_capacity);
	}
	// wypisz rezultat kiedy wrocisz do pierwszego odwolania
	if(depth == 0)
	{
		std::cout << '\n' << std::string(10, '+') << "Metoda dynamiczna" << std::string(10, '+') << '\n';
	    std::cout << std::string(item_count * 4 + 1, '-') << "\n|";
	    for(unsigned int i = 0; i < item_count; i++)
	    {
	        std::cout << ' ' << discr[i][0] << " |";
	    }
	    std::cout << "\n|";
	    for(unsigned int i = 0; i < item_count; i++)
	    {
	        std::cout << ' ' << discr[i][4] << " |";
	    }
	    std::cout << "\nWartosc plecaka: " << sum;
	    std::cout << "\nPoziom upakowania: " << max_capacity << '\n';	
	}
	// zwroc wartosc
	return sum;
}
// Funkcje pomocnicze
float** create2DArray(unsigned int width, unsigned int height)
{
	float** tab = new float*[height];
	for(int i = 0; i < height; i++) tab[i] = new float[width]{};
	return tab;
}
void delete2DArray(float** tab, unsigned int height)
{
  	for(int i = 0; i < height; i++) delete[] tab[i];
  	delete[] tab;
}
