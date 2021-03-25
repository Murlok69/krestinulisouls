#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <random>


using namespace std;


enum GameVar
{
EMPTY = '-',
CROSS = 'X',
ZERO = '0'
};

enum GameProgress
{
	IN_PROGRESS,
	HUMAN_WIN,
	AI_WIN,
	DRAW
};


struct Game
{
	GameVar** ppField = nullptr;
	const size_t sizeX = 3;
	const size_t sizeY = 3;
	GameVar human;
	GameVar ai;
	bool humanFirst;
	GameProgress progress;
};


struct Coord
{
	size_t x, y;
};


void clearS()
{
	system("cls");
}

int32_t Random(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generation(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generation);
}


void printF(Game& rg)
{
	cout << "    ";
	for (int j = 0; j < rg.sizeX; j++)
	{
		cout << " " << j + 1 << "   "; 
	}

	cout << endl;
	for (int i = 0; i < rg.sizeY; i++)
	{
		cout << i + 1 << " || ";
		for (int j = 0; j < rg.sizeX; j++)
		{
			cout << (char)rg.ppField[i][j] << " || ";
		}
		cout << endl; 
	}
	cout << endl;
	cout << "Человек играет: " << (char)rg.human << endl;
	cout << "Робот Bender играет: " << (char)rg.ai << endl;
	cout << endl;
}


void InitGame(Game& rg)
{
	rg.ppField = new GameVar * [rg.sizeY];
	for (int i = 0; i < rg.sizeY; i++)
	{
		rg.ppField[i] = new GameVar[rg.sizeX];
	}

	for (int i = 0; i < rg.sizeY; i++)
		for (int j = 0; j < rg.sizeX; j++)
		{
			rg.ppField[i][j] = EMPTY;
		}


	if (Random(0, 1) > 0)
	{
		rg.human = CROSS;
		rg.ai = ZERO;
		rg.humanFirst = true;

	}
	else
	{
		rg.human = ZERO;
		rg.ai = CROSS;
		rg.humanFirst = false;

	}

	rg.progress = IN_PROGRESS;

}
	
void deinitGame(Game& rg)
{
	for (int i = 0; i < rg.sizeY; i++)
	{
		delete[] rg.ppField[i];
	}
	delete[] rg.ppField;
	rg.ppField = nullptr;

}


Coord getHumanCoor(Game& rg)
{
	Coord c = { 0 };

	do 
	{
		cout << "Введи x: ";
		cin >> c.x;

		cout << "Введи y: ";
		cin >> c.y;

	}	
	while (c.y <= 0 || c.x <= 0 || c.y > 3 || c.x > 3 || rg.ppField[c.y - 1][c.x - 1] != EMPTY);

	c.x--;
	c.y--;
	return c;
}

GameProgress WIN(Game rg)
{
	for (int y = 0; y < rg.sizeY; y++)
	{
		if (rg.ppField[y][0] == rg.ppField[y][1] && rg.ppField[y][0] == rg.ppField[y][2])
		{
			if (rg.ppField[y][0] == rg.human)
			{
				return HUMAN_WIN;
			}
			if (rg.ppField[y][0] == rg.ai)
			{
				return AI_WIN;
			}
		}
	}
	for (int x = 0; x < rg.sizeX; x++)
	{
		if (rg.ppField[0][x] == rg.ppField[1][x] && rg.ppField[0][x] == rg.ppField[2][x])
		{
			if (rg.ppField[0][x] == rg.human)
			{
				return HUMAN_WIN;
			}
			if (rg.ppField[0][x] == rg.ai)
			{
				return AI_WIN;
			}
		}
	}
	if (rg.ppField[0][0] == rg.ppField[1][1] && rg.ppField[0][0] == rg.ppField[2][2])
	{
		if (rg.ppField[0][0] == rg.human)
		{
			return HUMAN_WIN;
		}
		if (rg.ppField[0][0] == rg.ai)
		{
			return AI_WIN;
		}
	}
	if (rg.ppField[2][0] == rg.ppField[1][1] && rg.ppField[2][0] == rg.ppField[0][2])
	{
		if (rg.ppField[1][1] == rg.human)
		{
			return HUMAN_WIN;
		}
		if (rg.ppField[1][1] == rg.ai)
		{
			return AI_WIN;
		}
	}

	bool draw = true;
	for (int y = 0; y < rg.sizeY; y++)
		for (int x = 0; x < rg.sizeX; x++)
			if (rg.ppField[y][x] == EMPTY)
				draw = false;

	if (draw)
		return DRAW;

	return IN_PROGRESS;

}


