#include <iostream>
#include <iomanip>
#include "matrix.hpp"

using namespace std;

void copy_test()
{
    cout << "Testing copy copying and assigning" << endl;

    Matrix<double> m1(3);
    cout << "m1:" << endl;
    m1.print();

    Matrix<double> m2 = m1; // via copy constructor
    cout << "m2 (via copy contructor):" << endl;
    m2.print();

    Matrix<double> m3(3);
    m3 = m1; // via assignment operator
    cout << "m3 (via assignment operator):" << endl;
    m3.print();

    m1.transpose();
    cout << "Transposed m1:" << endl;
    m1.print();

    cout << "m2 (no change):" << endl;
    m2.print();

    cout << "m3 (no change):" << endl;
    m3.print();

    cout << endl;
}

void test_minor()
{
    cout << "Testing minors:" << endl;

    const unsigned int size = 3;

    const Matrix<double> matrix(size);
    cout << "matrix:" << endl;
    matrix.print();

    for(unsigned int i = 0; i < size; i++)
        for(unsigned int j = 0; j < size; j++) {
            cout << "MinorMatrix(" << i << "," << j << "): " << endl;
            matrix.makeMinorMatrix(i,j).print();
        }

    cout << endl;
}

void test_determinant()
{
    cout << "Testing determinants:" << endl;
    const Matrix<double> matrix(2);
    cout << "Matrix:" << endl;
    matrix.print();

    cout << "Determinant: " << matrix.findDeterminant() << endl;

    cout << endl;
}

void test_inverse()
{
    cout << "Testing inverse:" << endl;

    const Matrix<double> matrix(3);
    cout << "Matrix:" << endl;
    matrix.print();

    cout << "Inverse:" << endl;
    Matrix<double> inverse = matrix.makeInverse();
    inverse.print();

    cout << "Product:" << endl;
    Matrix<double> prod = matrix * inverse;
    prod.print();

    cout << endl;
}

void pass_by_value(Matrix<double> m)
{
    cout << "Got matrix m by value:" << endl;
    m.print();
}

Matrix<double> create_matrix()
{
    cout << "Creating matrix..." << endl;
    const Matrix<double> m(10);
    m.print();
    return m;
}

void test_move()
{
    Matrix<double> m(5);
    Matrix<double> m1 = move(m);
    Matrix<double> m2(1);
    m2 = move(m1);
    cout << m.getSize() << endl;
    cout << m1.getSize() << endl;
    cout << m2.getSize() << endl;

    pass_by_value(move(m2));
}

void test_move_on_return()
{
    Matrix<double> m(1);
    m = create_matrix();
    cout << "Returned result:" << endl;
    m.print();
}

int main()
{
    try {
        copy_test();
        test_minor();
        test_determinant();
        test_inverse();
        test_move();
        test_move_on_return();
    } catch (string e) {
        cout << e << endl;
    }

    return 0;
}
