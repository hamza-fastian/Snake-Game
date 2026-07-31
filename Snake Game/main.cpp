#include<iostream>
#include<conio.h>
#include<windows.h>
#include<ctime>
using namespace std;

const int length = 20;
const int width = 30;

bool gameOver = false;
bool paused = false;
string message = "";
string bonusMessage = "";

int score = 0;
int FoodX;
int FoodY;
bool bonus = false;

int speed = 100;

int snakeX[100];
int snakeY[100];
int snakeLength = 1;

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void welcomeScreen()
{
    system("cls");

    setColor(11);
    cout << "  ======================================\n";
    cout << "       S N A K E   A D V E N T U R E\n";
    cout << "  ======================================\n\n";

    setColor(14);
    cout << "            Welcome Player!\n\n";

    setColor(7);
    cout << "  Controls:" << endl;
    cout << "  W or w : Move Up" << endl;
    cout << "  S or s : Move Down" << endl;
    cout << "  A or a : Move Left" << endl;
    cout << "  D or d : Move Right" << endl;
    cout << "  P or p : Pause / Resume" << endl;

    setColor(13);
    cout << "\n  Eat food (*) and grow your snake!" << endl;
    cout << "  Don't hit walls or yourself." << endl;
    cout << "  Eat bonus ($) for extra points.\n\n";

    setColor(11);
    cout << "  Press ENTER to start your adventure...";

    cin.get();
    system("cls");
}

class Game {
public:
    void startGame()
    {
        snakeX[0] = 10;
        snakeY[0] = 10;
    }

    void update()
    {
        COORD coord = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        for (int i = 1; i <= length; i++)
        {
            if (i == 1 || i == length)
            {
                setColor(11);
                for (int j = 1; j <= width; j++)
                {
                    cout << "#";
                }
                cout << endl;
            }
            else
            {
                for (int k = 1; k <= width; k++)
                {
                    bool snakePart = false;

                    for (int s = 0; s < snakeLength; s++)
                    {
                        if (snakeX[s] == k && snakeY[s] == i)
                        {
                            snakePart = true;
                            break;
                        }
                    }

                    if (k == 1 || k == width)
                    {
                        setColor(11);
                        cout << "#";
                    }
                    else if (i == FoodY && k == FoodX)
                    {
                        setColor(12);
                        if (snakeLength % 5 == 0)
                        {
                            cout << "$";
                            bonus = true;
                        }
                        else
                        {
                            cout << "*";
                            bonus = false;
                        }
                    }
                    else if (snakePart)
                    {
                        if (snakeX[0] == k && snakeY[0] == i)
                        {
                            setColor(15);
                            cout << "@";
                        }
                        else
                        {
                            setColor(10);
                            cout << "0";
                        }
                    }
                    else
                    {
                        setColor(7);
                        cout << " ";
                    }
                }
                cout << endl;
            }
        }
        setColor(14);
        cout << "\n" << message << "          ";
        setColor(7);
    }
};

class Snake {
public:
    char direction = 'd';
    void move()
    {
        for (int i = snakeLength - 1; i > 0; i--)
        {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }

        switch (direction)
        {
        case 'w': snakeY[0]--; break;
        case 's': snakeY[0]++; break;
        case 'a': snakeX[0]--; break;
        case 'd': snakeX[0]++; break;
        }
    }
    bool grow()
    {
        if (snakeX[0] == FoodX && snakeY[0] == FoodY)
        {
            snakeLength++;
            if (snakeLength % 5 == 0)
                bonusMessage = "BONUS +5!";
            else
                bonusMessage = "FOOD +1!";
            FoodX = rand() % (width - 2) + 2;
            FoodY = rand() % (length - 2) + 2;

            return true;
        }

        return false;
    }
    void checkCollision()
    {
        if (snakeX[0] <= 1 || snakeX[0] >= width)
            gameOver = true;

        if (snakeY[0] <= 1 || snakeY[0] >= length)
            gameOver = true;

        for (int s = 1; s < snakeLength; s++)
        {
            if (snakeX[0] == snakeX[s] && snakeY[0] == snakeY[s])
                gameOver = true;
        }
    }
};

class Food {
public:
    void generateFood()
    {
        FoodX = rand() % (width - 2) + 2;
        FoodY = rand() % (length - 2) + 2;
    }
};

class Score {
public:
    void addScore()
    {
        if (bonus)
            score += 5;
        else
            score++;
    }
    void displayScore()
    {
        setColor(14);
        cout << "Score: " << score << endl;
        if (bonusMessage != "")
        {
            setColor(12);
            cout << bonusMessage << "          " << endl;
        }
        setColor(7);
    }
};

int main()
{
    welcomeScreen();
    int choice;

    setColor(5);
    cout << "  =================================\n";
    cout << "       SELECT DIFFICUTLY LEVEL\n";
    cout << "  =================================\n\n";
    setColor(7);

    cout << "\n  1. Easy\n  2. Medium\n  3. Hard\n";

    setColor(13);
    cout << "\n  Invalid input will start the game in Medium mode!" << endl;

    setColor(11);
    cout << "\n  Enter your choice: ";
    cin >> choice;

    system("cls");

    if (choice == 1)
        speed = 100;
    else if (choice == 2)
        speed = 70;
    else if (choice == 3)
        speed = 40;
    else
        speed = 70;

    Game g;
    Snake s;
    Food f;
    Score sc;

    srand(time(0));

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);

    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    g.startGame();
    f.generateFood();

    while (!gameOver)
    {
        if (_kbhit())
        {
            char key = _getch();

            if (key == 'p' || key == 'P')
            {
                paused = !paused;
                if (paused)
                    message = "GAME PAUSED";
                else
                    message = "GAME RESUMED";
            }

            if (!paused)
            {
                if ((key == 'w' || key == 'W') && s.direction != 's')
                    s.direction = 'w';
                else if ((key == 's' || key == 'S') && s.direction != 'w')
                    s.direction = 's';
                else if ((key == 'a' || key == 'A') && s.direction != 'd')
                    s.direction = 'a';
                else if ((key == 'd' || key == 'D') && s.direction != 'a')
                    s.direction = 'd';
            }
        }
        if (!paused)
        {
            s.move();
            s.checkCollision();

            if (s.grow())
                sc.addScore();

            g.update();
            sc.displayScore();
        }
        else
        {
            g.update();
        }
        Sleep(speed);
    }
    system("cls");

    setColor(12);
    cout << "\n============================\n";
    cout << "        GAME OVER!\n";
    cout << "============================\n\n";

    setColor(14);
    cout << "Final Score: " << score << endl;

    setColor(7);
    cout << "\nThanks for playing Snake Adventure!\n";

    system("pause");
    return 0;
}
