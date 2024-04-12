#include <iostream>
#include <conio.h> // console I/O - kbhit()
#include <windows.h> // Sleep() function

using namespace std;

// Variables
bool game_over = false;
int score = 0;
int tail_number = 0;

// Coordinates

// Snake
int snake_x, snake_y;
int snake_tail_x[100], snake_tail_y[100];
// Fuit
int fruit_x, fruit_y;
// Console
const int width = 50;
const int height = 25;

enum DIRECTION
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP = 0
};

DIRECTION currentDirection;

void initializeGame()
{
    currentDirection = STOP;

}

void draw()
{
    // Top border
    for (int i = 0; i < width + 2; i++)
        cout<<"#";
    cout<<endl;

    for (int i = 0; i < height; i++)
    {
        // Left border
        cout<<"#";

        for (int j = 0; j < width; j++)
        {
            // Drawing snake, fruit and empty spaces
            if (i == snake_y && j == snake_x)
                cout<<"O"; // Snake head
            else if (i == fruit_y && j == fruit_x)
                cout<<"X"; // Fruit
            else
            {
                bool print_tail = false;

                // Checking for snake tail segments
                for (int k = 0; k < tail_number; k++)
                {
                    if (snake_tail_x[k] ==j && snake_tail_y[k] == i)
                    {
                        cout<<"o"; // Snake tail
                        print_tail = true;
                        break; // Exit loop once snake taile segment is found
                    }
                }
                if (!print_tail)
                    cout<<" "; // Empty space
            }
            // Right border
            if (j == width - 1)
                cout<<"#";
        }
        cout<<endl;
    }
    // Bottom border
    for (int i = 0; i< width + 2; i++)
        cout<<"#";
    cout<<endl;

    cout<<"Score: "<<score<<endl;
}

void userInput()
{
    if(kbhit()) // Check if a key has been pressed on the keyboard without needing to continuously hold down the directional buttons
    {
        switch(getch()) // Get character
        {
            case 'a':
                currentDirection = LEFT;
                break;
            case 'd':
                currentDirection = RIGHT;
                break;
            case 'w':
                currentDirection = UP;
                break;
            case 's':
                currentDirection = DOWN;
                break;
            case 'k':
                game_over = true;
                break;
        }
    }
}

void snakeLogic()
{

}



int main() {

    initializeGame();

    while (!game_over)
    {
        draw();
        userInput();
        snakeLogic();
        Sleep(10); // control the speed of the game - milliseconds
    }
    return 0;
}