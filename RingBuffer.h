//
// Created by eli goodwin on 2/7/18.
//

#ifndef TESTINGTHREADING_RINGBUFFER_H
#define TESTINGTHREADING_RINGBUFFER_H

#include <string>

typedef struct Node{
    bool fromSender;
    std::string message;
    Node* next;
    Node(bool origin, std::string message){
        fromSender = origin;
        this->message = message;
        next = nullptr;
    };
} dataNode;


class RingBuffer {
public:
    RingBuffer(int size);
    ~RingBuffer();
    void pushToBuffer(bool origin, std::string message);
    bool FillBuffer(dataNode buffer[]);

private:
int size;
    int capacity;
    dataNode* head;
    dataNode* tail;
};


#endif //TESTINGTHREADING_RINGBUFFER_H
