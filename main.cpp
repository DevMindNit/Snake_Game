#include <iostream>
#include <ncurses.h> // ncurses library
#include <cstdlib>    // for rand() and srand()
#include <ctime>      // for time()
#include <thread>     // for std::this_thread::sleep_for
#include <chrono>     // for std::chrono
#include <vector>     // for std::vector

using namespace std;

// Variables
bool game_over = false;
int score = 0;
int tail_number = 0;

// Coordinates

// Snake
int snake_x, snake_y;
vector<int> snake_tail_x;  // Use vector to store snake tail positions
vector<int> snake_tail_y;
// Fruit
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
    snake_x = width / 2;
    snake_y = height / 2;
    fruit_x = rand() % width;
    fruit_y = rand() % height;

    // Initialize ncurses
    initscr();             // Start ncurses mode
    timeout(100);          // Set non-blocking input (timeout 100 ms)
    cbreak();              // Disable line buffering (get characters immediately)
    noecho();              // Don't display input characters
    curs_set(0);           // Hide cursor
    srand(time(nullptr));  // Seed random number generator
}

void draw()
{
    clear(); // Clear the screen for the new frame

    // Draw the top border
    for (int i = 0; i < width + 2; i++)
        mvaddch(0, i, '#');
    
    for (int i = 0; i < height; i++)
    {
        mvaddch(i + 1, 0, '#'); // Left border

        for (int j = 0; j < width; j++)
        {
            if (i == snake_y && j == snake_x)
                mvaddch(i + 1, j + 1, 'O'); // Snake head
            else if (i == fruit_y && j == fruit_x)
                mvaddch(i + 1, j + 1, 'X'); // Fruit
            else
            {
                bool print_tail = false;
                for (int k = 0; k < tail_number; k++)
                {
                    if (snake_tail_x[k] == j && snake_tail_y[k] == i)
                    {
                        mvaddch(i + 1, j + 1, 'o'); // Snake tail
                        print_tail = true;
                        break;
                    }
                }
                if (!print_tail)
                    mvaddch(i + 1, j + 1, ' '); // Empty space
            }
        }

        mvaddch(i + 1, width + 1, '#'); // Right border
    }

    // Draw the bottom border
    for (int i = 0; i < width + 2; i++)
        mvaddch(height + 1, i, '#');

    // Draw score
    mvprintw(height + 2, 0, "Score: %d", score);

    refresh(); // Update the screen
}

void userInput()
{
    int ch = getch(); // Get user input
    switch (ch)
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
    case 'k': // Press 'k' to end the game
        game_over = true;
        break;
    }
}

void snakeLogic()
{
    int prevX = snake_x;
    int prevY = snake_y;
    int prev2X, prev2Y;
    int tailX, tailY;

    // Move the tail
    for (int i = 0; i < tail_number; i++)
    {
        prev2X = snake_tail_x[i];
        prev2Y = snake_tail_y[i];
        snake_tail_x[i] = prevX;
        snake_tail_y[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move the snake head
    switch (currentDirection)
    {
    case LEFT:
        snake_x--;
        break;
    case RIGHT:
        snake_x++;
        break;
    case UP:
        snake_y--;
        break;
    case DOWN:
        snake_y++;
        break;
    default:
        break;
    }

    // If the snake eats the fruit
    if (snake_x == fruit_x && snake_y == fruit_y)
    {
        score += 10;
        fruit_x = rand() % width;
        fruit_y = rand() % height;

        // Add a new segment to the snake tail
        snake_tail_x.push_back(prevX);
        snake_tail_y.push_back(prevY);
        tail_number++; // Grow the snake
    }

    // If the snake hits the boundaries or itself
    if (snake_x >= width || snake_x < 0 || snake_y >= height || snake_y < 0)
        game_over = true;

    for (int i = 0; i < tail_number; i++)
    {
        if (snake_tail_x[i] == snake_x && snake_tail_y[i] == snake_y)
        {
            game_over = true;
        }
    }
}

int main()
{
    initializeGame();

    while (!game_over)
    {
        draw();
        userInput();
        snakeLogic();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // control the speed of the game
    }

    // End the game
    mvprintw(height + 3, 0, "Game Over! Final Score: %d", score);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Show final score for 3 seconds

    endwin(); // End ncurses mode
    return 0;
}
