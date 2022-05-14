#include <iostream>
#include <ctime>
#include <cstdlib>

class TreeBST
{
	struct Node
	{
		int m_key{};
		std::string m_pendant{};
		Node* m_left{}, *m_right{};
		
		~Node()
		{
			if(m_left) delete m_left;
			if(m_right) delete m_right;
		}	
	};
	
	Node* m_root{};
	Node* Search(int key);
	Node* _SearchParent(int key); // Przy usuwaniu potrzebny jest rodzic ¿eby odpowiednio usun¹æ dany node, a sama struktura node'a ma posiadac tylko wskaznik na lewego i prawego potomka
public:
	TreeBST() : m_root(NULL) {}
	
	~TreeBST()
	{
		if(m_root) delete m_root;
	}
	void Add(int key);
	void Add(int key, const std::string& pendant);
	void Print(int key);
	void Delete(int key);	
};

// funkcja generujaca losowe breloki
std::string RandomKey(); // zwraca napis o dlugosci 5 znakow

int main()
{
	std::srand(std::time(0));
	
	// 1. Zainicjalizuj puste drzewo. Klucz korzenia to 13. Brelok korzenia to imiê i nazwisko autora.
	std::cout << "Ad1 " << std::string(40, '*') << '\n';
	TreeBST drzewo;
	drzewo.Add(13, "Cezary Jarocki");
	
	// 2. Dodaj elementy o kluczach z zadania "Quicksort pdf". Breloki - losowe ci¹gi znaków o d³ugoœci 5.
	std::cout << "Ad2 " << std::string(40, '*') << '\n';
	int znane_klucze[10] = { 0, 73, 19, 52, 44, 23, 69, 45, 63, 62 };
	for(unsigned int i = 0; i < sizeof(znane_klucze) / sizeof(int); i++)
	{
		drzewo.Add(znane_klucze[i]);
	}
	
	// 3. ZnajdŸ i wyœwietl element o kluczu 13 (Wyœwietl klucz i brelok).
	std::cout << "Ad3 " << std::string(40, '*') << '\n';
	drzewo.Print(13);
	
	// 4. ZnajdŸ i wyœwietl element o losowym kluczu z zakresu 0-100 (Wyœwietl klucz i brelok).
	std::cout << "Ad4 " << std::string(40, '*') << '\n';
	int losowy_element = std::rand() % 101;
	drzewo.Print(losowy_element);
	
	// 5.ZnajdŸ i wyœwietl element o kluczu znanym (dowolny) (Wyœwietl klucz i brelok).
	std::cout << "Ad5 " << std::string(40, '*') << '\n';
	drzewo.Print(znane_klucze[2]);
	
	// 6. Usuñ element o kluczu z pkt 4.
	std::cout << "Ad6 " << std::string(40, '*') << '\n';
	drzewo.Delete(losowy_element);
	
	// 7. Usuñ element o kluczu znanym.
	std::cout << "Ad7 " << std::string(40, '*') << '\n';
	drzewo.Delete(znane_klucze[2]); // usun wczesniej wylosowany znany klucz
	
	// 8. ZnajdŸ i wyœwietl element o kluczu 13 (Wyœwietl klucz i brelok).
	std::cout << "Ad8 " << std::string(40, '*') << '\n';
	drzewo.Print(13);
	
	// 9. ZnajdŸ i wyœwietl element o losowym kluczu z zakresu 0-100 (Wyœwietl klucz i brelok).
	std::cout << "Ad9 " << std::string(40, '*') << '\n';
	losowy_element = std::rand() % 101;
	drzewo.Print(losowy_element);
	
	// 10. ZnajdŸ i wyœwietl element o kluczu znanym (dowolny) (Wyœwietl klucz i brelok).
	std::cout << "Ad10 " << std::string(39, '*') << '\n';
	drzewo.Print(znane_klucze[2]);
	
	return 0;
}

TreeBST::Node* TreeBST::Search(int key)
{
	Node* curr = m_root;
	while(curr && curr->m_key != key)
		curr = (key < curr->m_key) ? curr->m_left : curr->m_right;
	return curr;
}

