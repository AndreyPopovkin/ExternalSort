#pragma once
#include "all.h"

template<typename T>
class OutputSerialiser {
private:
    vector<T> buffer;
    size_t n;
    std::ofstream out;
    string output_file_name;
    virtual void pushbuffer();
public:
    OutputSerialiser(int, const string&);
    virtual void open();
    virtual void push(T);
    virtual void close();
};

template<typename T>
OutputSerialiser<T>::OutputSerialiser(int n_, const string& _output_file_name){
    output_file_name = _output_file_name;
    n = n_;
}

template<typename T>
void OutputSerialiser<T>::open() {
    out.open(output_file_name, std::ios_base::out);
    assert(out.is_open());
}

template<typename T>
void OutputSerialiser<T>::close() {
    pushbuffer();
    out.close();
}

template<typename T>
void OutputSerialiser<T>::pushbuffer() {
    for (size_t i = 0; i < buffer.size(); ++i)
        out << buffer[i] << ' ';
    buffer.clear();
}

template<typename T>
void OutputSerialiser<T>::push(T arg) {
    assert(out.is_open());
    if (buffer.size() >= n) pushbuffer();
    buffer.push_back(arg);
}
