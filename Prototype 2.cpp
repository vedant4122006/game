#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <chrono>

using namespace std;

BOOL setCursorToPosition(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

bool isVirtualKeyPressed(int vk) {
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

int main() {
    int moveX = 0, moveY = 0;
    char symbol = '*';

    cout << "Press a number key (0-9) to set the symbol." << endl;

    // Choose a symbol
    while (true) {
        for (int i = '0'; i <= '9'; i++) {
            if (isVirtualKeyPressed(i)) {
                symbol = ")!@#$%^&*("[i - '0'];
                goto START_DRAWING;
            }
        }

        if (isVirtualKeyPressed(VK_ESCAPE))
            return 0;

        this_thread::sleep_for(chrono::milliseconds(10));
    }

START_DRAWING:
    system("cls");
    cout << "Symbol set to: " << symbol << endl;
    cout << "Use arrow keys (combine for diagonals), DEL to clear, ESC to exit." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    system("cls");

    while (true) {
        // Diagonal and single directions
        if (isVirtualKeyPressed(VK_UP)) moveY--;
        if (isVirtualKeyPressed(VK_DOWN)) moveY++;
        if (isVirtualKeyPressed(VK_LEFT)) moveX--;
        if (isVirtualKeyPressed(VK_RIGHT)) moveX++;

        // Clear screen
        if (isVirtualKeyPressed(VK_DELETE)) {
            system("cls");
        }

        // Exit
        if (isVirtualKeyPressed(VK_ESCAPE)) {
            break;
        }

        setCursorToPosition(10 + moveX, 10 + moveY);
        cout << symbol;

        this_thread::sleep_for(chrono::milliseconds(50)); // Adjust speed here
    }

    return 0;
}
