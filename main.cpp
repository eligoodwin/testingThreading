#include <iostream>
#include <thread>
#include <unistd.h>
#include <ncurses.h>
#include "RingBuffer.h"
using namespace std;

void interruptingCow(RingBuffer& ringBuffer);
void updateMessageWindow(WINDOW *messageWindow, RingBuffer &ringBuffer);
void pushMessageToBuffer(RingBuffer& ringBuffer, string message, bool origin);
void inputManager(WINDOW* inputWindow, RingBuffer& ringBuffer);
WINDOW *messageWindow = nullptr;

bool killthread = false;
int main(int argc, char *argv[]){
    int parent_x, parent_y;
    int score_size = 3; initscr();

    getmaxyx(stdscr, parent_y, parent_x);
    // set up initial windows
    WINDOW *inputWindow = nullptr;
    messageWindow = newwin(parent_y - score_size, parent_x, 0, 0);
    inputWindow = newwin(score_size, parent_x, parent_y - score_size, 0); // draw to our windows
    keypad(inputWindow, TRUE);
    int messageRows;
    int messsageCols;
    getmaxyx(messageWindow, messageRows, messsageCols);
    //ring buffer init

    RingBuffer ringBuffer((messageRows + 1));

    mvwprintw(messageWindow, 0, 0, "Messages");
    mvwprintw(inputWindow, 0, 0, "MESSAGE INPUT"); // refresh each window
    wrefresh(messageWindow);
    wrefresh(inputWindow);




    //begin mooing
    thread cowMow(interruptingCow, ref(ringBuffer));
    thread inputThread(inputManager, ref(inputWindow), ref(ringBuffer));

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

void inputManager(WINDOW* inputWindow, RingBuffer& ringBuffer){
    int inputRows;
    int inputCols;
    getmaxyx(inputWindow, inputRows, inputCols);
    char inputBuffer[200];
    while(strcmp(inputBuffer, "/quit") != 0){
        memset(inputBuffer, '\0', 200 * sizeof(char));
        wmove(inputWindow, inputRows - 1, 0);
        wclrtobot(inputWindow);
        wprintw(inputWindow, "SEND >> ");
        wgetstr(inputWindow, inputBuffer);
        pushMessageToBuffer(ringBuffer, inputBuffer, true);
        mvwprintw(inputWindow, inputRows - 2, 0, "USER ENTRY: %s", inputBuffer);
        wclrtoeol(inputWindow);
    }
    killthread = true;
}

void interruptingCow(RingBuffer& ringBuffer) {

    int i = 0;
    while(!killthread){
        string cowsays = " says MOOO: ";
        cowsays += to_string(i);
        pushMessageToBuffer(ringBuffer, cowsays, false);
        sleep(4);
        ++i;


    }
}

void updateMessageWindow(WINDOW *messageWindow, RingBuffer &ringBuffer){
    int bufferStart = ringBuffer.getStart();
    int bufferCapacity = ringBuffer.getCapacity();
    //clear the window
    wclear(messageWindow);
    Data* messageData = nullptr;
    //start writing to screen
    for(int i = 0; i < bufferCapacity; ++i){
        if(bufferStart == 0){
            messageData =  ringBuffer.iterateObject(i);
        }
        else{
            messageData = ringBuffer.iterateObject((i + bufferStart) % bufferCapacity);
        }

        if(!messageData->message.empty() && !messageData->fromClinet){
            //print client info to screen
            mvwprintw(messageWindow, i, 0, "COW SAYS: %s", messageData->message.c_str());
        }
        else if(!messageData->message.empty() && messageData->fromClinet){
            //print message to screen from server
            mvwprintw(messageWindow, i, 0, "YOU SAID: %s", messageData->message.c_str());
        }
    }
    //update the window
    wrefresh(messageWindow);
}

void pushMessageToBuffer(RingBuffer& ringBuffer, string message, bool origin){
    ringBuffer.push(message, origin);
    updateMessageWindow(messageWindow, ringBuffer);
}