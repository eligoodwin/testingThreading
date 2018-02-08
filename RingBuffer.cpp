//
// Created by eli goodwin on 2/7/18.
//

#include "RingBuffer.h"
using namespace std;
RingBuffer::RingBuffer(int size) {
    this->capacity = size;
    dataNode* temp = new dataNode(false, "");
    this->head->next = temp;

    for(int i = 0; i < this->capacity; ++i){
        temp->next = new dataNode(false, "");
        temp = temp->next;

        if(i == this->capacity - 1){
            this->tail = temp;
            temp->next = this->head;
        }
    }
}

RingBuffer::~RingBuffer() {
    dataNode* current = this->head;
    dataNode* temp = nullptr;
    while(current) {
        temp = current->next;
        delete current;
        current = temp;
    }
    this->head = nullptr;
    this->tail = nullptr;
}

void RingBuffer::pushToBuffer(bool origin, std::string message) {

}

