#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>
#include <set>

// Poziomy wsparcia i ufnosci (w %):
#define MIN_CONFIDENCE 30 // -> 30%
#define MIN_SUPPORT 30 // -> 30%

// Level to dodatkowa zmienna opcjonalna -> pozwala na wyswietlenie jednej ze wszystkich wygenerowanych list z produktami (level 0 -> szansa na wziecie pierwszego produktu, level 1 -> drugiego produktu, etc)
void Apriori(std::string* product_list, unsigned int product_num, std::vector<std::string>*& baskets, unsigned int basket_num, unsigned int level = 2);
// Funkcja pomocnicza
bool IsSubset(std::set<std::string>& first, std::set<std::string>& second);

int main()
{
	std::srand(std::time(0));
	
	// Dodatkowe zmienne
	unsigned int basket_num, product_num, lower_bound, upper_bound;
	bool check = false;
	
	// Wyjscie + wejscie
	std::cout << "Ile koszykow?: "; std::cin >> basket_num;
	
	// Utworz pojemnik dynamiczny na koszyki
	std::vector<std::string>* koszyki = new std::vector<std::string>[basket_num]();
	
	// Wyjscie + wejscie 
	std::cout << "Ile roznych produktow?: "; std::cin >> product_num;
	std::cout << "Lista produktow:\n";
	
	// Utworz pojemnik na unikalne produkty (spis przedmiotow)
	std::string* product_list = new std::string[product_num]();
	std::cin.ignore(); // potrzebne dla prawidlowego dzialania std::getline (problem dodatkowego entera tworzonego przez std::cin)
	
	// Wejscie -> Podaj produkty
	for(unsigned int i = 0; i < product_num; i++)
	{
		std::cout << i+1 << ". ";
		std::getline(std::cin, product_list[i]);
	}
	
	// Wyjscie + wejscie -> Podaj zasieg pojemnosci koszykow
	std::cout << "Ile produktow na koszyk?\nod: "; std::cin >> lower_bound;
	std::cout << "do: "; std::cin >> upper_bound;
	
	// Wygeneruj losowe koszyki
	for(unsigned int i = 0; i < basket_num; i++)
	{
		unsigned int size = std::rand() % (upper_bound - lower_bound + 1) + lower_bound;
		for(unsigned int j = 0; j < size; j++)
		{
			unsigned int val = std::rand() % product_num;
			koszyki[i].push_back(product_list[val]);
		}
	}
	
	// Wyjscie + wejscie
	std::cout << "Czy wyswietlic liste towarow w koszykach (1 - tak, 0 - nie)?: "; std::cin >> check;
	std::cout << '\n';
	if(check)
	{
		// Wypisywanie koszykow
		for(unsigned int i = 0; i < basket_num; i++)
		{
			std::cout << "Koszyk " << i + 1 << ":\n";
			std::cout << koszyki[i][0];
			for(unsigned int j = 1; j < koszyki[i].size(); j++)
			{
				std::cout << ", " << koszyki[i][j];
			}
			std::cout << '\n';			
		}
	}
	std::cout << '\n';
	// Wywolanie funkcji
	Apriori(product_list, product_num, koszyki, basket_num);
	
	// Zwalniamy pamiec
	delete[] koszyki;
	delete[] product_list;		
    return 0;
}

