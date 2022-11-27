#include <iostream>
using namespace std;

#include <vector>
#include <windows.h>
#include <conio.h>
#include <cstring>
#include <time.h>
#include <random>

#define WIDTH 40
#define HEIGHT 20
#define BODY 'x'
#define APPLE 'o'

enum class Direction
{
	up,
	right,
	down,
	left
};

struct Point
{
	int x;
	int y;
};


#pragma region GlobalVariable
vector<Point> snake = {
	Point{ WIDTH / 2 + 2, HEIGHT / 2 },
	Point{ WIDTH / 2 + 1, HEIGHT / 2 },
	Point{ WIDTH / 2, HEIGHT / 2 },
	Point{ WIDTH / 2 - 1, HEIGHT / 2 },
	Point{ WIDTH / 2 - 2, HEIGHT / 2 }
};
Direction direction = Direction::right;
Point apple;
int score = 0;
int speed = 300;
Point prevTail;
#pragma endregion


#pragma region Prototype
void drawSnakePart(Point);
void drawSnake();
void gotoxy(int, int);
void ShowConsoleCursor(bool);
void move();
void drawBox();
bool isHitWall();
bool isBiteItself();
void drawHeadnTail();
void genApple();
bool isAteApple();
void growing();
void displayScore();
void showEndMenu();
void startGame();
void resetSnake();
void showStartMenu();
#pragma endregion



int main()
{
	showStartMenu();
	return 0;
}


#pragma region GameFunction
void drawBox()
{
	for (size_t i = 0; i < WIDTH; i++)
		cout << '=';
	gotoxy(0, HEIGHT);
	for (size_t i = 0; i < WIDTH; i++)
		cout << '=';
	for (size_t i = 1; i < HEIGHT; i++)
	{
		gotoxy(0, i);
		cout << '|';
	}
	for (size_t i = 1; i < HEIGHT; i++)
	{
		gotoxy(WIDTH, i);
		cout << '|';
	}
}

bool isHitWall()
{
	return snake[0].x == 0 || snake[0].y == 0 || snake[0].x == WIDTH || snake[0].y == HEIGHT;
}

void genApple()
{
	srand(time(0));
	int x = rand() % (WIDTH - 1) + 1;
	int y = rand() % (HEIGHT - 1) + 1;
	apple = {
		x,
		y,
	};
	gotoxy(x, y);
	cout << APPLE;
}

bool isAteApple()
{
	return snake[0].x == apple.x && snake[0].y == apple.y;
}

// Show score on right side of the board
void displayScore()
{
	gotoxy(WIDTH + 5, 2);
	cout << "Your score: " << score << endl;
	gotoxy(WIDTH + 5, 4 );
	cout << "-- Guide --" << endl;
	gotoxy(WIDTH + 5, 5);
	cout<<"UP : W"<<endl;
	gotoxy(WIDTH + 5, 6);
	cout << "DOWN : S" << endl;
	gotoxy(WIDTH + 5, 7);
	cout << "LEFT : A" << endl;
	gotoxy(WIDTH + 5, 8);
	cout << "RIGHT : D" << endl;
	gotoxy(WIDTH + 5, 9); 
	cout << "-- Option --" << endl;
	gotoxy(WIDTH + 5, 10);
	cout << "EXIT : E" << endl;
	
}


// Show menu at the end of the game
void showEndMenu()
{
	system("cls");
	gotoxy(0, 0);
	cout << "Game over!" << endl;
	cout << "Diem : " << score << endl;
	cout << "ban muon choi lai? ";
	char option;
	cin >> option;
	option = tolower(option);
	if (option == 'y')
	{
		resetSnake();
		startGame();
	}
	else if (option == 'n')
		exit(1);
}