Coord getAICoorH(Game& rg)
{
	Coord c = { 0 };

	for (size_t y = 0; y < rg.sizeY; y++)
	{
		for (size_t x = 0; x < rg.sizeX; x++)
		{
			if (rg.ppField[y][x] == EMPTY)
			{
				rg.ppField[y][x] = rg.ai;
				if (WIN(rg) == AI_WIN)
				{
					rg.ppField[y][x] = EMPTY;
					c = { x, y };
					return c;
				}
				rg.ppField[y][x] = EMPTY;
			}
		}
	}


	for (size_t y = 0; y < rg.sizeY; y++)
	{
		for (size_t x = 0; x < rg.sizeX; x++)
		{
			if (rg.ppField[y][x] == EMPTY)
			{
				rg.ppField[y][x] = rg.human;
				if (WIN(rg) == HUMAN_WIN)
				{
					rg.ppField[y][x] = EMPTY;
					c = { x, y };
					return c;
				}
				rg.ppField[y][x] = EMPTY;
			}
		}
	}



	if (rg.ppField[1][1] == EMPTY)
	{
		c = { 1,1 };
		return c;
	}

	if (rg.ppField[0][0] == EMPTY)
	{
		c = { 0,0 };
		return c;
	}

	if (rg.ppField[2][2] == EMPTY)
	{
		c = { 2,2 };
		return c;
	}

	if (rg.ppField[0][2] == EMPTY)
	{
		c = { 2,0 };
		return c;
	}

	if (rg.ppField[2][0] == EMPTY)
	{
		c = { 0,2 };
		return c;
	}
	if (rg.ppField[0][1] == EMPTY)
	{
		c = { 1,0 };
		return c;
	}

	if (rg.ppField[2][1] == EMPTY)
	{
		c = { 1,2 };
		return c;
	}

	if (rg.ppField[1][0] == EMPTY)
	{
		c = { 0,1 };
		return c;
	}

	if (rg.ppField[1][2] == EMPTY)
	{
		c = { 2,1 };
		return c;
	}

	return c;
}

Coord getAICoorE(Game& rg)
{
	Coord c = { 0 };


	if (rg.ppField[1][1] == EMPTY)
	{
		c = { 1,1 };
		return c;
	}

	if (rg.ppField[0][0] == EMPTY)
	{
		c = { 0,0 };
		return c;
	}

	if (rg.ppField[2][2] == EMPTY)
	{
		c = { 2,2 };
		return c;
	}

	if (rg.ppField[0][2] == EMPTY)
	{
		c = { 2,0 };
		return c;
	}

	if (rg.ppField[2][0] == EMPTY)
	{
		c = { 0,2 };
		return c;
	}
	if (rg.ppField[0][1] == EMPTY)
	{
		c = { 1,0 };
		return c;
	}

	if (rg.ppField[2][1] == EMPTY)
	{
		c = { 1,2 };
		return c;
	}

	if (rg.ppField[1][0] == EMPTY)
	{
		c = { 0,1 };
		return c;
	}

	if (rg.ppField[1][2] == EMPTY)
	{
		c = { 2,1 };
		return c;
	}

	return c;
}



int main()
{
	setlocale(LC_ALL, "Russian");
	clearS();
	Game game;
	InitGame(game);
	char n;

	cout << "WELCOME TO THE SOULS GAME" << endl;

	cout << "SELECT YOUR LVL MODE" << endl << "E - EASY" << endl << "H - HARD" << endl;

	cout << "Введи уровень: ";

	cin >> n;

	printF(game);
	size_t turn;
	turn = (game.humanFirst) ? 0 : 1;





	if (n == 'H')
	{
		while (game.progress == IN_PROGRESS)
		{

			if (turn % 2 == 0)
			{
				Coord c = getHumanCoor(game);
				game.ppField[c.y][c.x] = game.human;

			}
			else
			{
				Coord c = getAICoorH(game);
				game.ppField[c.y][c.x] = game.ai;

			}

			clearS();
			printF(game);
			turn++;

			game.progress = WIN(game);


		}
	}

	if (n == 'E')
	{
		while (game.progress == IN_PROGRESS)
		{

			if (turn % 2 == 0)
			{
				Coord c = getHumanCoor(game);
				game.ppField[c.y][c.x] = game.human;

			}
			else
			{
				Coord c = getAICoorE(game);
				game.ppField[c.y][c.x] = game.ai;

			}

			clearS();
			printF(game);
			turn++;

			game.progress = WIN(game);


		}

	}

	if (game.progress == DRAW)
		cout << " ЭЙ! Я ВЫИГРАЛ! Кусок мяса, и ты выиграл! Это уже не так весело  " << endl;

	else if (game.progress == AI_WIN)
		cout << "ПОЦЕЛУЙ МОЙ БЛЕСТЯЩИЙ ЗАД - Я ВЫИГРАЛ!" << endl;
	else if (game.progress == HUMAN_WIN)
		cout << "КУСОК МЯСА, ТЕБЕ ПОВЕЗЛО! ТЫ ОПЯТЬ ВЫИГРАЛ! " << endl;

	

}