void Apriori(std::string* product_list, unsigned int product_num, std::vector<std::string>*& baskets, unsigned int basket_num, unsigned int level)
{
	// Definicje typow zlozonych -> dla prostszej nawigacji/ tworzenia zmiennych
	typedef std::pair<std::string, float> 							elem_t;
	typedef std::vector<elem_t> 									next_t;
	typedef std::pair<std::set<std::string>, float>	 				set_t;
	typedef std::pair<set_t, next_t> 								node_t;
	 
	// Bez wyzej wymienionych definicji trzeba by bylo stworzyc bardzo dlugi typ danych
	std::list<node_t>* levels = new std::list<node_t>[product_num];

	// Zmienne pomocnicze -> wsparcie i ufnosc oraz ilosc wystapien obecnego zestawu w koszykach + uwzglednienie nastepnego produktu
	float set_appearance = 0, next_element_appearance = 0, support = 0, confidence = 0;

	// Wypelnianie pierwszej listy w specyficzny sposob	
	std::set<std::string> new_set; // Tworzymy pusty zestaw
	set_t current_set_compound = make_pair(new_set, 0); // Tworzymy pare z zestawu oraz liczby 0 (na samym poczatku nie ma ani jednego produktu)
	next_t current_set_subsets; // Przygotowujemy pojemnik na produkty ktore beda uwzglednione (wsparcie w tym wypadku jest rowne ufnosci)
	for(unsigned int powtorz = 0; powtorz < product_num; powtorz++, next_element_appearance = 0)
	{	
		for(unsigned int i = 0; i < basket_num; i++)
		{
			std::set<std::string> basket_set(baskets[i].begin(), baskets[i].end());
			if(basket_set.find(product_list[powtorz]) != basket_set.end()) next_element_appearance++;
		}
		confidence = (next_element_appearance / basket_num) * 100;
		if(confidence > MIN_CONFIDENCE)	current_set_subsets.push_back(make_pair(product_list[powtorz], confidence));	
	}
	levels[0].push_back(make_pair(current_set_compound, current_set_subsets)); // Zalaczamy utowrzony element do pierwszej listy			
	
	// Wypelnianie pozostalych list na podstawie poprzedniej listy (np lista 2 na podstawie listy 1, 3 na podstawie 2, etc)
	for(unsigned int list_num = 1; list_num < product_num; list_num++)
	{
		// Ponizsza petla wykorzystuje iterator dla poprzedniej listy
		for(std::list<node_t>::iterator poprzedni = levels[list_num-1].begin(); poprzedni != levels[list_num-1].end(); poprzedni++)
		{
			// Tworzymy pusty zestaw w oparciu o iterator wskazujacy na zestaw z poprzedniej listy (kopiujemy zestaw)
			std::set<std::string> new_set = poprzedni->first.first;
			
			// Tworzymy wszystkie mozliwe zestawy w oparciu o skopiowany zestaw oraz vector produktow z poprzedniej listy
			// (zgodnie z wykladem tworzymy zestaw wiekszy o jeden element, ktorego ufnosc w poprzednim zestawie byla wieksza od minimalnej ufnosci) 
			for(unsigned int p = 0; p < poprzedni->second.size(); p++, set_appearance = 0)
			{
				// Jesli nie znaleziono takiego elementu w obecnym zestawie to...
				if(new_set.find(poprzedni->second[p].first) == new_set.end())
				{
					// ... tymczasowo wrzuc go do zestawu
					new_set.insert(poprzedni->second[p].first);
					bool skip = false; // spontaniczna zmienna do kontroli
					// Sprawdz czy powiekszony zestaw juz nie znajduje sie w obecnej liscie
					for(std::list<node_t>::iterator prev = levels[list_num].begin(); prev != levels[list_num].end(); prev++)
					{
						if(prev->first.first == new_set) skip = true; // Jesli sie znajduje to ustaw flage
					}
					// Jesli nie ma takiego zestawu w obecnej liscie to...
					if(!skip)
					{
						// ...sprawdz ile razy ten zestaw jest podzbiorem w vectorze koszykow
						for(unsigned int i = 0; i < basket_num; i++)
						{
							// Stworz tymczasowa zmienna zbioru z koszyka -> pozwala to na latwiejsze porownywanie czy dany zbior jest podzbiorem (eliminuje powtarzajace sie elementy)
							std::set<std::string> basket_set(baskets[i].begin(), baskets[i].end());
							if(IsSubset(new_set, basket_set)) set_appearance++; // Zwieksz liczbe wystapien
						}
						// Policz wsparcie zestawu
						support = (set_appearance / basket_num) * 100;
						// Jesli wsparcie jest wieksze niz zakladane minimum to... 
						if(support > MIN_SUPPORT)
						{
							// ... utworz odpowiednie czesci obecnego elementu listy
							set_t current_set_compound = make_pair(new_set, support); // obecny zestaw wraz z liczba jego wystapien 
							next_t current_set_subsets; // vector na elementy ufnosci
							// Utworz elementy ufnosci w oparciu o poprzednie elementy ufnosci
							// (zamiast uzywac pelnej listy produktow -> w ten sposob zmniejszamy potencjalnie niepotrzebne odwolania - i tak by nie byly brane pod uwage)
							for(unsigned int powtorz = 0; powtorz < poprzedni->second.size(); powtorz++, next_element_appearance = 0)
							{
								// Jesli nie znaleziono elementu ufnosci w obecnym zestawie to...
								if(new_set.find(poprzedni->second[powtorz].first) == new_set.end())
								{
									// ... dodaj go tymczasowo do obecnego zestawu
									new_set.insert(poprzedni->second[powtorz].first);
									// Powtorz procedure sprawdzania czy dany zbior jest podzbiorem
									for(unsigned int i = 0; i < basket_num; i++)
									{
										std::set<std::string> basket_set(baskets[i].begin(), baskets[i].end());
										if(IsSubset(new_set, basket_set)) next_element_appearance++;
									}
									// Usun element ufnosci z zestawu (byl tylko potrzebny do liczenia)
									new_set.erase(poprzedni->second[powtorz].first);
									// Policz ufnosc danego elementu
									confidence = (next_element_appearance / set_appearance) * 100;
									// Zapisz pare - element ufnosci oraz liczbe jego wystapien - i wrzuc do vectora jesli spelnia warunek minimalnej ufnosci
									if(confidence > MIN_CONFIDENCE) current_set_subsets.push_back(make_pair(poprzedni->second[powtorz].first, confidence));
								}
							}
							// Stworz element listy w oparciu o utworzony zestaw oraz vector elementow ufnosci oraz zapisz do listy
							node_t element_listy = make_pair(current_set_compound, current_set_subsets);
							levels[list_num].push_back(element_listy);		
						}
					}
					// Usun dorzucony na poczatku petli element z zestawu (obecny zestaw jest tylko wzorcem do dokladania i usuwania elementow)
					new_set.erase(poprzedni->second[p].first);
				}
			}
		}
	}
	// Wyjscie:
	std::cout << std::string(20, '+') << " Prawdopodobienstwo (wsparcie: " << MIN_SUPPORT << "%, ufnosc: " << MIN_CONFIDENCE << "%) zakupu " << level+1 << " produktu " << std::string(20, '+') << '\n';
	for(std::list<node_t>::iterator it = levels[level].begin(); it != levels[level].end(); it++)
	{
		for(unsigned int j = 0; j < it->second.size(); j++)
		{
			for(std::set<std::string>::iterator st = it->first.first.begin(); st != it->first.first.end();)
			{
				std::cout << *st;
				if(++st != it->first.first.end()) std::cout << " + ";		
			}
			std::cout << " -> " << it->second[j].first << " (" << it->second[j].second << "%)\n";
		}
	}		
	// Zwalniamy pamiec
	delete[] levels;
}
// Funkcja pomocnicza -> sprawdza czy zestaw jest podzbiorem innego zestawu
bool IsSubset(std::set<std::string>& first, std::set<std::string>& second)
{
	if(second.size() < first.size()) return false;
	std::set<std::string>::iterator s = second.begin(), f = first.begin();
	for(; f != first.end(); f++)
	{
		if(second.find(*f) == second.end()) return false;
	}
	return true;
}
