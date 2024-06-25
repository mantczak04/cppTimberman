#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <random>
#include <utility>

#define BLACK 0x0000 << 4
#define BLUE 0x0001 << 4
#define GREEN 0x0002 << 4
#define CYAN 0x0003 << 4
#define RED 0x0004 << 4
#define MAGENTA 0x0005 << 4
#define BROWN 0x0006 << 4
#define LIGHT_GRAY 0x0007 << 4
#define DARK_GRAY 0x0008 << 4
#define LIGHT_BLUE 0x0009 << 4
#define LIGHT_GREEN 0x000A << 4
#define LIGHT_CYAN 0x000B << 4
#define LIGHT_RED 0x000C << 4
#define LIGHT_MAGENTA 0x000D << 4
#define YELLOW 0x000E << 4
#define WHITE 0x000F << 4

const int nTreeWidth = 4;
const int nScreenWidth = 80;
const int nScreenHeight = 40;
const int nLevelIncrementation = 11;

class Screen
{
    wchar_t* screen;
    HANDLE hConsole;
    DWORD bytesWritten;
    const int width;
    const int height;

public:
    Screen(int width, int height) : width(width), height(height)
    {
        screen = new wchar_t[width * height];
        hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        bytesWritten = 0;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                setPixel(j, i, LIGHT_BLUE);
            }
        }

        for (int i = 38; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                setPixel(j, i, GREEN);
            }
        }

        for (int i = 0; i < height - 1; i++)
        {
            for (int j = (width / 2) - nTreeWidth; j < (width / 2) + nTreeWidth; j++)
            {
                setPixel(j, i, BROWN);
            }
        }
    }

    ~Screen()
    {
        delete[] screen;
        CloseHandle(hConsole);
    }

    void displayScreen()
    {
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &bytesWritten);
    }

    void clearScreen()
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                setPixel(j, i, LIGHT_BLUE);
            }
        }

        for (int i = 38; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                setPixel(j, i, GREEN);
            }
        }

        for (int i = 0; i < height - 1; i++)
        {
            for (int j = (width / 2) - nTreeWidth; j < (width / 2) + nTreeWidth; j++)
            {
                setPixel(j, i, BROWN);
            }
        }
    }

    void setScreenActive()
    {
        SetConsoleActiveScreenBuffer(hConsole);
    }

    void setPixel(int x, int y, WORD color, wchar_t character)
    {
        screen[y * nScreenWidth + x] = character;
        COORD pos = { (SHORT)x, (SHORT)y };
        FillConsoleOutputAttribute(hConsole, color, 1, pos, &bytesWritten);
    }

    void setPixel(int x, int y, WORD color)
    {
        screen[y * nScreenWidth + x] = ' ';
        COORD pos = { (SHORT)x, (SHORT)y };
        FillConsoleOutputAttribute(hConsole, color, 1, pos, &bytesWritten);
    }
};

void displayPlayer(Screen & screen, bool isPlayerOnLeft, int trigger)
{
    int startPos = nScreenWidth/2;
    isPlayerOnLeft ? startPos -= 13+trigger : startPos += 12-trigger;

    //shoes
    screen.setPixel(startPos-1, 38, BLACK);
    screen.setPixel(startPos-2, 38, BLACK);
    screen.setPixel(startPos-3, 38, BLACK);

    screen.setPixel(startPos+1, 38, BLACK);
    screen.setPixel(startPos + 2, 38, BLACK);
    screen.setPixel(startPos + 3, 38, BLACK);


    //legs
    screen.setPixel(startPos - 1, 37, BLUE);
    screen.setPixel(startPos - 1, 36, BLUE);

    screen.setPixel(startPos + 1, 37, BLUE);
    screen.setPixel(startPos + 1, 36, BLUE);


    //torso
    screen.setPixel(startPos - 2, 35, RED);
    screen.setPixel(startPos - 1, 35, RED);
    screen.setPixel(startPos, 35, RED);
    screen.setPixel(startPos + 1, 35, RED);
    screen.setPixel(startPos + 2, 35, RED);

    screen.setPixel(startPos - 2, 34, RED);
    screen.setPixel(startPos - 1, 34, RED);
    screen.setPixel(startPos, 34, RED);
    screen.setPixel(startPos + 1, 34, RED);
    screen.setPixel(startPos + 2, 34, RED);

    screen.setPixel(startPos - 2, 33, RED);
    screen.setPixel(startPos - 1, 33, RED);
    screen.setPixel(startPos, 33, RED);
    screen.setPixel(startPos + 1, 33, RED);
    screen.setPixel(startPos + 2, 33, RED);

    //hands
    screen.setPixel(startPos + 3, 33, RED);
    screen.setPixel(startPos + 4, 33, RED);
    screen.setPixel(startPos + 5, 33, YELLOW);

    screen.setPixel(startPos - 3, 33, RED);
    screen.setPixel(startPos - 4, 33, RED);
    screen.setPixel(startPos - 5, 33, YELLOW);

    //head
    screen.setPixel(startPos -1, 32, YELLOW);
    screen.setPixel(startPos, 32, YELLOW | BLACK>>4, '-');
    screen.setPixel(startPos + 1, 32, YELLOW);

    screen.setPixel(startPos - 1, 31, YELLOW | BLACK>>4, 'o');
    screen.setPixel(startPos, 31, YELLOW);
    screen.setPixel(startPos + 1, 31, YELLOW | BLACK >> 4, 'o');

    //hair
    screen.setPixel(startPos - 1, 30, LIGHT_BLUE | BLACK>>4, '.');
    screen.setPixel(startPos, 30, LIGHT_BLUE | BLACK >> 4, '.');
    screen.setPixel(startPos + 1, 30, LIGHT_BLUE | BLACK>>4, '.');

    //axe
    if (isPlayerOnLeft)
    {
        screen.setPixel(startPos + 5, 31, BROWN);
        screen.setPixel(startPos + 5, 34, BROWN);
        screen.setPixel(startPos + 5, 32, BROWN);
        screen.setPixel(startPos + 5, 30, RED);
        screen.setPixel(startPos + 6, 30, RED);
        screen.setPixel(startPos + 7, 30, WHITE);
    }
    else
    {
        screen.setPixel(startPos - 5, 31, BROWN);
        screen.setPixel(startPos - 5, 34, BROWN);
        screen.setPixel(startPos - 5, 32, BROWN);
        screen.setPixel(startPos - 5, 30, RED);
        screen.setPixel(startPos - 6, 30, RED);
        screen.setPixel(startPos - 7, 30, WHITE);
    }
}

