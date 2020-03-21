#include <iostream>
#include <cstring>
#include "Matrix.h"

/**
 * const for Matrix
 * @param rows the number of rows in the matrix
 * @param cols the number of cols in the matrix
 */
Matrix::Matrix(int rows, int cols) :
        _matrixDims{rows = rows, cols = cols}
{
    if (rows < MIN_MATRIX_DIM || cols < MIN_MATRIX_DIM)
    {
        std::cerr << ERROR << std::endl;
        exit(FAILURE);
    }
    else
    {
        this->_table = new float[rows * cols]();
    }
}

/**
 * empty const, const 1x1 matrix, init the single ele to 0
 */
Matrix::Matrix() :
        Matrix(MIN_MATRIX_DIM, MIN_MATRIX_DIM)
{
}

/**
 * const matrix from anther matrix m
 * @param m the other matrix
 */
Matrix::Matrix(const Matrix &other) :
        _matrixDims{other._matrixDims.rows, other._matrixDims.cols}
{
    delete[](this->_table); // delete the current info
    this->_table = new float[other._matrixDims.rows * other._matrixDims.cols];
    for (int i = 0; i < _matrixDims.rows * _matrixDims.cols; ++i)
    {
        this->_table[i] = other._table[i];
    }
}

/**
 * dest for Matrix object
 */
Matrix::~Matrix()
{
    _matrixDims.cols = EMPTY_MATRIX;
    _matrixDims.rows = EMPTY_MATRIX;
    delete[](this->_table);
}

/**
 * getter for the rows
 * @return number of rows
 */
int Matrix::getRows()
{
    return this->_matrixDims.rows;
}

/**
 * gettter for the cols
 * @return number of cols
 */
int Matrix::getCols()
{
    return this->_matrixDims.cols;
}

/**
 * getter for the rows
 * @return number of rows
 */
int Matrix::getRows() const
{
    return this->_matrixDims.rows;
}

/**
 * gettter for the cols
 * @return number of cols
 */
int Matrix::getCols() const
{
    return this->_matrixDims.cols;
}

/**
 * Transforms a matrix into a coloumn vector Supports function calling concatenation i.e.(1)
 * Matrix m(5,4);... m.vectorize(), then m.vectorize() + b should be a valid expression
 */
Matrix Matrix::vectorize()
{
    int size = this->_matrixDims.rows * this->_matrixDims.cols;
    this->_matrixDims.rows = size;
    this->_matrixDims.cols = MIN_MATRIX_DIM;
    return *this;
}

/**
 * Prints matrix elements, no return value. prints space after each element (incl. last element in the row)
 * prints newline after each row (incl. last row)
 */
