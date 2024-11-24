#include <windows.h>


#include "snake.h"
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

    // add starting node
    coord snakeStart1 = {3, 3}; 
    addNode(snakeStart1);
    coord snakeStart2 = {3, 4}; 
    addNode(snakeStart2);
    coord snakeStart3 = {3, 5}; 
    addNode(snakeStart2);
    coord snakeStart4 = {3, 6}; 
    addNode(snakeStart2);

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
        
        // draw game
        moveSnake();
        drawPlayArea(100, 30, snake);
        _sleep(100);
    }

    UnhookWindowsHookEx(hHook);
    return 0;
}
