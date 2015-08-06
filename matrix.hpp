#ifndef MATRIX_H_
#define MATRIX_H_

#include <string>
#include <sstream>
#include <iomanip>

template <typename T>
class Matrix
{
	unsigned int size;
	T **data;

public:
	Matrix(const unsigned int size) : size(size)
	{
		std::cout << "Constructing matrix of size " << size << std::endl;
		data = new T*[size];
		for(unsigned int i = 0; i < size; i++) {
			data[i] = new T[size];
			for(unsigned int j = 0; j < size; j++) {
				data[i][j] = i*size + j; // default values: [0, size*size)
			}
		}
	}
	Matrix(const Matrix<T>& other) // copy constructor
	{
		copyOther(other);
	}	
	~Matrix()
	{
		std::cout << "Destructing matrix of size " << size << std::endl;
		clear();
	}

	Matrix<T>& operator= (const Matrix<T>& other)
	{
		if(this != &other) {
			clear();
			copyOther(other);
		}
		return *this;
	}

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

	void transpose()
	{
		for(unsigned int i = 0; i < size - 1; i++)
			for(unsigned int j = i + 1; j < size; j++) {
				T temp = data[i][j];
				data[i][j] = data[j][i];
				data[j][i] = temp;
			}
	}

	Matrix<T> getMinorMatrix(const unsigned int row_index, const unsigned int column_index) const
	{
		if(row_index >= size || column_index >= size) {
			throw std::string("Index out of range");
		}

		Matrix<T> minor(size-1);

		for(unsigned int i = 0; i < size; ++i)
		{
			T* row = minor.getRow(i > row_index ? i - 1 : i);
			for(unsigned int j = 0; j < size; ++j) {
				row[j > column_index ? j - 1 : j] = data[i][j];
			}
		}

		return minor;
	}

	T getDeterminant() const
	{
		if(size == 1) {
			return data[0][0];
		}

		T det = 0;
		for(unsigned int i = 0; i < size; i++) {
			for(unsigned int j = 0; j < size; j++) {
				det += this->getCofactor(i, j);
			}
		}
	}

	T getMinor(const unsigned int i, const unsigned int j) const
	{
		if(i >= size || j >= size) {
			throw std::string("Index out of range (minor)");
		}

		return getMinorMatrix(i, j).getDeterminant();
	}

	T getCofactor(const unsigned int i, const unsigned int j) const
	{
		if((i + j) % 2 == 0) {
			return this->getMinor(i, j);
		} else {
			return -this->getMinor(i, j);
		}
	}

	void print() const
	{
		for(unsigned int i = 0; i < size; ++i) {
			for(unsigned int j = 0; j < size; ++j) {
				std::cout << std::setw(4) << data[i][j];
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