TreeBST::Node* TreeBST::_SearchParent(int key)
{
	Node* curr = m_root;
	while(curr)
	{
		if(curr->m_left && key < curr->m_key)
		{
			if(curr->m_left->m_key == key) return curr;
			else curr = curr->m_left;
		}
		else if(curr->m_right && key > curr->m_key)
		{
			if(curr->m_right->m_key == key) return curr;
			else curr = curr->m_right;		
		}
	}
	return curr;
}

void TreeBST::Add(int key)
{
	Node* elem = new Node, *curr = m_root;
	elem->m_key = key;
	elem->m_left = NULL;
	elem->m_right = NULL;
	elem->m_pendant = RandomKey();
	
	if(curr)
	{
		while(true)
		{
			if(curr->m_key > key)
			{
				if(curr->m_left)
				curr = curr->m_left;
				else
				{
					curr->m_left = elem; break;
				}
			}
			else if(curr->m_key < key)
			{
				if(curr->m_right)
				curr = curr->m_right;
				else
				{
					curr->m_right = elem; break;
				}
			}
			else std::cout << "Wezel o podanym kluczu juz jest czescia drzewa\n";			
		}
	}
	else
	{
		m_root = elem;
	}
}

void TreeBST::Add(int key, const std::string& pendant)
{
	Node* elem = new Node, *curr = m_root;
	elem->m_key = key;
	elem->m_left = NULL;
	elem->m_right = NULL;
	elem->m_pendant = pendant;
	
	if(curr)
	{
		while(true)
		{
			if(curr->m_key > key)
			{
				if(curr->m_left)
				curr = curr->m_left;
				else
				{
					curr->m_left = elem; break;
				}
			}
			else if(curr->m_key < key)
			{
				if(curr->m_right)
				curr = curr->m_right;
				else
				{
					curr->m_right = elem; break;
				}
			}
			else std::cout << "Wezel o podanym kluczu juz jest czescia drzewa\n";			
		}
	}
	else
	{
		m_root = elem;
	}
}

void TreeBST::Print(int key)
{
	Node* curr = Search(key);
	if(curr)
	{
		std::cout << "Klucz: " << curr->m_key << ", brelok: " << curr->m_pendant << '\n';
	}
	else std::cout << "Nie mozna wyswietlic -> brak elementu o kluczu <" << key << ">\n";
}

void TreeBST::Delete(int key)
{
	Node* curr = Search(key), *temp = NULL;
	if(!curr)
	{
		std::cout << "Nie mozna usunac -> brak elementu o kluczu <" << key << ">\n";
	}
	else if(curr->m_left && curr->m_right)
	{
		std::cout << "Stopien 2, nie mozna usunac\n";
	}
	else if(curr->m_left)
	{
		temp = curr->m_left;
		curr->m_key = temp->m_key;
		curr->m_pendant = temp->m_pendant;
		curr->m_left = temp->m_left;
		curr->m_right = temp->m_right;
		temp->m_left = NULL;
		temp->m_right = NULL;
		delete temp; temp = NULL;
	}
	else if(curr->m_right)
	{
		temp = curr->m_right;
		curr->m_key = temp->m_key;
		curr->m_pendant = temp->m_pendant;
		curr->m_left = temp->m_left;
		curr->m_right = temp->m_right;
		temp->m_left = NULL;
		temp->m_right = NULL;
		delete temp; temp = NULL;
	}
	else
	{
		temp = _SearchParent(key);
		if(temp->m_left->m_key == key) temp->m_left = NULL;
		else if(temp->m_right->m_key == key) temp->m_right = NULL;
		delete curr; curr = NULL;
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
			num = (std::rand() % 75) + 48;
		} while((num > 'Z' && num < 'a') || (num > '9' && num < 'A'));
		// zmien typ z int na char i dodaj do napisu
		o_key += static_cast<char>(num);
	}

	// zwroc wygenerowany klucz
	return o_key;
}
