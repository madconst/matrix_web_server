#include <iostream>
#include <iomanip>
#include "matrix.hpp"

using namespace std;

void test()
{
    const unsigned int size = 2;
    const Matrix<double> matrix(size);
    cout << "Our matrix:" << endl;
    matrix.print();

    matrix.getMinorMatrix(0,0).print();
    matrix.getMinorMatrix(0,1).print();
    matrix.getMinorMatrix(1,0).print();
    matrix.getMinorMatrix(1,1).print();

/*
    for(unsigned int i = 0; i < 2; ++i)
        for(unsigned int j = 0; j < 1; ++j) {
            cout << "Minor (" << i << "," << j << "): " << m.getMinor(i, j) << endl;
        }
*/
    // cout << "Det(M): " << m.getDeterminant() << endl;
}

int main()
{
try {
    test();
} catch (string e) {
    cout << e << endl;
}
    return 0;
}