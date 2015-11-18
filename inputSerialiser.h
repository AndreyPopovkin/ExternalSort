#pragma once
#include "all.h"

template<typename T>
class InputSerialiser {
private:
    std::ifstream in;
    bool buf;
    T next_;
    string input_file_name;
    void update_buff();
public:
    InputSerialiser(const string&);
    virtual void open();
    virtual void close();
    virtual T get();
    virtual bool empty();
};

template<typename T>
InputSerialiser<T>::InputSerialiser(const string& _input_file_name = "C:/Users/Howl/documents/visual studio 2015/Projects/Tim/Tim/ExternalSort/input") {
    input_file_name = _input_file_name;
}

template<typename T>
void InputSerialiser<T>::open() {
    in.open(input_file_name, std::ios_base::in);
    assert(in.is_open());
}

template<typename T>
void InputSerialiser<T>::close() {
    in.close();
}

template<typename T>
void InputSerialiser<T>::update_buff() {
    assert(in.is_open());
    empty();
}

template<typename T>
bool InputSerialiser<T>::empty() {
    assert(in.is_open());
    if (buf) return 0;
    if (in.eof()) return 1;
    in >> next_;
    buf = !in.eof();
    return !buf;
}

template<typename T>
T InputSerialiser<T>::get() {
    assert(!empty());
    update_buff();
    buf = 0;
    //cout << next_ << "\n";
    return next_;
}