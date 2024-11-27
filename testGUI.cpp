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

int speed = 100;

#include "snake.h"
#include "food.h"
#include "cli.h"

void drawSnake(HDC hdc, int x, int y) {
    RECT snakePart = {x, y, x+diag, y+diag};
    FillRect(hdc, &snakePart, (HBRUSH)GetStockObject(BLACK_BRUSH));
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
                // TODO: pausing game
                // wchar_t buffer[512];
                // wsprintfW(buffer, L"Score: %d", score);
                // MessageBox(hwnd, (LPCSTR)buffer, (LPCSTR)L"Game over!", NULL);
                DestroyWindow(hwnd);
            };
            if (checkFoodColision(gameCoord.x, gameCoord.y, snake)) {
                speed *= .98;
            };
            // Draw snake
            // for debugging
            // coord temp = snake.back();
            // std::cout << temp.x << "-" << temp.y << std::endl;
            for (auto x : snake) {
                drawSnake(hdc, (x.x+2)*10, (x.y+2)*10);
            }
            // Draw food
            for (auto x : foodList) {
                drawSnake(hdc, (x.x+2)*10, (x.y+2)*10);
            }
            // // Draw score text
            // LPRECT textBox = {0, 0, 12, 30};
            // char buffer[512];
            // StringCbPrintfA(buffer, sizeof(buffer), "Score: %d", (int)score);
            // DrawText(hdc, buffer, sizeof(buffer), textBox, DT_CENTER);
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

    const char CLASS_NAME[] = "DynamicRectangleWindow";

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
        "Dynamic Rectangle Example",    // Window text
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
        coord snakeStart = {(int)gameCoord.x/2, (int)(gameCoord.y/2)+i}; 
        addNode(snakeStart);
    }
    for (int i = 0; i < 3; i++) {
        createRandomFood(1, 1, gameCoord.x-2, gameCoord.y-2, snake);
    }

    // DEBUG game init
    for (auto x : snake) {
        std::cout << x.x << "-" << x.y << std::endl;
    }
    for (auto x : foodList) {
        std::cout << x.x << "_" << x.y << std::endl;
    }
    

    // Set a timer to update the rectangle position periodically
    SetTimer(hwnd, 1, speed, NULL); // Timer ID = 1, Interval = 100ms

    // Run the message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    KillTimer(hwnd, 1); // Clean up the timer
    return 0;
}
