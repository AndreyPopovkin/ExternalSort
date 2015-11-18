#include "all.h"
#include "ExternalSort.h"
#include "ExternalReverse.h"
#include "IoSerialiser.h"
#include "inputSerialiser.h"
#include "OutputSerialiser.h"


// вся работа с файлами задается из этого
// далее несколько констант, позволяюих протестировать различные возможности в разных условиях


const int MAXN = 1000;
// общее кол-во элементов, в конце сортруются std::sort для проверки
const int MAXMEM = 200;
// ограничение на кол-во элементов в памяти
const bool algo = 1;
// algo == 1 - sort, 0 - reverse
const bool ORDER = 0;
// ORDER == 1 - true, 0 - reversed (without using reverse)
const bool LOG = 0;
// выводит инф о каждом считанном блоке
const int TESTS_NUM = 10;
// количество тестов

typedef int ELEM;
// готово тестирование для int и string
// строки длины до 200 из мал латинских букв


using std::vector;
using std::cout;
using std::cin;
using std::string;

void gen_test(string filename, int N) {
    std::ofstream out_;
    srand(clock());
    out_.open(filename, std::ios_base::out);

    if (typeid(ELEM) == typeid(int)) {
        for (int i = 0; i < N; ++i)
            out_ << rand() << ' ';
    }
    else if (typeid(ELEM) == typeid(string)) {
        string tmp;
        for (int i = 0; i < N; ++i) {
            size_t size = rand() % 100;
            tmp.resize(size);
            for (size_t q = 0; q < size; ++q)
                tmp[q] = 'a' + rand() % 26;
            out_ << tmp << ' ';
        }
    }
    else if (typeid(ELEM) == typeid(vector<int>)) {
        vector<int> tmp;
        for (int i = 0; i < N; ++i) {
            size_t size = rand() % 100;
            out_ << size << ' ';
            for (size_t q = 0; q < size; ++q)
                out_ << rand() << ' ';
        }
    }

    cout << "generating done\n";
    out_.close();
}

template<typename T>
class Greater {
public:
    bool operator ()(const T& a, const T& b) {
        return a > b;
    }
};

int check(string input, string output, bool f) {
    // f = 1 - sort, 0 - reverse
    std::ifstream in_;
    in_.open(input, std::ios_base::in);
    assert(in_.is_open());
    std::vector<ELEM> v;
    ELEM tmp;
    in_ >> tmp;
    do {
        v.push_back(tmp);
        in_ >> tmp;
    } while (!in_.eof());
    if (f) 
        if(ORDER)
            sort(v.begin(), v.end(), std::less<ELEM>());
        else 
            sort(v.begin(), v.end(), Greater<ELEM>());
    else reverse(v.begin(), v.end());
    in_.close();
    in_.open(output, std::ios_base::in);
    for (size_t i = 0; i < v.size(); ++i) {
        in_ >> tmp;
        if (tmp != v[i]) {
            cout << "Error!!!\n";
            return 0;
        }
    }
    cout << "Ok\n";
    return 1;
}

std::pair<int, int> test() {
    gen_test("./input", MAXN);
    string input_file_name("./input");
    string output_file_name("./output");
    InputSerialiser<ELEM> in(input_file_name);
    OutputSerialiser<ELEM> out(MAXMEM, output_file_name);
    IoSerialiser<ELEM> ser(&in, &out);
    int t = clock();
    try {
        if (algo) {
            if (ORDER) {
                ExternalSort<ELEM, std::less<ELEM> > algo(ser, "./tmp", MAXMEM);
                algo.setLogging(LOG);
                algo.start();
            }
            else {
                ExternalSort<ELEM, Greater<ELEM> > algo(ser, "./tmp", MAXMEM);
                algo.setLogging(LOG);
                algo.start();
            }
        }
        else {
            ExternalReverse<ELEM> algo(ser, "./tmp", MAXMEM);
            algo.setLogging(LOG);
            algo.start();
        }
    }
    catch (...) {
        cout << "Getting exception\n";
        return std::pair<int, int>(0, clock() - t);
    }
    t = clock() - t;
    return std::pair<int, int>(check("./input", "./output", algo), t);
}

int main() {
    int sum = 0;
    int tmin = 1000 * 1000 * 1000;
    int tmax = 0;
    int tsum = 0;
    for (int i = 0; i < TESTS_NUM; ++i) {
        cout << "Test " << i + 1 << ":\n";
        std::pair<int, int> tmp = test();
        sum += tmp.first;
        tsum += tmp.second;
        tmin = std::min(tmin, tmp.second);
        tmax = std::max(tmax, tmp.second);
    }
    cout << "Testing DONE!\n";
    cout << "Accuracy: " << 100.0 * sum / TESTS_NUM << "%\n";
    cout << "min t = " << 1.0 * tmin / CLOCKS_PER_SEC << "\n";
    cout << "max t = " << 1.0 * tmax / CLOCKS_PER_SEC << "\n";
    cout << "average t = " << 1.0 * tsum / TESTS_NUM / CLOCKS_PER_SEC << "\n";

    system("pause");
}