void clearPlayer(Screen& screen, bool isPlayerOnLeft, int trigger)
{
    int startPos = nScreenWidth / 2;
    isPlayerOnLeft ? startPos -= 13 + trigger : startPos += 12 - trigger;

    //shoes
    screen.setPixel(startPos - 1, 38, GREEN);
    screen.setPixel(startPos - 2, 38, GREEN);
    screen.setPixel(startPos - 3, 38, GREEN);

    screen.setPixel(startPos + 1, 38, GREEN);
    screen.setPixel(startPos + 2, 38, GREEN);
    screen.setPixel(startPos + 3, 38, GREEN);


    //legs
    screen.setPixel(startPos - 1, 37, LIGHT_BLUE);
    screen.setPixel(startPos - 1, 36, LIGHT_BLUE);

    screen.setPixel(startPos + 1, 37, LIGHT_BLUE);
    screen.setPixel(startPos + 1, 36, LIGHT_BLUE);


    //torso
    screen.setPixel(startPos - 2, 35, LIGHT_BLUE);
    screen.setPixel(startPos - 1, 35, LIGHT_BLUE);
    screen.setPixel(startPos, 35, LIGHT_BLUE);
    screen.setPixel(startPos + 1, 35, LIGHT_BLUE);
    screen.setPixel(startPos + 2, 35, LIGHT_BLUE);

    screen.setPixel(startPos - 2, 34, LIGHT_BLUE);
    screen.setPixel(startPos - 1, 34, LIGHT_BLUE);
    screen.setPixel(startPos, 34, LIGHT_BLUE);
    screen.setPixel(startPos + 1, 34, LIGHT_BLUE);
    screen.setPixel(startPos + 2, 34, LIGHT_BLUE);

    screen.setPixel(startPos - 2, 33, LIGHT_BLUE);
    screen.setPixel(startPos - 1, 33, LIGHT_BLUE);
    screen.setPixel(startPos, 33, LIGHT_BLUE);
    screen.setPixel(startPos + 1, 33, LIGHT_BLUE);
    screen.setPixel(startPos + 2, 33, LIGHT_BLUE);

    //hands
    screen.setPixel(startPos + 3, 33, LIGHT_BLUE);
    screen.setPixel(startPos + 4, 33, LIGHT_BLUE);
    screen.setPixel(startPos + 5, 33, LIGHT_BLUE);

    screen.setPixel(startPos - 3, 33, LIGHT_BLUE);
    screen.setPixel(startPos - 4, 33, LIGHT_BLUE);
    screen.setPixel(startPos - 5, 33, LIGHT_BLUE);

    //head
    screen.setPixel(startPos - 1, 32, LIGHT_BLUE);
    screen.setPixel(startPos, 32, LIGHT_BLUE);
    screen.setPixel(startPos + 1, 32, LIGHT_BLUE);

    screen.setPixel(startPos - 1, 31, LIGHT_BLUE);
    screen.setPixel(startPos, 31, LIGHT_BLUE);
    screen.setPixel(startPos + 1, 31, LIGHT_BLUE);

    //hair
    screen.setPixel(startPos - 1, 30, LIGHT_BLUE);
    screen.setPixel(startPos, 30, LIGHT_BLUE);
    screen.setPixel(startPos + 1, 30, LIGHT_BLUE);

    //axe
    if (isPlayerOnLeft)
    {
        screen.setPixel(startPos + 5, 31, LIGHT_BLUE);
        screen.setPixel(startPos + 5, 34, LIGHT_BLUE);
        screen.setPixel(startPos + 5, 32, LIGHT_BLUE);
        screen.setPixel(startPos + 5, 30, LIGHT_BLUE);
        screen.setPixel(startPos + 6, 30, LIGHT_BLUE);
        screen.setPixel(startPos + 7, 30, LIGHT_BLUE);
    }
    else
    {
        screen.setPixel(startPos - 5, 31, LIGHT_BLUE);
        screen.setPixel(startPos - 5, 34, LIGHT_BLUE);
        screen.setPixel(startPos - 5, 32, LIGHT_BLUE);
        screen.setPixel(startPos - 5, 30, LIGHT_BLUE);
        screen.setPixel(startPos - 6, 30, LIGHT_BLUE);
        screen.setPixel(startPos - 7, 30, LIGHT_BLUE);
    }
}