void startGame()
{
	system("cls");
	ShowConsoleCursor(false);
	drawBox();
	drawSnake();
	genApple();
	displayScore();

	while (true)
	{
		if (_kbhit())
		{
			char ch = _getch();
			ch = tolower(ch);
			if (ch == 'a' && direction != Direction::right)
				direction = Direction::left;
			else if (ch == 'w' && direction != Direction::down)
				direction = Direction::up;
			else if (ch == 's' && direction != Direction::up)
				direction = Direction::down;
			else if (ch == 'd' && direction != Direction::left)
				direction = Direction::right;
			else if (ch == 'e') // Exit game
			{
				showEndMenu();
				break;
			}
		}
		move();
		drawHeadnTail();
		if (isAteApple())
		{
			score++;
			displayScore();
			growing();
			genApple();
		}
		if (isBiteItself())
		{
			ShowConsoleCursor(true);
			showEndMenu();
			break;
		}
		if (isHitWall())
		{
			ShowConsoleCursor(true);
			showEndMenu();
			break;
		}
		Sleep(speed);
	}
}

void resetSnake()
{
	score = 0;
	direction = Direction::right;
	snake = {
		Point{ WIDTH / 2 + 2, HEIGHT / 2 },
		Point{ WIDTH / 2 + 1, HEIGHT / 2 },
		Point{ WIDTH / 2, HEIGHT / 2 },
		Point{ WIDTH / 2 - 1, HEIGHT / 2 },
		Point{ WIDTH / 2 - 2, HEIGHT / 2 }
	};
}


void showStartMenu()
{
	system("cls");
	cout << "Welcome to snake game!" << endl;
	cout << "Team : 5 Anh em sieu nhan " << endl;
	cout << "  ==============" << endl;
	cout << "Menu :" << endl;
	cout << "1. Start" << endl;
	cout << "2. Exit" << endl;
	cout << "Your choice: ";
	int option;
	cin >> option;
	if (option == 1)
	{
		system("cls");
		cout << "Choose your level (1 - 5): ";
		int t;
		cin >> t;
		speed = 600 - t * 100;
		system("cls");
		cout << "Tip: While playing game, you can press 'e' to exit";
		gotoxy(0, 3);
		cout << "Ready!";
		Sleep(1000);
		for (size_t i = 3; i > 0; i--)
		{
			gotoxy(0, 3);
			cout << i << "         ";
			Sleep(1000);
		}
		gotoxy(0, 3);
		cout << "GO!";
		Sleep(1000);
		startGame();
	}
	else if (option == 2)
		exit(1);
}
#pragma endregion


#pragma region SnakeFunction
// Draw a part of snake
void drawSnakePart(Point p)
{
	gotoxy(p.x, p.y);
	cout << BODY;
}

// Draw whole snake
void drawSnake()
{
	for (size_t i = 0; i < snake.size(); i++)
		drawSnakePart(snake[i]);
}

// move the snake
void move()
{
	prevTail = snake.back();
	for (size_t i = snake.size() - 1; i > 0; i--)
		snake[i] = snake[i - 1];
	if (direction == Direction::up)
		snake[0].y -= 1;
	else if (direction == Direction::down)
		snake[0].y += 1;
	else if (direction == Direction::left)
		snake[0].x -= 1;
	else if (direction == Direction::right)
		snake[0].x += 1;
}

// Redraw head & tail to make the snake move
void drawHeadnTail()
{
	gotoxy(snake[0].x, snake[0].y);
	cout << BODY;
	gotoxy(prevTail.x, prevTail.y);
	cout << ' '; // Clear the old tail
}

// Check if snake bite itself
bool isBiteItself()
{
	Point head = snake[0];
	for (size_t i = 1; i < snake.size(); i++)
		if (head.x == snake[i].x && head.y == snake[i].y)
			return true;
	return false;
}

// Growing snake when it ate an apple
void growing()
{
	snake.push_back(prevTail);
}
#pragma endregion


#pragma region ConsoleFunction
// Goto position (x, y)
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

// Set the visibility of cursor
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}
#pragma endregion