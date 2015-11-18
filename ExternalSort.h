#pragma once
#include "all.h"
#include "External.h"
#include "Heap.h"

using std::pair;

template<typename T, class Comp>
class Heap_pair_comp{
private:
    Comp compare;
public:
    bool operator()(const pair<T, int>& a, const pair<T, int>& b) {
        return compare(a.first, b.first) || (!compare(b.first, a.first) && a.second < b.second);
    }
};

template<typename T, class Comp = std::less<T> >
class ExternalSort :
    public ExternalAlgorithm<T> {
private:
    Comp compare;
    Heap<std::pair<T, int>, Heap_pair_comp<T, Comp> > heap;
    virtual void calcBlock(vector<T>& block);
    virtual void mergePrecalc();
    virtual bool nextElem(T&);
public:
    ExternalSort(IoSerialiser<T>, string, int);
};

template<typename T, class Comp>
ExternalSort<T, Comp>::ExternalSort(IoSerialiser<T> s, string tmpDir = ".\tmp", int maxNInRAM = 1E7)
    : ExternalAlgorithm(s, tmpDir, maxNInRAM) {}

template<typename T, class Comp>
void ExternalSort<T, Comp>::calcBlock(vector<T>& block) {
    std::sort(block.begin(), block.end(), compare);
}

template<typename T, class Comp>
void ExternalSort<T, Comp>::mergePrecalc() {
    for (size_t i = 0; i < serialisers.size(); ++i) {
        assert(!serialisers[i]->empty());
        T elem = serialisers[i]->get();
        heap.push(std::make_pair(elem, i));
    }
}

template<typename T, class Comp>
bool ExternalSort<T, Comp>::nextElem(T& ans) {
    if (heap.empty()) return 0;

    std::pair<T, int> tmp = heap.getmin();
    if (!serialisers[tmp.second]->empty())
        heap.push(std::make_pair(serialisers[tmp.second]->get(), tmp.second));
    else
        serialisers[tmp.second]->close();
    ans = tmp.first;

    return 1;
}