#include <iostream>
#include <thread>
#include <unistd.h>
#include <ncurses.h>
#include <cstring>
using namespace std;

void inputManager(WINDOW* inputWindow);
void interruptingCow(WINDOW* message, int messageRows);

using namespace std;
bool killthread = false;

int main(int argc, char *argv[]){
    int parent_x, parent_y;
    int score_size = 3; initscr();

    getmaxyx(stdscr, parent_y, parent_x);
    // set up initial windows
    WINDOW *messageWindow = nullptr;
    WINDOW *inputWindow = nullptr;
    messageWindow = newwin(parent_y - score_size, parent_x, 0, 0);
    inputWindow = newwin(score_size, parent_x, parent_y - score_size, 0); // draw to our windows
    keypad(inputWindow, TRUE);

    mvwprintw(messageWindow, 0, 0, "Messages");
    mvwprintw(inputWindow, 0, 0, "MESSAGE INPUT"); // refresh each window
    wrefresh(messageWindow);
    wrefresh(inputWindow);

    int messageRows;
    int messsageCols;
    getmaxyx(messageWindow, messageRows, messsageCols);


    //begin mooing
    thread cowMow(interruptingCow, ref(messageWindow), messageRows);
    thread inputThread(inputManager, ref(inputWindow));

    inputThread.join();
    cowMow.join();
    wclear(inputWindow);
    wclear(messageWindow);
    delwin(inputWindow);
    delwin(messageWindow);
    endwin();
    cout << "Ending program." << endl;

    return 0;
}

void inputManager(WINDOW* inputWindow){
    int inputRows;
    int inputCols;
    getmaxyx(inputWindow, inputRows, inputCols);
    char inputBuffer[200];
    memset(inputBuffer, '\0', 200 * sizeof(char));
    while(strcmp(inputBuffer, "/quit") != 0){
        wmove(inputWindow, inputRows - 1, 0);
        wclrtobot(inputWindow);
        wprintw(inputWindow, "SEND >> ");
        wgetstr(inputWindow, inputBuffer);
        mvwprintw(inputWindow, inputRows - 2, 0, "USER ENTRY: %s", inputBuffer);
        wclrtoeol(inputWindow);
    }
    killthread = true;
}

void interruptingCow(WINDOW* messageWindow, int messageRows) {
    int i = 0;
    while(!killthread){
        wclrtobot(messageWindow);
        mvwprintw(messageWindow, messageRows - 4, 0, "COW SAYS MOO %i\n", i);
        wrefresh(messageWindow);
        sleep(3);
        ++i;
    }
}

