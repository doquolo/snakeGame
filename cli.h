#include <bits/stdc++.h>

using namespace std;

void drawPlayArea(int w, int h, vector<coord> snake, vector<coord> foodList) {
    system("cls");
    string playArea = "";
    // draw upper border
    for (int i = 0; i < w; i++) {
        playArea += "x";
    }
    playArea += '\n';
    // draw center
    for (int i = 1; i < h-2; i++) {
        playArea += "x";
        for (int j = 1; j < w-1; j++) {
            string temp = " ";
            for (auto part: snake) {
                if (part.x == i && part.y == j) {
                    if (part.x == snake.back().x && part.y == snake.back().y) temp = "O";
                    else temp = "o";
                };
            }
            for (auto part: foodList) {
                if (part.x == i && part.y == j) {
                    temp = "0";
                };
            }
            playArea += temp;
        }
        playArea += "x";
        playArea += '\n';   
    }
    for (int i = 0; i < w; i++) {
        playArea += "x";
    }
    playArea += '\n';
    cout << playArea;
}

