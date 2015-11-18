#pragma once
#include "all.h"
#include "External.h"

template<typename T>
class ExternalReverse :
    public ExternalAlgorithm<T> {
private:
    int cnt;
    virtual void calcBlock(vector<T>&);
    virtual void mergePrecalc();
    virtual bool nextElem(T&);
public:
    ExternalReverse(IoSerialiser<T>, string, int);
};

template<typename T>
ExternalReverse<T>::ExternalReverse(IoSerialiser<T> s, string tmpDir = ".\tmp", int maxNInRAM = 1E7)
    : ExternalAlgorithm(s, tmpDir, maxNInRAM) {}

template<typename T>
void ExternalReverse<T>::calcBlock(vector<T>& block) {
    std::reverse(block.begin(), block.end());
}

template<typename T>
void ExternalReverse<T>::mergePrecalc() {
    assert(serialisers.size() != 0);
    cnt = serialisers.size() - 1;
}

template<typename T>
bool ExternalReverse<T>::nextElem(T& ans) {
    assert(cnt >= 0);
    while(cnt >= 0 && serialisers[cnt]->empty()) {
        serialisers[cnt]->close();
        --cnt;
    }
    if (cnt < 0) return 0;
    ans = serialisers[cnt]->get();

    return 1;
}