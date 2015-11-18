#pragma once
#include "all.h"
#include "inputSerialiser.h"
#include "OutputSerialiser.h"

template<typename T>
class IoSerialiser {
public:
    InputSerialiser<T>* input;
    OutputSerialiser<T>* output;
    IoSerialiser(InputSerialiser<T>*, OutputSerialiser<T>*);
    IoSerialiser() : input(0), output(0) {}
};

template<typename T>
IoSerialiser<T>::IoSerialiser(InputSerialiser<T>* input, OutputSerialiser<T>* output) : input(input), output(output) {}