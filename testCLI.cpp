#include <windows.h>

#include <thread>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;
int speed = 125;

// struct to store x,y coordinate of a single point on screen 
// this is here to be used in other dependencies
struct coord {
    int x, y;
};

const int w = 100, h = 30;

#include "snake.h"
#include "food.h"
#include "cli.h"

// create hook for keyboard event
HHOOK hHook;

// callback for keyboard event
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN) {
            int keyboardCode = (int) pKeyboard->vkCode;
            switch (keyboardCode) {
                case 87:
                    processMovement(-1);
                    break;
                case 83:
                    processMovement(+1);
                    break;
                case 65:
                    processMovement(-2);
                    break;
                case 68:
                    processMovement(+2);
                    break;
                default:
                    break;
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// main loop
int main() {

    // create clock
    Clock::time_point prevTime = Clock::now();

    // add starting node
    for (int i = 0; i < 3; i++) {
        coord snakeStart = {3, 3+i}; 
        addNode(snakeStart);
    }
    for (int i = 0; i < 3; i++) {
        createRandomFood(1, 1, h-2, w-2, snake);
    }


    // creating instances for attaching hook
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);

    if (hHook == nullptr) {
        std::cerr << "Failed to install hook!" << std::endl;
        return 1;
    }

    MSG msg;
    while (true) {
        // Process Windows messages
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // update game
        if (std::chrono::duration_cast<milliseconds>(Clock::now() - prevTime) >= milliseconds((int)speed)) {
            prevTime = Clock::now();
            moveSnake();
            if (checkWallColision(w, h, snake) || checkSelfColision(snake)) {
                break;
            };
            if (checkFoodColision(snake)) {
                speed *= .98;
            };
            drawPlayArea(w, h, snake, foodList);
        }
    }

    // system("cls");
    std::cout << "Game over. Score: " << score << endl;
 
    UnhookWindowsHookEx(hHook);
    return 0;
}
