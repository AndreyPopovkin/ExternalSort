#pragma once
#include "all.h"
#include "Serialiser.h"
#include "IoSerialiser.h"
#include <memory>

template<typename T>
class ExternalAlgorithm {
private:
    void getBlocks();
    void merge();
protected:
    bool LOG;
    int maxNInRAM;
    int numOfBlocks;
    int maxNPerBlock;
    std::vector<std::unique_ptr<Serialiser<T> > > serialisers;
    std::string tmpDir;
    IoSerialiser<T> ioSerialiser;
    virtual void calcBlock(vector<T>&) = 0;
    virtual void mergePrecalc() = 0;
    virtual bool nextElem(T&) = 0;
public:
    void setLogging(bool f) { LOG = f; }
    ExternalAlgorithm(IoSerialiser<T>, string, int);
    ~ExternalAlgorithm();
    void start();
};

template<typename T>
ExternalAlgorithm<T>::ExternalAlgorithm(IoSerialiser<T> s, string tmpDir = ".\tmp", int maxNInRAM = 1E7)
    : ioSerialiser(s), tmpDir(tmpDir), maxNInRAM(maxNInRAM), LOG(0) {}


template<typename T>
ExternalAlgorithm<T>::~ExternalAlgorithm() {
    /*for (size_t i = 0; i < serialisers.size(); ++i)
        if (serialisers[i])
            delete serialisers[i];*/
}

template<typename T>
void ExternalAlgorithm<T>::merge() {
    maxNPerBlock = maxNInRAM / numOfBlocks;
    assert(maxNPerBlock > 0);

    for (auto i = serialisers.begin(); i != serialisers.end(); ++i)
        (*i)->openInMode('r', maxNPerBlock);

    mergePrecalc();

    OutputSerialiser<ELEM>* output = ioSerialiser.output;
    output->open();

    T tmp;
    while (nextElem(tmp))
        output->push(tmp);

    output->close();
}

template<typename T>
void ExternalAlgorithm<T>::getBlocks() {
    vector<T> elements;
    elements.resize(maxNInRAM);
    InputSerialiser<T>* input = ioSerialiser.input;
    input->open();
    int i = 0;
    numOfBlocks = 0;
    bool process = 1;
    while (process) {
        if (!input->empty())
            elements[i++] = input->get();
        else
            process = 0;
        if (i == maxNInRAM || (i != 0 && !process)) {
            if(LOG) cout << "creating and calc block " << numOfBlocks;
            elements.resize(i);
            calcBlock(elements);
            serialisers.push_back(std::unique_ptr<Serialiser<T> >(new Serialiser<T>(numOfBlocks++, tmpDir)));
            auto ser = prev(serialisers.end());
            (*ser)->openInMode('w');
            for (int q = 0; q < i; ++q)
                (*ser)->push(elements[q]);
            (*ser)->close();
            i = 0;
            if(LOG) cout << " complited\n";
        }
    }

    input->close();
}

template<typename T>
void ExternalAlgorithm<T>::start() {
    getBlocks();
    merge();
}