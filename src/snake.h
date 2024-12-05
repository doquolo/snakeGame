#include <vector>
#include <math.h>
using namespace std;

// vector to store parts of a snake body
vector<coord> snake;

// storing current direction the snake is heading to
// 0: start
// +1: right
// -1: left
// -2: up
// +2: down
int currentHeading = 0;
int avoidFirstHeading = -1;

// storing points
int score = 0;

// reset all game's variables
void resetAll() {
    snake.clear();
    int currentHeading = 0;
    int avoidFirstHeading = -1;
    int score = 0;
}

// used when snake extends its length (by consuming foods on map)
// the food position will be the new head of the snake
void addNode(coord point) {
    snake.push_back(point);
}

// used to move the snake 
// a new head would be processed by another part of the code, then return to this function to be the new head of the snake.
// Meanwhile the tail (index 0 in snake vector) would be cut off
// if keepTail is true then the tail would be kept, and increase the score by one
void moveSnake(bool keepTail = false) {
    if (currentHeading != 0) {
        coord newHead = snake.back();
        switch (currentHeading) {
            case +1:
                newHead.x++;
                break;
            case -1:
                newHead.x--;
                break;
            case +2:
                newHead.y++;
                break;
            case -2:
                newHead.y--;
                break;
            default:
                break;
        }
        snake.push_back(newHead);
        if (!keepTail) snake.erase(snake.begin());  
        else score++;
    }
}

// process input
// snake cannot head back into its body, so the condition to changing direction is only when the snake go left/right depending on the current heading.
void processMovement(int direction) {
    // Because we got no way to know where the snake would be heading at the start of the game
    // it might be possbile that the snake would be going backward, colliding with itself
    // therefore a hardcoded limit has been set to prevent that
    if (direction == avoidFirstHeading && currentHeading == 0) return;
    if (abs(direction) != abs(currentHeading)) {
        currentHeading = direction;
    }
}

// used when user want to check for self colision, that is when the snake hits itself
// it first cuts of the head and store in a temp variable, then a for loop is used to check whether the head collide with any parts of the snake body.
// return true if the snake hits itself, otherwise false
bool checkSelfColision(vector<coord> snake) {
    vector<coord> temp = snake;
    coord head = {temp.back().x, temp.back().y};
    temp.pop_back();
    for (auto x : temp) {
        if (x.x == head.x && x.y == head.y) return true;
    }
    return false;
}

// used when the user want to check for if the snake hits the wall
// return true if the snake hits the wall, otherwise false
bool checkWallColision(int w, int h, vector<coord> snake) {
    coord head = snake.back();
    if (head.x >= (h-2) || head.x <= -2 || head.y >= (w-2) || head.y <= -2) return true;
    return false;
}


