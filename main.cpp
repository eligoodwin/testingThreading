#include <iostream>
#include <thread>
#include <unistd.h>
#include <ncurses.h>
#include <cstring>
using namespace std;

void manageInput(WINDOW *inputWindow, WINDOW* messageWindow);
void interruptingCow(WINDOW* message, int messageRows);
string getInput(WINDOW* inputwindow);

using namespace std;
bool killthread = false;
WINDOW *messageWindow = nullptr;
WINDOW *inputWindow = nullptr;


int main(int argc, char *argv[]){
    int parent_x, parent_y;
    int score_size = 3; initscr();
    keypad(stdscr, TRUE);


    getmaxyx(stdscr, parent_y, parent_x);
    // set up initial windows
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

    //test thread


    string output;

    thread inputstuff(manageInput, ref(inputWindow), ref(messageWindow));
    thread annoyCow(interruptingCow, ref(messageWindow), messageRows);

    inputstuff.join();
    annoyCow.join();
    delwin(inputWindow);
    delwin(messageWindow);
    endwin();
    cout << "Ending program." << endl;

    return 0;
}

void manageInput(WINDOW *inputWindow, WINDOW* messageWindow){
    int messageRows;
    int messsageCols;
    getmaxyx(messageWindow, messageRows, messsageCols);
    int inputWindowRows;
    int inputWindowsCols;
    string output;
    getmaxyx(inputWindow, inputWindowRows, inputWindowsCols);
    char inputBuffer[1024];
    while(output != "/quit"){
        wmove(inputWindow, inputWindowRows - 1, 0);
        wclrtobot(inputWindow);
        mvwprintw(inputWindow, inputWindowRows - 1, 0, "SEND> ");
        output = getInput(inputWindow);
        memset(inputBuffer, '\0', 1024 * sizeof(char));
        strcpy(inputBuffer, output.c_str());
        wmove(messageWindow, messageRows - 2, 0);
        wclrtobot(messageWindow);
        mvwprintw(messageWindow, messageRows - 2, 0, inputBuffer);
        wrefresh(messageWindow);
    }
    killthread = true;
}


string getInput(WINDOW *inputWindow){
    string output;
    int ch = wgetch(inputWindow);
    while(ch != '\n'){
        output.push_back(ch);
        ch = wgetch(inputWindow);
    }
    if(output == "/quit"){
        return output;
    }
    output.insert(0, "Incoming message: ");
    return output;
}

void interruptingCow(WINDOW* message, int messageRows) {
    int i = 0;
    while(!killthread){
        wmove(messageWindow, messageRows - 4, 0);
        wclrtobot(messageWindow);
        mvwprintw(messageWindow, messageRows - 4, 0, "COW SAYS MOO %i\n", i);
        wrefresh(message);
        sleep(3);
        ++i;
    }
}