void displayBranches(Screen& screen, std::vector<std::pair<int,int>> branches)
{
    int startPos = 0;
    for (const auto& branch : branches)
    {
        if (branch.first == 0) continue;
        else if (branch.first == 1)  startPos = nScreenWidth/2 + nTreeWidth/2 + 2; // put the branch on right if its right
        else if (branch.first == -1) startPos = nScreenWidth/2 - nTreeWidth/2 - 12;
        

        for (int i = 0; i < 10; i++) // width of the branch
            screen.setPixel(startPos+i, branch.second, BROWN);
    }
}

void clearBranches(Screen& screen, std::vector<std::pair<int, int>> branches)
{
    int startPos = 0;
    for (const auto& branch : branches)
    {
        if (branch.first == 0) continue;
        else if (branch.first == 1)  startPos = nScreenWidth / 2 + nTreeWidth / 2 + 2; // put the branch on right if its right
        else if (branch.first == -1) startPos = nScreenWidth / 2 - nTreeWidth / 2 - 12;


        for (int i = 0; i < 10; i++) // width of the branch
            screen.setPixel(startPos + i, branch.second, LIGHT_BLUE);
    }
}

int main() {

    Screen screen(nScreenWidth, nScreenHeight); // 80x40 pixels screen;
    screen.setScreenActive();

    bool isPlayerOnLeft = false;

    bool decreaseBranches = false;

    bool switcher = true;

    bool isDKeyPressed = false;
    bool isAKeyPressed = false;

    std::pair<int, int> pBranch; // -1 - branch on left | 0 - no branch | 1 - branch on right, int - yLevel of the branch
    std::vector<std::pair<int, int>> vBranches;

    std::random_device rd; // for random numbers
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100);


    while (1) {
        int randomNumber = distrib(gen);
        decreaseBranches = false;


        if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
            if (!isDKeyPressed) {
                clearPlayer(screen, isPlayerOnLeft, 0);
                clearBranches(screen, vBranches);
                
                if(switcher)
                randomNumber % 2 == 0 ? vBranches.push_back(std::make_pair(1, -nLevelIncrementation)) : vBranches.push_back(std::make_pair(-1, -nLevelIncrementation));
                else 
                vBranches.push_back(std::make_pair(0, -nLevelIncrementation));

                switcher = !switcher;
                decreaseBranches = true;
                isPlayerOnLeft = false;
                isDKeyPressed = true;

            }
        }
        else {
            isDKeyPressed = false;
        }

        if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
            if (!isAKeyPressed) {
                clearPlayer(screen, isPlayerOnLeft, 0);
                clearBranches(screen, vBranches);
                
                if (switcher)
                    randomNumber % 2 == 0 ? vBranches.push_back(std::make_pair(1, -nLevelIncrementation)) : vBranches.push_back(std::make_pair(-1, -nLevelIncrementation));
                else
                    vBranches.push_back(std::make_pair(0, -nLevelIncrementation));

                switcher = !switcher;

                decreaseBranches = true;
                isPlayerOnLeft = true;
                isAKeyPressed = true;
                
            }
        }
        else {
            isAKeyPressed = false;
        }

        if (decreaseBranches)
        {
            for (auto& branch : vBranches) {
                branch.second+=nLevelIncrementation;

                if (branch.first == -1 && branch.second > 30 && isPlayerOnLeft) return 0;
                else if (branch.first == 1 && branch.second > 30 && !isPlayerOnLeft) return 0;
            }

            vBranches.erase(
                remove_if(vBranches.begin(), vBranches.end(), [](const std::pair<int, int>& branch) {
                    return branch.second > 39;
                    }),
                vBranches.end()
            );

            decreaseBranches = false;
        }

            //if (branch.second >= nScreenHeight&& branch.first == -1 && !isPlayerOnLeft) // if its a last branch, branch is left and player is on the right




        displayBranches(screen, vBranches);
        displayPlayer(screen, isPlayerOnLeft, 0);
        screen.displayScreen();
    }

    return 0;
}