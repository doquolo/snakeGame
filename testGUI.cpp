#include <windows.h>
#include <string>
#include <iostream>

struct coord {
    int x, y;
};

coord rect = {5, 5};
const int diag = 100;

void updateRect(int direction) {
    switch (direction) {
        case +1:
            rect.y--;
            break;
        case -1:
            rect.y++;
            break;
        case +2:
            rect.x--;
            break;
        case -2:
            rect.x++;
            break;
    }
}

// Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rectangle = {rect.x, rect.y, rect.x+diag, rect.y+diag};
            FillRect(hdc, &rectangle, (HBRUSH)GetStockObject(BLACK_BRUSH));

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_KEYDOWN: {
            // Move rectangle on arrow key presses
            switch (wParam) {
                std::cout << wParam << std::endl;
                case VK_UP:
                    updateRect(+1);
                    break;
                case VK_DOWN:
                    updateRect(-1);
                    break;
                case VK_LEFT:
                    updateRect(+2);
                    break;
                case VK_RIGHT:
                    updateRect(-2);
                    break;
                default:
                    break;
            }
            InvalidateRect(hwnd, NULL, TRUE); // Request a redraw
            return 0;
        }
        case WM_TIMER: {
            InvalidateRect(hwnd, NULL, TRUE); // Request a redraw
            return 0;
        }
        case WM_DESTROY:
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
        500, 400,                       // Size
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Set a timer to update the rectangle position periodically
    SetTimer(hwnd, 1, 100, NULL); // Timer ID = 1, Interval = 100ms

    // Run the message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    KillTimer(hwnd, 1); // Clean up the timer
    return 0;
}
