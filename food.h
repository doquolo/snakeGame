#include <vector>
#include <random>

using namespace std;

vector<coord> foodList;

void createRandomFood(int xmin, int ymin, int xmax, int ymax, vector<coord> snake) {
    coord newFood = {rand() % xmax, rand() % ymax};
    for (auto x : snake) {
        if (x.x == newFood.x && x.y == newFood.y) {
            newFood = {rand() % xmax, rand() % ymax};
        }
    }
    foodList.push_back(newFood);
}

bool checkFoodColision(vector<coord> snake) {
    coord head = {snake.back().x, snake.back().y};
    for (int i = 0; i < foodList.size(); i++) {
        if (head.x == foodList[i].x && head.y == foodList[i].y) {
            foodList.erase(foodList.begin() + i);
            moveSnake(true);
            createRandomFood(1, 1, h-2, w-2, snake);
            return true;
        }
    }
    return false;
}