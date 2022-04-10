#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
enum Color { PINK = 13, DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4, BLACK = 0 };
enum MazeObject { HALL, WALL, COIN, ENEMY, BORDER, GRAVE, HEALTHBOX };
enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

void VisualSetting();
void FillMaze(int maze[][40], int H, int W);
void ShowMaze(int maze[][40], int H, int W);
void ShowHelth(int wf, int healthf);
void ShowEnergy(int wf,int energyf);
void ShowSteps(int wf, int stepsf);
void ShowCoins(int wf, int coinsf);
void GenPosition(int X1, int Y1);
int main();

int main()
{
    const int WIDTH = 40; // ширина лабиринта
    const int HEIGHT = 10; // высота лабиринта
    int health = 100;
    int steps = 0;
    int coins = 0; // счётчик собранных монет
    int energy = 110;
    int maze[HEIGHT][WIDTH];
    COORD infobox;
    COORD position;// координаты нашего персонажа
    position.X = 0;
    position.Y = 2;
    VisualSetting();
    FillMaze(maze, HEIGHT, WIDTH);
    ShowMaze(maze, HEIGHT, WIDTH);
    /////////////////////////////////////////////////////////////////////
    GenPosition(0,2);
    ////////////////////////////////////////////////////////////////////
    // информация по всем показателям
    ShowCoins(WIDTH, coins);
    ShowHelth(WIDTH, health);
    ShowSteps(WIDTH, steps);
    ShowEnergy(WIDTH, energy);
    int coinquantity = 0;//(я добавила)

    while (true)
    {

        // keyboard hit
        if (_kbhit()) // если было нажатие на клавиши пользователем

        {
            int code = _getch(); // get character, получение кода нажатой клавиши
            if (code == 224) { // если это стрелка
                code = _getch(); // получить конкретный код стрелки
            }

            // стирание персонажика в старой позиции
            SetConsoleCursorPosition(h, position);
            if (maze[position.Y][position.X] == MazeObject::GRAVE)
            {
                SetConsoleTextAttribute(h, Color::PINK);
                cout << "X";
            }
            else
            {
                SetConsoleTextAttribute(h, Color::BLACK);
                cout << " ";
            }

            if (code == KeyCode::ENTER && energy >= 50)
            {
                energy -= 50;
                int radius = 3;
                for (int y = position.Y - radius; y <= position.Y + radius; y++)
                {
                    for (int x = position.X - radius; x <= position.X + radius; x++)
                    {
                        if (x >= 0 && x <= WIDTH - 1 &&
                            y >= 0 && y <= HEIGHT - 1 &&
                            maze[y][x] == MazeObject::ENEMY)
                        {
                            maze[y][x] = MazeObject::GRAVE;
                            COORD effects;
                            effects.X = x;
                            effects.Y = y;
                            SetConsoleCursorPosition(h, effects);
                            SetConsoleTextAttribute(h, Color::PINK);
                            cout << "X";
                        }
                    }
                }


            }
            else if (code == KeyCode::ESCAPE)
            {
                cout << "ESCAPE!\n";
                exit(0);
            }
            else if (code == KeyCode::SPACE)
            {
                cout << "SPACE!\n";
                exit(0);
            }
            else if (code == KeyCode::LEFT
                && maze[position.Y][position.X - 1] != MazeObject::WALL
                && maze[position.Y][position.X - 1] != MazeObject::BORDER)
            {
                position.X--;
                steps++;
                energy--;
            }
            else if (code == KeyCode::RIGHT // если ГГ собрался пойти направо
                && maze[position.Y][position.X + 1] != MazeObject::WALL
                && maze[position.Y][position.X + 1] != MazeObject::BORDER)
                // и при этом в лабиринте на той же строке (где ГГ) и
                // немного (на одну ячейку) правее на 1 столбец от ГГ
            {
                position.X++;
                steps++;
                energy--;
            }
            else if (code == KeyCode::UP
                && maze[position.Y - 1][position.X] != MazeObject::WALL
                && maze[position.Y - 1][position.X] != MazeObject::BORDER)
            {
                position.Y--;
                steps++;
                energy--;
            }
            else if (code == KeyCode::DOWN
                && maze[position.Y + 1][position.X] != MazeObject::WALL
                && maze[position.Y + 1][position.X] != MazeObject::BORDER)
            {
                position.Y++;
                steps++;
                energy--;
            }

            // показ ГГ в новой позиции
            SetConsoleCursorPosition(h, position);
            SetConsoleTextAttribute(h, Color::BLUE);
            cout << (char)2;

            /////////////////////////////////////
            // нормальный показ количества шагов
            infobox.X = WIDTH + 1;
            infobox.Y = 3;
            SetConsoleCursorPosition(h, infobox);
            SetConsoleTextAttribute(h, Color::DARKYELLOW);
            cout << "STEPS: ";
            SetConsoleTextAttribute(h, Color::YELLOW);
            cout << steps << "\n";

            infobox.X = WIDTH + 1;
            infobox.Y = 4;
            SetConsoleCursorPosition(h, infobox);
            SetConsoleTextAttribute(h, Color::DARKRED);
            cout << "ENERGY: ";
            SetConsoleTextAttribute(h, Color::RED);
            cout << energy << " \n";

            ////////////////////////////////////////////////////////////////
            // пересечение с элементами массива
            if (position.Y == HEIGHT - 3 &&
                position.X == WIDTH - 1)
            {
                MessageBoxA(0, "you find exit!", "WIN!", 0);
                system("cls");
                main(); // для запуска сначала, но на другой рандомной локации
            }

            // пересечение с монетками
            // если в лабиринте по позиции ГГ (под ним) находится монетка
            if (maze[position.Y][position.X] == MazeObject::COIN)
            {
                energy += 10;

                infobox.X = WIDTH + 1;
                infobox.Y = 4;
                SetConsoleCursorPosition(h, infobox);
                SetConsoleTextAttribute(h, Color::DARKRED);
                cout << "ENERGY: ";
                SetConsoleTextAttribute(h, Color::RED);
                cout << energy << " \n";

                coins++; // на одну монетку собрали больше
                infobox.X = WIDTH + 1;
                infobox.Y = 1;
                SetConsoleCursorPosition(h, infobox);
                SetConsoleTextAttribute(h, Color::DARKYELLOW);
                cout << "COINS: ";
                SetConsoleTextAttribute(h, Color::YELLOW);
                cout << coins << "\n";
                maze[position.Y][position.X] = MazeObject::HALL; // убираем монетку из лабиринта


            }

            coinquantity = 0;

            for (int a = 0; a < HEIGHT; a++) // 
            {
                for (int b = 0; b < WIDTH; b++)
                {
                    if (maze[a][b] == MazeObject::COIN) coinquantity++;
                }
            }//
            if (coinquantity == 0) MessageBoxA(0, "you find all coins!", "WIN!", 0);// (я  добавила)


            if (maze[position.Y][position.X] == MazeObject::HEALTHBOX)//()
            {
                health = 100;
                infobox.X = WIDTH + 1;
                infobox.Y = 2;
                SetConsoleCursorPosition(h, infobox);
                SetConsoleTextAttribute(h, Color::DARKRED);
                cout << "HEALTH: ";
                SetConsoleTextAttribute(h, Color::RED);
                cout << health << " \n";
                maze[position.Y][position.X] = MazeObject::GRAVE;
            }//()
            // пересечение с врагами
            // если в лабиринте по позиции ГГ (под ним) находится враг
            if (maze[position.Y][position.X] == MazeObject::ENEMY)
            {
                health -= 20;
                infobox.X = WIDTH + 1;
                infobox.Y = 2;
                SetConsoleCursorPosition(h, infobox);
                SetConsoleTextAttribute(h, Color::DARKRED);
                cout << "HEALTH: ";
                SetConsoleTextAttribute(h, Color::RED);
                cout << health << " \n";
                maze[position.Y][position.X] = MazeObject::GRAVE; // убираем врага из лабиринта

                if (health <= 0)
                {
                    int answer = MessageBoxA(0, "health is over!\n\nwant to play again?", "FAIL!", MB_YESNO);
                    system("cls");
                    if (answer == IDYES)
                    {
                        main();
                    }
                    else
                    {
                        exit(0);
                    }
                }

            }


        }
        else
        {

            Sleep(15);
            ///////////////////////////////////////////////////////////////
            // движение врагов
            COORD enemy_positions[100]; // массив который хранит координаты врагов
            int enemy_count = 0; // реальное количество врагов на данный момент после шага ГГ

            // перебор всех ячеек которые есть в лабиринте
            for (int y = 0; y < HEIGHT; y++) // перебор строк
            {
                for (int x = 0; x < WIDTH; x++) // перебор столбцов
                {
                    // если очередная ячейка - это враг
                    if (maze[y][x] == MazeObject::ENEMY)
                    {
                        enemy_positions[enemy_count].X = x;
                        enemy_positions[enemy_count].Y = y;
                        enemy_count++;
                    }
                }
            }

            // перебор всех врагов
            for (int i = 0; i < enemy_count; i++)
            {
                int r = rand() % 100; // 0-left, 1-right, 2-up, 3-down
                if (r == LEFT &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALL &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::BORDER &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::ENEMY)
                {
                    // стирание врага в старой позиции
                    COORD temp = enemy_positions[i];
                    SetConsoleCursorPosition(h, temp);
                    cout << " ";
                    maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

                    // перемещаем врага в новую позицию
                    temp.X = enemy_positions[i].X - 1;
                    temp.Y = enemy_positions[i].Y;
                    SetConsoleCursorPosition(h, temp);
                    SetConsoleTextAttribute(h, Color::RED);
                    cout << (char)1;
                    maze[enemy_positions[i].Y][enemy_positions[i].X - 1] = MazeObject::ENEMY;
                }
                else if (r == RIGHT &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALL &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::BORDER &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::ENEMY)
                {
                    // стирание врага в старой позиции
                    COORD temp = enemy_positions[i];
                    SetConsoleCursorPosition(h, temp);
                    cout << " ";
                    maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

                    // перемещаем врага в новую позицию
                    temp.X = enemy_positions[i].X + 1;
                    temp.Y = enemy_positions[i].Y;
                    SetConsoleCursorPosition(h, temp);
                    SetConsoleTextAttribute(h, Color::RED);
                    cout << (char)1;
                    maze[enemy_positions[i].Y][enemy_positions[i].X + 1] = MazeObject::ENEMY;
                }


                else if (r == UP &&
                    maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALL &&
                    maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::BORDER &&
                    maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::ENEMY)
                {
                    // стирание врага в старой позиции
                    COORD temp = enemy_positions[i];
                    SetConsoleCursorPosition(h, temp);
                    cout << " ";
                    maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

                    // перемещаем врага в новую позицию
                    temp.X = enemy_positions[i].X;
                    temp.Y = enemy_positions[i].Y - 1;
                    SetConsoleCursorPosition(h, temp);
                    SetConsoleTextAttribute(h, Color::RED);
                    cout << (char)1;
                    maze[enemy_positions[i].Y - 1][enemy_positions[i].X] = MazeObject::ENEMY;
                }
                else if (r == DOWN &&
                    maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALL &&
                    maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::BORDER &&
                    maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::ENEMY)
                {
                    // стирание врага в старой позиции
                    COORD temp = enemy_positions[i];
                    SetConsoleCursorPosition(h, temp);
                    cout << " ";
                    maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

                    // перемещаем врага в новую позицию
                    temp.X = enemy_positions[i].X;
                    temp.Y = enemy_positions[i].Y + 1;
                    SetConsoleCursorPosition(h, temp);
                    SetConsoleTextAttribute(h, Color::RED);
                    cout << (char)1;
                    maze[enemy_positions[i].Y + 1][enemy_positions[i].X] = MazeObject::ENEMY;
                }

            }

        }

    }

}
void FillMaze(int maze[][40], int H, int W)
{
    
    srand(time(0));
    //int maze[HEIGHT][WIDTH] = {}; // maze - лабиринт по-английски
    // алгоритм заполнения массива
    for (int y = 0; y < H; y++) // перебор строк
    {
        for (int x = 0; x < W; x++) // перебор столбцов
        {
            maze[y][x] = rand() % 4; // 4 типа объектов в игре

            if (maze[y][x] == MazeObject::ENEMY) // если в лабиринте сгенерился враг
            {
                int probability = rand() % 15; // 0...14, если выпало 0 - враг останется, останется только одна пятая часть врагов
                if (probability != 0) // убираем врага
                {
                    maze[y][x] = MazeObject::HALL; // на место врага ставим коридор
                }
            }

            if (maze[y][x] == MazeObject::WALL) // если в лабиринте сгенерировалась стена
            {
                int probability = rand() % 2; // 0...1, если выпало 0 - стена останется, останется только половина стен
                if (probability != 0) // убираем стену
                {
                    maze[y][x] = MazeObject::HALL; // на место стены ставим коридор
                }
            }

            if (x == 0 || y == 0 || x == W - 1 || y == H - 1)
                maze[y][x] = MazeObject::BORDER; // белая рамка

            if (x == 0 && y == 2 || x == 1 && y == 2 || x == 2 && y == 2)
                maze[y][x] = MazeObject::HALL; // вход

            if (x == W - 1 && y == H - 3 ||
                x == W - 2 && y == H - 3 ||
                x == W - 3 && y == H - 3)
                maze[y][x] = MazeObject::HALL; // выход
            if (x == 5 && y == 5)//()
                maze[y][x] = MazeObject::HEALTHBOX;//()
        }
    }
}
void ShowMaze(int maze[][40], int H, int W)
{
    
    for (int y = 0; y < H; y++) // перебор строк
    {
        for (int x = 0; x < W; x++) // перебор столбцов
        {
            switch (maze[y][x])
            {
            case MazeObject::HALL: // hall - коридор
                SetConsoleTextAttribute(h, Color::BLACK);
                cout << " ";
                break;

            case MazeObject::WALL: // wall - стена
                SetConsoleTextAttribute(h, Color::DARKGREEN);
                cout << (char)178;
                break;

            case MazeObject::BORDER: // border - рамка
                SetConsoleTextAttribute(h, Color::WHITE);
                cout << (char)178;
                break;

            case MazeObject::COIN: // coin - монетка
                SetConsoleTextAttribute(h, Color::YELLOW);
                cout << ".";
                break;


            case MazeObject::ENEMY: // enemy - враг
                SetConsoleTextAttribute(h, Color::RED);
                cout << (char)1;
                break;

            case MazeObject::HEALTHBOX: // ()
                SetConsoleTextAttribute(h, 15 * 16 + 12);
                cout << "+";
                break;//()
            }
        }
        cout << "\n";
    }
}
void VisualSetting()
{
    system("color 07");
    // настройки шрифта консоли
    CONSOLE_FONT_INFOEX font; // https://docs.microsoft.com/en-us/windows/console/console-font-infoex
    font.cbSize = sizeof(font);
    font.dwFontSize.Y = 50;
    font.FontFamily = FF_DONTCARE;
    font.FontWeight = FW_NORMAL;
    wcscpy_s(font.FaceName, 9, L"Consolas");
    SetCurrentConsoleFontEx(h, 0, &font);

    // скрытие мигающего курсора 
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = false; // спрятать курсор
    cursor.dwSize = 1; // 1...100
    SetConsoleCursorInfo(h, &cursor);

    system("title Maze");
    MoveWindow(GetConsoleWindow(), 20, 60, 1850, 900, true);
    // 20 - отступ слева от левой границы рабочего стола до левой границы окна консоли (в пикселях!)
    // 60 - отступ сверху от верхней границы рабочего стола до верхней границы окна консоли
    // 1850 - ширина окна консоли в пикселях
    // 900 - высота окна консоли
    // true - перерисовать окно после перемещения
}
void ShowHelth(int wf, int healthf)
{
    COORD infobox;
    infobox.X = wf + 1;
    infobox.Y = 2;
    SetConsoleCursorPosition(h, infobox);
    SetConsoleTextAttribute(h, Color::DARKRED);
    cout << "HEALTH: ";
    SetConsoleTextAttribute(h, Color::RED);
    cout << healthf << "\n"; // 100
}
void ShowSteps(int wf, int stepsf)
{
    COORD infobox;
    infobox.X = wf + 1;
    infobox.Y = 3;
    SetConsoleCursorPosition(h, infobox);
    SetConsoleTextAttribute(h, Color::DARKYELLOW);
    cout << "STEPS: ";
    SetConsoleTextAttribute(h, Color::YELLOW);
    cout << stepsf << "\n";
}
void ShowCoins(int wf, int coinsf)
{
    COORD infobox;
    infobox.X = wf + 1;
    infobox.Y = 1;
    SetConsoleCursorPosition(h, infobox);
    SetConsoleTextAttribute(h, Color::DARKYELLOW);
    cout << "COINS: ";
    SetConsoleTextAttribute(h, Color::YELLOW);
    cout << coinsf << "\n"; // 0
}
void ShowEnergy(int wf,int energyf)
{
    COORD infobox;
    infobox.X = wf + 1;
    infobox.Y = 4;
    SetConsoleCursorPosition(h, infobox);
    SetConsoleTextAttribute(h, Color::DARKRED);
    cout << "ENERGY: ";
    SetConsoleTextAttribute(h, Color::RED);
    cout << energyf << " \n";
}
void GenPosition(int X1, int Y1)
{
    COORD position1;
    position1.X = X1;
    position1.Y = Y1;
    SetConsoleCursorPosition(h, position1);
    SetConsoleTextAttribute(h, Color::BLUE);
    cout << (char)2;
}
