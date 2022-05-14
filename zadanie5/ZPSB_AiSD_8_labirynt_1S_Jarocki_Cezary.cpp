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

// enum dla funkcji wyswietlajacej
enum MAZE
{
    PATH = -4, EXIT = -3, BRICK = -2, AIR = -1, START = 0
};

// funkcja tworzaca sciezke
void DrawEscapeRoute(int (*maze)[15], int legend[5], unsigned int& steps);
// funkcja wyswietlajaca labirynt
void PrintMaze(int (*maze)[15]);

// Zegar do pomiaru czasu
Timer czas;

int main()
{
    // labirynt prowadzacego
    int maze_1[15][15] = {
        {-2,-2,-2,-2,-2,-2,-2,-2,-1,-2,-2,-2,-2,-2,-2},
        {-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2},
        {-2,-2,-2,-2,-1,-2,-2,-2,-2,-2,-1,-2,-2,-1,-2},
        {-2,-1,-2,-1,-1,-1,-2,-1,-1,-1,-1,-2,-1,-1,-2},
        {-2,-1,-2,-1,-2,-1,-2,-2,-2,-2,-2,-2,-1,-2,-2},
        {-2,-1,-2,-1,-2,-1,-1,-1,-1,-1,-1,-2,-1,-2,-2},
        {-2,-1,-2,-1,-2,-1,-2,-2,-2,-2,-1,-2,-1,-1,-2},
        {-2,-1,-2,-1,-2,-1,-1,-1,-2,-1,-1,-2,-2,-1,-2},
        {-2,-1,-1,-1,-2,-2,-2,-2,-2,-2,-1,-2,-2,-1,-2},
        {-2,-1,-2,-1,-1,-1,-1,-1,-2,-1,-1,-1,-1,-1,-2},
        {-2,-1,-2,-2,-2,-2,-2,-1,-2,-1,-2,-2,-2,-1,-2},
        {-2,-1,-1,-1,-2,-1,-1,-1,-1,-1,-2,-1,-1,-1,-2},
        {-2,-1,-2,-2,-2,-1,-2,-2,-2,-2,-2,-2,-2,-1,-2},
        {-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2},
        {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2}
    };

    // labirynt studenta
    int maze_2[15][15] = {
        {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
        {-2,-1,-1,-1,-1,-1,-1,-2,-1,-1,-1,-1,-1,-1,-2},
        {-2,-1,-2,-2,-2,-2,-2,-2,-2,-2,-1,-2,-2,-1,-2},
        {-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2,-1,-1,-2},
        {-2,-1,-2,-2,-2,-1,-2,-2,-2,-2,-2,-2,-1,-2,-2},
        {-2,-1,-2,-1,-2,-1,-1,-1,-1,-1,-1,-2,-1,-2,-2},
        {-2,-1,-2,-1,-2,-1,-2,-2,-2,-2,-1,-2,-1,-1,-2},
        {-2,-1,-2,-1,-2,-1,-1,-1,-1,-1,-1,-2,-2,-1,-2},
        {-2,-1,-1,-1,-2,-2,-2,-2,-2,-2,-1,-2,-2,-1,-2},
        {-2,-1,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2},
        {-2,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-2},
        {-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2,-1,-1,-1,-2},
        {-2,-2,-2,-2,-2,-1,-2,-2,-2,-2,-2,-1,-2,-1,-2},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2,-1,-2},
        {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2}
    };

    // legenda do labiryntu (odpowiada enumowi MAZE)
    int legend[5] = { -4, -3, -2, -1, 0 }, choice;
    unsigned int steps, start_x, start_y, koniec_x, koniec_y; // liczba krokow + wspolrzedne poczatku/konca
    int (*maze)[15]; // wskaznik to przypisania odpowiedniego labiryntu

    // Wejscie
    std::cout << "Labirynt do wyboru:\n1.Labirynt prowadzacego\n2.Labirynt studenta\nKtory labirynt wybierasz: ";
    std::cin >> choice;

    // upewnij sie ze wybrano poprawny labirynt
    do {
        if(choice == 1) 
		{
			maze = maze_1; break;
		}
        else if(choice == 2) 
		{
			maze = maze_2; break;	
		}
        else
        {
            std::cout << "Nie ma takiego labiryntu\nPodaj nowy numer: ";
            std::cin >> choice;
        }
    } while(true);

    //Wyjscie + Wejscie (podawanie wspolrzednych)
    PrintMaze(maze);
    std::cout << "Poczatek X: "; std::cin >> std::hex >> start_x;
    std::cout << "Poczatek Y: "; std::cin >> start_y;
    std::cout << "Koniec X: "; std::cin >> koniec_x;
    std::cout << "Koniec Y: "; std::cin >> koniec_y;
    maze[start_y][start_x] = START;
    maze[koniec_y][koniec_x] = EXIT;

    std::cout << std::endl;

    czas.Start();
    //Algorytm
    DrawEscapeRoute(maze, legend, steps);
    czas.Stop();

    //Wyjscie
    PrintMaze(maze);

    std::cout << "Liczba wykonanych krokow: " << steps << "\n\n";
    czas.Display();

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
void DrawEscapeRoute(int (*maze)[15], int legend[5], unsigned int& steps)
{
    int pos = legend[4]; // wartosc poczatkowa = START
    unsigned int pos_i, pos_j; // wspolrzedne wyjscia
    bool IsNotEnd = true; // zmienna definiujaca przerwanie petel (bardzo niestabilne podejscie)
    // wypelnianie tablicy wartosciami
    while(IsNotEnd)
    {
        for(unsigned int i = 0; i < 15 && IsNotEnd; i++)
        {
            for(unsigned int j = 0; j < 15 && IsNotEnd; j++)
            {
                if(maze[i][j] == pos)
                {
                	if(i != 0)
                	{
                	    if(maze[i-1][j] == legend[1])
                    	{
                        	pos_i = i-1; pos_j = j;
                        	IsNotEnd = false; maze[i-1][j] = legend[0]; break;
                    	}
						if(maze[i-1][j] == legend[3]) maze[i-1][j] = pos+1;	
					}
					if(j != 0)
					{
	                    if(maze[i][j-1] == legend[1])
	                    {
	                        pos_i = i; pos_j = j-1;
	                        IsNotEnd = false; maze[i][j-1] = legend[0]; break;
	                    }
						if(maze[i][j-1] == legend[3]) maze[i][j-1] = pos+1;	
					}
					if(j != 14)
					{
	                    if(maze[i][j+1] == legend[1])
	                    {
	                        pos_i = i; pos_j = j+1;
	                        IsNotEnd = false; maze[i][j+1] = legend[0]; break;
	                    }	
	                    if(maze[i][j+1] == legend[3]) maze[i][j+1] = pos+1;
					}
					if(i != 14)
					{
	                    if(maze[i+1][j] == legend[1])
	                    {
	                        pos_i = i+1; pos_j = j;
	                        IsNotEnd = false; maze[i+1][j] = legend[0]; break;
	                    }
	                    if(maze[i+1][j] == legend[3]) maze[i+1][j] = pos+1;
					}
                }
            }
        }
        pos++;
    }
    steps = pos;
    IsNotEnd = true;
    // "chodzenie" po wartosciach i oznaczanie sciezki
    while(maze[pos_i][pos_j] != legend[4] && IsNotEnd)
    {
    	if(pos_i != 0)
    	{
    		if(maze[pos_i-1][pos_j] == legend[4])
       		{
            	maze[pos_i-1][pos_j] = legend[0]; IsNotEnd = false;
        	}
	        else if(maze[pos_i-1][pos_j] == pos-1)
	        {
	            maze[pos_i-1][pos_j] = legend[0]; pos_i--;
	        }	
		}
		if(pos_j != 0)
		{
	        if(maze[pos_i][pos_j-1] == legend[4])
	        {
	            maze[pos_i][pos_j-1] = legend[0]; IsNotEnd = false;
	        }	
	        else if(maze[pos_i][pos_j-1] == pos-1)
	        {
	            maze[pos_i][pos_j-1] = legend[0]; pos_j--;
	        }
		}
		if(pos_i != 14)
		{
	        if(maze[pos_i+1][pos_j] == legend[4])
	        {
	            maze[pos_i+1][pos_j] = legend[0]; IsNotEnd = false;
	        }
	        else if(maze[pos_i+1][pos_j] == pos-1)
	        {
	            maze[pos_i+1][pos_j] = legend[0]; pos_i++;
	        }
		}
		if(pos_j != 14)
		{
	        if(maze[pos_i][pos_j+1] == legend[4])
	        {
	            maze[pos_i][pos_j+1] = legend[0]; IsNotEnd = false;
	        }
	        else if(maze[pos_i][pos_j+1] == pos-1)
	        {
	            maze[pos_i][pos_j+1] = legend[0]; pos_j++;
	        }	
		}
        pos--;
    }
}
void PrintMaze(int (*maze)[15])
{
    std::cout << std::hex << "  ";
    for(unsigned int i = 0; i < 15; i++)
    {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    for(unsigned int i = 0; i < 15; i++)
    {
        std::cout << i << ' ';
        for(unsigned int j = 0; j < 15; j++)
        {
            switch(maze[i][j])
            {
                case BRICK: std::cout << '#';
                    break;
                case AIR: std::cout << ' ';
                    break;
                case PATH: std::cout << '$';
                    break;
                case START: std::cout << 'O';
                    break;
                case EXIT: std::cout << 'X';
                    break;
                default: std::cout << ' ';
            }
            std::cout << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::dec;
}
