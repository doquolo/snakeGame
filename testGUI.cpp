#include <windows.h>
#include <strsafe.h>
#include <string>
#include <iostream>

struct coord {
    int x, y;
};

const int borderThickness = 10;
const int diag = 10;
coord window = {450, 600};
coord gameCoord = {window.x / diag, window.y / diag};

int speed = 125;

#include "snake.h"
#include "food.h"
#include "cli.h"


// brushes
HBRUSH greenBrush = CreateSolidBrush(RGB(52, 235, 52));
HBRUSH redBrush = CreateSolidBrush(RGB(235, 52, 52));
HBRUSH orangeBrush = CreateSolidBrush(RGB(235, 159, 52));

void drawParts(HDC hdc, int x, int y, int selector) {
    RECT snakePart = {x, y, x+diag, y+diag};
    HBRUSH hb;
    switch (selector) {
        case 0: // snake body
        {
            FillRect(hdc, &snakePart, greenBrush);
            break;
        }
        case 1: // snake head
        {
            FillRect(hdc, &snakePart, redBrush);
            break;
        }
        case 2: // snake head
        {
            FillRect(hdc, &snakePart, orangeBrush);
            break;
        }
        default: break;
    }
}

// Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // draw frame
            RECT outerFrame = {0, 0, window.y, window.x};
            RECT innerFrame = {0+borderThickness, 0+borderThickness, window.y-borderThickness, window.x-borderThickness};
            FillRect(hdc, &outerFrame, (HBRUSH)GetStockObject(BLACK_BRUSH));
            FillRect(hdc, &innerFrame, (HBRUSH)GetStockObject(WHITE_BRUSH));

            // Process game logic
            moveSnake();
            if (checkWallColision(gameCoord.x, gameCoord.y, snake) || checkSelfColision(snake)) {
                DestroyWindow(hwnd);
            };
            if (checkFoodColision(gameCoord.x, gameCoord.y, snake)) {
                speed *= .98;
                // reset timer
                SetTimer(hwnd, 1, speed, NULL); // Timer ID = 1
            };
            // Draw snake
            // for debugging
            // coord temp = snake.back();
            // std::cout << temp.x << "-" << temp.y << std::endl;
            for (int i = 0; i < snake.size(); i++) {
                coord temp = snake[i];
                drawParts(hdc, (temp.x+2)*10, (temp.y+2)*10, (i == int(snake.size()) - 1) ? 1 : 0);
            }
            // Draw food
            for (auto x : foodList) {
                drawParts(hdc, (x.x+2)*10, (x.y+2)*10, 2);
            }
            // // Draw score text
            char buffer[512];
            StringCbPrintfA(buffer, sizeof(buffer), "Score: %d", (int)score);
            TextOutA(hdc, 20, 15, buffer, strlen(buffer));
            // ENDPAINT
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_KEYDOWN: {
            // Move rectangle on arrow key presses
            switch (wParam) {
                // std::cout << wParam << std::endl;
                case VK_UP:
                    processMovement(-2);
                    break;
                case VK_DOWN:
                    processMovement(+2);
                    break;
                case VK_LEFT:
                    processMovement(-1);
                    break;
                case VK_RIGHT:
                    processMovement(+1);
                    break;
                default:
                    break;
            }
            return 0;
        }
        case WM_TIMER: {
            InvalidateRect(hwnd, NULL, TRUE); // Request a redraw
            return 0;
        }
        case WM_DESTROY:
            char buffer[512];
            StringCbPrintfA(buffer, sizeof(buffer), "Score: %d", (int)score);
            int res = MessageBoxA(hwnd, (LPCSTR)buffer, (LPCSTR)"Game over!", MB_OK | MB_TASKMODAL);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    const char CLASS_NAME[] = "SoNekGame";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc; // Set the window procedure
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    // Register the window class
    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "SnakeGame",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position
        window.y+16, window.x+38,             // Size
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Prepare game
    // add starting node
    for (int i = 0; i < 3; i++) {
        coord snakeStart = {((int)((gameCoord.y-2))/2)+i, ((int)((gameCoord.x-2))/2)};  
        addNode(snakeStart);
    }
    for (int i = 0; i < 4; i++) {
        createRandomFood(1, 1, gameCoord.x-2, gameCoord.y-2, snake);
    }

    // // DEBUG game init
    // for (auto x : snake) {
    //     std::cout << x.x << "-" << x.y << std::endl;
    // }
    // for (auto x : foodList) {
    //     std::cout << x.x << "_" << x.y << std::endl;
    // }
    

    // Set a timer to update the rectangle position periodically
    SetTimer(hwnd, 1, speed, NULL); // Timer ID = 1

    // Run the message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    KillTimer(hwnd, 1); // Clean up the timer
    return 0;
}
