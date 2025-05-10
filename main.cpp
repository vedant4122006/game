#include <iostream>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

// Constants for drawing area boundaries
const int DRAW_LEFT =5;
const int DRAW_TOP =5;
const int DRAW_RIGHT =99;
const int DRAW_BOTTOM =27;

// Utility to move cursor
BOOL setCursorToPosition(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Utility to set color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Check if key is pressed
bool isVirtualKeyPressed(int vk) {
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

// Class for drawing board
class DrawingBoard {
private:
    int x, y;              // Current position
    char symbol;           // Drawing symbol
    int colorCode;         // Drawing color
    bool running;          // Loop flag

public:
    DrawingBoard() {
        x = (DRAW_LEFT + DRAW_RIGHT) / 2;
        y = (DRAW_TOP + DRAW_BOTTOM) / 2;
        symbol = '*';
        colorCode = 7;
        running = true;
    }

    ~DrawingBoard() {
        setColor(7);
        system("cls");
        cout << "Exiting Drawing Board. Goodbye!\n";
    }

    void drawFrame() {
        setColor(7);
        for (int i = DRAW_LEFT; i <= DRAW_RIGHT; ++i) {
            setCursorToPosition(i, DRAW_TOP); cout << '-';
            setCursorToPosition(i, DRAW_BOTTOM); cout << '-';
        }
        for (int i = DRAW_TOP; i <= DRAW_BOTTOM; ++i) {
            setCursorToPosition(DRAW_LEFT, i); cout << '|';
            setCursorToPosition(DRAW_RIGHT, i); cout << '|';
        }
        setCursorToPosition(DRAW_LEFT, DRAW_TOP); cout << '+';
        setCursorToPosition(DRAW_RIGHT, DRAW_TOP); cout << '+';
        setCursorToPosition(DRAW_LEFT, DRAW_BOTTOM); cout << '+';
        setCursorToPosition(DRAW_RIGHT, DRAW_BOTTOM); cout << '+';
    }

    void displayInstructions() {
        setColor(7); // White color for text
        setCursorToPosition(0, DRAW_BOTTOM + 2);
        cout << "Controls: Arrow Keys to draw | C: Change color | 0-9: Symbol | DEL: Clear | ESC: Exit   ";
    }

    void selectSymbol() {
        setCursorToPosition(0, 0);
        setColor(7);
        cout << "Select a symbol (0-9):\n";
        cout << "0: )  1: !  2: @  3: #  4: $  5: %  6: ^  7: &  8: *  9: (\n";

        while (true) {
            for (int key = '0'; key <= '9'; ++key) {
                if (isVirtualKeyPressed(key)) {
                    symbol = ")!@#$%^&*("[key - '0'];
                    cout << "\nSymbol selected: " << symbol << endl;
                    this_thread::sleep_for(chrono::milliseconds(500));
                    return;
                }
            }
            if (isVirtualKeyPressed(VK_ESCAPE)) {
                running = false;
                return;
            }
            this_thread::sleep_for(chrono::milliseconds(5));
        }
    }

    void run() {
        system("cls");
        drawFrame();
        displayInstructions();
        setColor(colorCode);

        while (running) {
            // Movement input
            if (isVirtualKeyPressed(VK_LEFT)) x--;
            if (isVirtualKeyPressed(VK_RIGHT)) x++;
            if (isVirtualKeyPressed(VK_UP)) y--;
            if (isVirtualKeyPressed(VK_DOWN)) y++;

            // Boundary check
            if (x <= DRAW_LEFT + 1) x = DRAW_LEFT + 1;
            if (x >= DRAW_RIGHT - 1) x = DRAW_RIGHT - 1;
            if (y <= DRAW_TOP + 1) y = DRAW_TOP + 1;
            if (y >= DRAW_BOTTOM - 1) y = DRAW_BOTTOM - 1;

            // Clear screen and redraw border
            if (isVirtualKeyPressed(VK_DELETE)) {
                system("cls");
                drawFrame();
                displayInstructions();
            }

            // Change color
            if (isVirtualKeyPressed('C')) {
                colorCode = (colorCode % 7) + 1;
                setColor(colorCode);
                this_thread::sleep_for(chrono::milliseconds(200));
            }

            // Change symbol anytime
            for (int key = '0'; key <= '9'; ++key) {
                if (isVirtualKeyPressed(key)) {
                    symbol = ")!@#$%^&*("[key - '0'];
                    this_thread::sleep_for(chrono::milliseconds(200));
                }
            }

            // Exit
            if (isVirtualKeyPressed(VK_ESCAPE)) break;

            // Draw
            setColor(colorCode);
            setCursorToPosition(x, y);
            cout << symbol;

            this_thread::sleep_for(chrono::milliseconds(30)); // Smoothness
        }
    }
};

// Main
int main() {
    
    system("mode con: cols=100 lines=30"); // Resize terminal
    system("cls");
    DrawingBoard board;
    board.selectSymbol(); // Select symbol first
    board.run();          // Run drawing loop
    return 0;
}
