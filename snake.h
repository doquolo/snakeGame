#include <vector>
#include <math.h>
#include <iostream>
using namespace std;

// struct to store x,y coordinate of a single point on screen 
struct coord {
    int x, y;
};

// vector to store parts of a snake body
vector<coord> snake;

// storing current direction the snake is heading to
// 0: start
// +1: up
// -1: down
// +2: left
// -2: right
int currentHeading = 0;

// storing points
int score = -1;

// used when snake extends its length (by consuming foods on map)
// the food position will be the new head of the snake
void addNode(coord point) {
    snake.push_back(point);
    score++;
}

// used to move the snake 
// a new head would be processed by another part of the code, then return to this function to be the new head of the snake.
// Meanwhile the tail (index 0 in snake vector) would be cut off
void moveSnake() {
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
        snake.erase(snake.begin());  
    }
}

// process input
// snake cannot head back into its body, so the condition to changing direction is only when the snake go left/right depending on the current heading.
void processMovement(int direction) {
    if (abs(direction) != abs(currentHeading)) {
        currentHeading = direction;
    }
}

// used when user want to check for self colision, that is when the snake hits itself
// it first cuts of the head and store in a temp variable, then a for loop is used to check whether the head collide with any parts of the snake body.
// return true if the snake hits itself, otherwise false
// TODO: some weird shits happened here
bool checkSelfColision(vector<coord> snake) {
    vector<coord> temp;
    coord head = temp.back();
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
    cout << head.x << " " << head.y << endl; 
    if (head.x >= (h-1) || head.x <= 0 || head.y >= (w-1) || head.y <= 0) return true;
    return false;
}