void Matrix::plainPrint() const
{
    for (int i = 0; i < this->_matrixDims.rows; ++i)
    {
        for (int j = 0; j < this->_matrixDims.cols; ++j)
        {
            std::cout << this->_table[i * this->_matrixDims.cols + j];
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

/**
*  overload the * operator to multiply matrix and scalar
* @param scalar the scalar we want to multiply the matrix by
* @param matrix the matrix
* @return scalar * matrix
*/
Matrix operator*(const float &scalar, const Matrix &matrix)
{
    Matrix output = Matrix(matrix);
    for (int i = 0; i < matrix.getRows() * matrix.getCols(); ++i)
    {
        output[i] *= scalar;
    }
    return output;
}

/**
*  overload the * operator to multiply matrix and scalar
* @param scalar the scalar we want to multiply the matrix by
* @return scalar * matrix
*/
Matrix Matrix::operator*(const float &scalar) const
{
    Matrix output = Matrix(*this);
    for (int i = 0; i < this->getRows() * this->getCols(); ++i)
    {
        output[i] *= scalar;
    }
    return output;
}

/**
 * overload the * operator, to multiply with other matrix
 * @param other the other matrix
 * @return the solution of 2 matrix multiplication
 */
Matrix Matrix::operator*(const Matrix &other) const
{
    if ((other._matrixDims.rows != this->_matrixDims.cols))
    {
        std::cerr << ERROR << std::endl;
        exit(FAILURE);
    }
    else
    {
        auto ans = Matrix(_matrixDims.rows, other._matrixDims.cols); // all zeros
        for (int i = 0; i < _matrixDims.rows; ++i)
        {
            for (int j = 0; j < other._matrixDims.cols; ++j)
            {
                for (int k = 0; k < _matrixDims.cols; ++k)
                {
                    float temp = _table[i * _matrixDims.cols + k];
                    ans._table[i * other._matrixDims.cols + j] += temp * other(k, j);
                }
            }
        }
        return ans;
    }
}

/**
 * overload the + operator to add our matrix with our matrix
 * @param other the other matrix we want to add to our
 * @return the addition matrix
 */
Matrix Matrix::operator+(const Matrix &other) const
{
    if ((other._matrixDims.rows != this->_matrixDims.rows) || ((other._matrixDims.cols != this->_matrixDims.cols)))
    {
        std::cerr << ERROR << std::endl;
        exit(FAILURE);
    }
    else
    {
        auto ans = Matrix(*this);
        for (int i = 0; i < _matrixDims.rows; ++i)
        {
            for (int j = 0; j < _matrixDims.cols; ++j)
            {
                ans._table[i * _matrixDims.cols + j] += other(i, j);
            }
        }
        return ans;
    }
}

/**
 * overload the + operator to add our matrix with our matrix
 * @param other the other matrix we want to add to our
 * @return our matrix += the other matrix values
 */
Matrix &Matrix::operator+=(const Matrix &other)
{
    if ((other._matrixDims.rows != this->_matrixDims.rows) || ((other._matrixDims.cols != this->_matrixDims.cols)))
    {
        std::cerr << ERROR << std::endl;
        exit(FAILURE);
    }
    else
    {
        for (int i = 0; i < this->_matrixDims.rows; ++i)
        {
            for (int j = 0; j < this->_matrixDims.cols; ++j)
            {
                this->_table[i * this->_matrixDims.cols + j] += other._table[i * other._matrixDims.cols + j];
            }
        }
        return *this;
    }
}

/**
 * place the other matrix values inside ours
 * @param other the other matrix
 * @return our matrix with the other values
 */
Matrix &Matrix::operator=(const Matrix &other)
{
    if (this == &other)
    {
        return *this; // so we wont delete our own table
    }
    delete[](this->_table); // delete the current info
    this->_matrixDims = other._matrixDims; // update the matrix dimensions
    this->_table = new float[other._matrixDims.rows * other._matrixDims.cols];
    std::memcpy(this->_table, other._table, _matrixDims.rows * _matrixDims.cols * sizeof(float));
    return *this;
}

/**
 * overload the () operator, to get the i,j cell inside the matrix
 * @param i the row index
 * @param j the column index
 * @return the i,j element inside the matrix
 */
float &Matrix::operator()(const int &i, const int &j) const
{
    if ((this->_matrixDims.rows <= i) || ((this->_matrixDims.cols <= j)) || i < FIRST_VAL || j < FIRST_VAL)
    {
        std::cerr << ERROR << std::endl;
        exit(FAILURE);
    }
    else
    {
        return this->_table[i * this->_matrixDims.cols + j];

    }
}

/**
 * overload the () operator, to get the i,j cell inside the matrix
 * @param i the row index
 * @param j the column index
 * @return the i,j element inside the matrix
 */
float &Matrix::operator()(const int &i, const int &j)
{
    if ((this->_matrixDims.rows <= i) || ((this->_matrixDims.cols <= j)) || i < FIRST_VAL || j < FIRST_VAL)
    {
        std::cerr << ERROR << std::endl;
        exit(FAILURE);
    }
    else
    {
        return this->_table[i * this->_matrixDims.cols + j];
    }
}

/**
 * overload the [] operator to return the i object in the matrix
 * @param idx the matrix idx
 * @return the ith object in teh matrix
 */
float &Matrix::operator[](int idx)
{
    if (this->_matrixDims.rows * this->_matrixDims.cols <= idx || idx < FIRST_VAL)
    {
        std::cerr << ERROR << std::endl;
        exit(FAILURE);
    }
    else
    {
        return this->_table[idx];
    }
}

/**
 * overload the [] operator to return the i object in the matrix
 * @param idx the matrix idx
 * @return the ith object in teh matrix
 */
float Matrix::operator[](int idx) const
{
    if (this->_matrixDims.rows * this->_matrixDims.cols <= idx || idx < FIRST_VAL)
    {
        std::cerr << ERROR << std::endl;
        exit(FAILURE);
    }
    else
    {
        return this->_table[idx];
    }
}

/**
 * overlaod the << operator, output the matrix values
 * @param output output stream
 * @param matrix the matrix we want output its values
 * @return the output stream
 */
std::ostream &operator<<(std::ostream &output, const Matrix &matrix)
{
    for (int i = 0; i < matrix._matrixDims.rows; ++i)
    {
        for (int j = 0; j < matrix._matrixDims.cols; ++j)
        {
            if (matrix(i, j) <= MIN_PROB)
            {
                output << "  ";
            }
            else
            {
                output << "**";
            }
        }
        output << std::endl;
    }
    return output;
}

/**
* overlaod the >> operator, input the matrix values
* @param input the input stream
* @param matrix the matrix we want to load values to
* @return the input stream
*/
std::istream &operator>>(std::istream &input, const Matrix &matrix)
{
    int idx = 0;
    while (input.good())
    {
        input.read(reinterpret_cast<char *>(&matrix._table[idx]), sizeof(float));
        idx++;
    }
    return input;
}


