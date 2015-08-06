#ifndef MATRIX_H_
#define MATRIX_H_

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <iostream>

template <typename T>
class Matrix
{
    unsigned int size;
    T **data;

public:
    Matrix(const unsigned int size) : size(size)
    {
        data = new T*[size];
        for(unsigned int i = 0; i < size; i++) {
            data[i] = new T[size];
            for(unsigned int j = 0; j < size; j++) {
                data[i][j] = std::rand() % 10;
            }
        }
    }
    Matrix(const Matrix<T>& other) // copy constructor
    {
    	std::cout << "Copy constructor" << std::endl;
        copyOther(other);
    }
    Matrix(Matrix<T>&& other) : data(other.data), size(other.size) // move constructor
    {
    	std::cout << "Move constructor" << std::endl;
    	other.data = nullptr;
    	other.size = 0;
    }
    ~Matrix()
    {
        clear();
    }

    Matrix<T>& operator= (const Matrix<T>& other) // copy assignment
    {
        if(this != &other) {
            clear();
            copyOther(other);
        }
        return *this;
    }

    Matrix<T>& operator= (Matrix<T>&& other) // move assignment
    {
        if(this != &other) {
            clear();
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    Matrix<T> operator* (const Matrix<T>& b) const
    {
        if(this->getSize() != b.getSize()) {
            throw std::string("Sizes do not match");
        }

        Matrix<double> a = *this;
        a.transpose();

        Matrix<double> result(size);

        for(unsigned int i = 0; i < size; i++) {
            T* result_row = result.getRow(i);
            for(unsigned int j = 0; j < size; j++) {
                // multiply A_column and B_row
                T* a_col = a.getRow(i);
                T* b_col = b.getRow(j);
                result_row[j] = 0;
                for(unsigned k = 0; k < size; k++) {
                    result_row[j] += a_col[k] * b_col[k];
                }
            }
        }

        return result;
    }

    /* Simple getters */
    unsigned int getSize() const
    {
        return size;
    }

    T* getRow(const unsigned row_index) const
    {
        if(row_index >= size) {
            std::stringstream s;
            s << "Row index out of range: " << row_index << " >= " << size << " //" << __FILE__ << ": " << __LINE__;
            throw s.str();
        }

        return data[row_index];
    }

    /* Transformations */
    void transpose()
    {
        for(unsigned int i = 0; i < size - 1; i++)
            for(unsigned int j = i + 1; j < size; j++) {
                T temp = data[i][j];
                data[i][j] = data[j][i];
                data[j][i] = temp;
            }
    }	

    Matrix<T> makeMinorMatrix(const unsigned int row_index, const unsigned int column_index) const
    {
        if(row_index >= size) {
            throw std::string("Row index out of range");
        }
        if(column_index >= size) {
            throw std::string("Column index out of range");
        }

        Matrix<T> minor_matrix(size-1);

        for(unsigned int i = 0; i < size; ++i)
        {
            T* row;
            if(i < row_index) {
                row = minor_matrix.getRow(i);
            } else if(i == row_index) {
                continue;
            } else {
                row = minor_matrix.getRow(i - 1);
            }
            for(unsigned int j = 0; j < size; ++j) {
                if(j < column_index) {
                    row[j] = data[i][j];
                } else if(j == column_index) {
                    continue;
                } else {
                    row[j-1] = data[i][j];
                }
            }
        }

        return minor_matrix;
    }

    Matrix<T> makeInverse() const
    {
        Matrix<T> result(size);

        T det = findDeterminant();

        if(!det) {
            throw std::string("No inverse exists: matrix determinant equals zero");
        }

        for(unsigned int i = 0; i < size; ++i) {
            T* row = result.getRow(i);
            for(unsigned int j = 0; j < size; ++j) {
                row[j] = findCofactor(j, i) / det;
            }
        }

        return result;
    }

    /* Calculations */
    T findMinor(const unsigned int i, const unsigned int j) const
    {
        if(i >= size || j >= size) {
            throw std::string("Index out of range (minor)");
        }

        return makeMinorMatrix(i, j).findDeterminant();
    }

    T findCofactor(const unsigned int i, const unsigned int j) const
    {
        if((i + j) % 2 == 0) {
            return this->findMinor(i, j);
        } else {
            return -1 * this->findMinor(i, j);
        }
    }

    T findDeterminant() const
    {
        if(size == 1) {
            return data[0][0];
        }

        T det = 0;
        unsigned int j = 0;
        for(unsigned int i = 0; i < size; i++) {
            det += data[i][j] * this->findCofactor(i, j);
        }

        return det;
    }

    void print() const
    {
        for(unsigned int i = 0; i < size; ++i) {
            for(unsigned int j = 0; j < size; ++j) {
                std::cout << std::setprecision(4) << data[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

private:
    void copyOther(const Matrix<T>& other)
    {
        size = other.getSize();
        data = new T*[size];
        for(unsigned int i = 0; i < size; i++) {
            data[i] = new T[size];
            T* row = other.getRow(i);
            for(unsigned int j = 0; j < size; j++) {
                data[i][j] = row[j];
            }
        }
    }
    void clear()
    {
        for(unsigned int i = 0; i < size; ++i) {
            delete [] data[i];
        }
        delete [] data;
    }
};

#endif // MATRIX_H_
