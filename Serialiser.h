#pragma once

#include "all.h"
using std::string;
using std::vector;

template<typename T>
class Serialiser {
private:
    void getbuff();
    void pushbuff();
    char type_;
    std::fstream io;
    string fileName;
    std::vector<T> buffer;
    int last, numInBuff;
    void update_buff();
public:
    Serialiser(int, string);
    void openInMode(char, int = 1);
    bool empty();
    T get();
    void push(T);
    void close();
    ~Serialiser();
};

template<typename T>
void Serialiser<T>::getbuff() {
    numInBuff = 0;
    last = 0;
    T tmp;
    while (numInBuff < (int)buffer.size() && (io >> tmp)) {
        buffer[numInBuff++] = tmp;
    }
}

template<typename T>
Serialiser<T>::~Serialiser() {
    close();
    std::remove(fileName.c_str());
}

template<typename T>
void Serialiser<T>::pushbuff() {
    for (int i = 0; i < last; ++i)
        io << buffer[i] << ' ';
    numInBuff = 0;
    last = 0;
}

template<typename T>
Serialiser<T>::Serialiser(int n, string s) {
    fileName = s + "/" + std::to_string(n);
}

template<typename T>
void Serialiser<T>::openInMode(char mode, int buffSize) {
    buffer.resize(buffSize);
    if (mode == 'r') io.open(fileName, std::ios_base::in);
    if (mode == 'w') io.open(fileName, std::ios_base::out);
    type_ = mode;
    assert(io.is_open());
    last = 0;
    numInBuff = 0;
}

template<typename T>
bool Serialiser<T>::empty() {
    update_buff();
    return last >= numInBuff;
}

template<typename T>
void Serialiser<T>::update_buff() {
    if (last >= numInBuff) getbuff();
}

template<typename T>
T Serialiser<T>::get() {
    assert(!empty());
    update_buff();
    return buffer[last++];
}

template<typename T>
void Serialiser<T>::push(T arg) {
    if (last >= (int)buffer.size()) pushbuff();
    buffer[last++] = arg;
}

template<typename T>
void Serialiser<T>::close() {
    if (type_ == 'r') {
        io.close();
        return;
    }
    if (type_ == 'w') {
        pushbuff();
        io.close();
        return;
    }
}