
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

// Utility function to set cursor position in the console
BOOL setCursorToPosition(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Utility function to check if a key is pressed
bool isVirtualKeyPressed(int vk) {
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

// Class representing the Drawing Board
class DrawingBoard {
private:
    int x, y;                // Current cursor position
    char symbol;             // Drawing symbol
    int colorCode;           // Current text color
    bool running;            // Program running flag

public:
    // Constructor to initialize variables
    DrawingBoard() {
        x = 10;
        y = 10;
        symbol = '*';        // Default symbol
        colorCode = 7;       // Default white color
        running = true;
    }

    // Destructor to show goodbye message or reset terminal state
    ~DrawingBoard() {
        setColor(7); // Reset color
        system("cls");
        cout << "Exiting Drawing Board. Goodbye!" << endl;
    }

    // Set text color using Windows API
    void setColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    // Show symbol selection menu
    void selectSymbol() {
        cout << "Select a symbol (0-9):" << endl;
        cout << "0: )  1: !  2: @  3: #  4: $  5: %  6: ^  7: &  8: *  9: (" << endl;

        while (true) {
            for (int key = '0'; key <= '9'; ++key) {
                if (isVirtualKeyPressed(key)) {
                    symbol = ")!@#$%^&*("[key - '0'];
                    cout << "Symbol selected: " << symbol << endl;
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

    // Show color selection menu
    void selectColor() {
        cout << "\nSelect a color (1-7):" << endl;
        cout << "1: Blue  2: Green  3: Cyan  4: Red  5: Magenta  6: Yellow  7: White" << endl;

        while (true) {
            for (int key = '1'; key <= '7'; ++key) {
                if (isVirtualKeyPressed(key)) {
                    colorCode = key - '0';
                    setColor(colorCode);
                    cout << "Color selected." << endl;
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

    // Handle movement and drawing logic
    void run() {
        cout << "\nUse arrow keys to move." << endl;
        cout << "Press C to change color." << endl;
        cout << "Press DELETE to clear screen, ESC to exit." << endl;

        while (running) {
            // Movement keys
            if (isVirtualKeyPressed(VK_LEFT)) x--;
            else if (isVirtualKeyPressed(VK_RIGHT)) x++;
            else if (isVirtualKeyPressed(VK_UP)) y--;
            else if (isVirtualKeyPressed(VK_DOWN)) y++;

            // Delete clears screen
            if (isVirtualKeyPressed(VK_DELETE)) system("cls");

            // Change color while drawing
            if (isVirtualKeyPressed('C')) {
                colorCode = (colorCode % 7) + 1; // Cycle through colors 1-7
                setColor(colorCode);
                this_thread::sleep_for(chrono::milliseconds(150)); // avoid rapid switching
            }

            // Exit
            if (isVirtualKeyPressed(VK_ESCAPE)) break;

            // Set cursor position and draw the symbol
            setCursorToPosition(x, y);
            setColor(colorCode);
            cout << symbol;

            this_thread::sleep_for(chrono::milliseconds(20)); // Control drawing speed
        }
    }
};

// Main function
int main() {
    system("cls"); // Clear screen on start

    // Create object of DrawingBoard (constructor runs automatically)
    DrawingBoard board;

    // Separate steps
    board.selectSymbol();   // Step 1: Choose symbol
    if (_kbhit()) _getch(); // Clear leftover input
    board.selectColor();    // Step 2: Choose color

    // Step 3: Run the board
    board.run();

    return 0; // Destructor runs automatically
}


/* if(_kbhit())
{
    char ch = _getche();
    message += ch;
}*/

/* setCursorToPosition(30,15);
cout<<message;

LockWindowUpdate(GetConsoleWindow());
system ("cls");

LockWindowUpdate(NULL); */