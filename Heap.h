#pragma once
#include "all.h"

using std::string;
using std::vector;

template<typename T, class Comp = std::less<T> >
class Heap {
private:
    Comp compare;
    void fixHeap(int);
    vector<T> m;
public:
    Heap();
    void push(T);
    T getmin();
    bool empty();
};

template<typename T, class Comp>
bool Heap<T, Comp>::empty() {
    return m.size() == 0;
}

template<typename T, class Comp>
T Heap<T, Comp>::getmin() {
    T tmp = m[0];
    int last = m.size() - 1;
    swap(m[0], m[last]);
    m.pop_back();
    fixHeap(0);
    return tmp;
}

template<typename T, class Comp>
void Heap<T, Comp>::fixHeap(int x) {
    while (x && compare(m[x], m[x / 2]))
        swap(m[x / 2], m[x]), x /= 2;
    while (x * 2 < (int)m.size())
        if (compare(m[x * 2], m[x]) && (x * 2 == m.size() - 1 || compare(m[x * 2], m[x * 2 + 1])))
            swap(m[x * 2], m[x]), x *= 2;
        else if(x * 2 + 1 < (int)m.size() && compare(m[x * 2 + 1], m[x]))
            swap(m[x * 2 + 1], m[x]), x = x * 2 + 1;
        else
            break;
}

template<typename T, class Comp>
void Heap<T, Comp>::push(T el) {
    m.push_back(el);
    fixHeap(m.size() - 1);
}

template<typename T, class Comp>
Heap<T, Comp>::Heap() {}