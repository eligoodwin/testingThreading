#include <iostream>
#include <thread>
#include <unistd.h>
#include <ncurses.h>

using namespace std;

class MaxRows{
public:
    MaxRows(int row, int cols){
        MAXROW = row;
        MAXCOLS = cols;
    }
    int getRows(){
        return MAXROW;
    }
    int getCols(){
        return MAXCOLS;
    }

private:
    int MAXROW;
    int MAXCOLS;

};

MaxRows* maxRows = nullptr;
string getUserInput(){
    string output;
    clrtoeol();
    int ch = getch();
    while(ch != '\n'){
        output.push_back(ch);
        ch = getch();
    }
    return output;
}

void printOutput(WINDOW* inputWindow){
    int row, cols;
    getmaxyx(inputWindow, row, cols);
    string output;
    while(output != "/quit"){
        output = getUserInput();
        clrtobot();
        mvwprintw(inputWindow, row -1,0,"User input: %s\n", output.c_str());
        wrefresh(inputWindow);                 /* Print it on to the real screen */
    }/* Wait for user input */
}


int main(int argc, char *argv[]) {
    int parent_x, parent_y;
    int score_size = 3; initscr();
    //keypad(stdscr, TRUE);
//    nocbreak();
    // get our maximum window dimensions
    getmaxyx(stdscr, parent_y, parent_x);
    // set up initial windows
    WINDOW *messageWindow = newwin(parent_y - score_size, parent_x, 0, 0);
    WINDOW *inputWindow = newwin(score_size, parent_x, parent_y - score_size, 0); // draw to our windows
    mvwprintw(messageWindow, 0, 0, "Messages");
    mvwprintw(inputWindow, 0, 0, "Out"); // refresh each window
    wrefresh(messageWindow);
    wrefresh(inputWindow);
    int inputWindowRows;
    int inputWindowsCols;
    int messageRows;
    int messsageCols;
    char inputBuffer[1024];
    string output;
    do{
        getmaxyx(messageWindow, messageRows, messsageCols);
        getmaxyx(inputWindow, inputWindowRows, inputWindowsCols);
        wmove(inputWindow, inputWindowRows - 1, 0);
        wclrtobot(inputWindow);
        waddstr(inputWindow, "Input > ");
        int ch = wgetch(inputWindow);
        while(ch != '\n'){
            output.push_back(ch);
            ch = wgetch(inputWindow);
        }
        wmove(messageWindow, messageRows - 1, 0);
        if(output != "/quit"){
            output.insert(0, "User input: ");
            memset(inputBuffer, '\0', 1024 * sizeof(char));
            strcpy(inputBuffer, output.c_str());
        }
        wclrtobot(messageWindow);
        waddstr(messageWindow, inputBuffer);
        wrefresh(messageWindow);
        wrefresh(inputWindow);
    }while(output != "/quit");


    sleep(5); // clean up
    cout << "Ending program." << endl;
    delwin(inputWindow);
    delwin(messageWindow);
    endwin();
    return 0;
}