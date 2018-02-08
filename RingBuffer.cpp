//
// Created by eli goodwin on 2/7/18.
//

#include "RingBuffer.h"

using namespace std;

RingBuffer::RingBuffer(int capacity) {
    this->capacity = capacity;
    currentIndex = 0;
    storage.reserve(capacity);
    size = 0;
};

void RingBuffer::push(string mess, bool fromClient){
    lock_guard<std::mutex> guard(protectBuffer);
    if(size < capacity){
        messageData newData = messageData(fromClient, mess);
        storage.push_back(newData);
        ++currentIndex;
        ++size;
        return;
    }
    storage[currentIndex % capacity].fromClinet = fromClient;
    storage[currentIndex % capacity].message = mess;
    ++currentIndex;
}

messageData* RingBuffer::iterateObject(int i){
    return &(storage[i]);
}


messageData* RingBuffer::getTopMessage() {
    return &(storage[(currentIndex - 1)% capacity]);
}

int RingBuffer::getSize() {
    return this->size;
}

int RingBuffer::getCapacity() {
    return this->capacity;
}

int RingBuffer::getStart() {
    if(size < capacity){
        return 0;
    }
    return (currentIndex + 1) % capacity;
}