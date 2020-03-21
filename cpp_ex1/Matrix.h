// Matrix.h
#include <iostream>

#ifndef MATRIX_H
#define MATRIX_H
#define EMPTY_MATRIX 0
#define MIN_MATRIX_DIM 1
#define ERROR "ERROR: invalid input"
#define FAILURE 1
#define MIN_PROB 0.1f
#define FIRST_VAL 0
/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

/**
 * Matrix class
 */
class Matrix
{
public:
    /**
     * empty const, const 1x1 matrix, init the single ele to 0
     */
    Matrix(); // default
    /**
     * dest for Matrix object
     */
    ~Matrix();

    /**
     * const for Matrix
     * @param rows the number of rows in the matrix
     * @param cols the number of cols in the matrix
     */
    Matrix(int rows, int cols);

    /**
     * const matrix from anther matrix m
     * @param m the other matrix
     */
    Matrix(const Matrix &m); // copy constructor
    //METHODS:
    /**
     * getter for the rows
     * @return number of rows
     */
    int getRows();

    /**
     * getter for the rows
     * @return number of rows
     */
    int getRows() const;

    /**
     * gettter for the cols
     * @return number of cols
     */
    int getCols();

    /**
     * gettter for the cols
     * @return number of cols
     */
    int getCols() const;

    /**
     * Transforms a matrix into a coloumn vector Supports function calling concatenation i.e.(1)
     * Matrix m(5,4);... m.vectorize(), then m.vectorize() + b should be a valid expression
     */
    Matrix vectorize();

    /**
     * Prints matrix elements, no return value. prints space after each element (incl. last element in the row)
     * prints newline after each row (incl. last row)
     */
    void plainPrint() const;

    // OPERATORS:
    /**
 *  overload the * operator to multiply matrix and scalar
 * @param scalar the scalar we want to multiply the matrix by
 * @param matrix the matrix
 * @return scalar * matrix
 */
    friend Matrix operator*(const float &scalar, const Matrix &matrix);

    /**
 *  overload the * operator to multiply matrix and scalar
 * @param scalar the scalar we want to multiply the matrix by
 * @return scalar * matrix
 */
    Matrix operator*(const float &scalar) const;

    /**
     * overload the * operator, to multiply with other matrix
     * @param other the other matrix
     * @return the solution of 2 matrix multiplication
     */
    Matrix operator*(const Matrix &other) const;


    /**
     * overload the () operator, to get the i,j cell inside the matrix
     * @param i the row index
     * @param j the column index
     * @return the i,j element inside the matrix
     */
    float &operator()(const int &i, const int &j) const;

    /**
     * overload the () operator, to get the i,j cell inside the matrix
     * @param i the row index
     * @param j the column index
     * @return the i,j element inside the matrix
     */
    float &operator()(const int &i, const int &j);

    /**
     * overload the + operator to add our matrix with our matrix
     * @param other the other matrix we want to add to our
     * @return the addition matrix
     */
    Matrix operator+(const Matrix &other) const;

    /**
     * overload the + operator to add our matrix with our matrix
     * @param other the other matrix we want to add to our
     * @return our matrix += the other matrix values
     */
    Matrix &operator+=(const Matrix &other);

    /**
     * place the other matrix values inside ours
     * @param other the other matrix
     * @return our matrix with the other values
     */
    Matrix &operator=(const Matrix &other);

    /**
     * overload the [] operator to return the i object in the matrix
     * @param idx the matrix idx
     * @return the ith object in teh matrix
     */
    float &operator[](int idx);

    /**
     * overload the [] operator to return the i object in the matrix
     * @param idx the matrix idx
     * @return the ith object in teh matrix
     */
    float operator[](int idx) const;

/**
 * overlaod the << operator, output the matrix values
 * @param output output stream
 * @param matrix the matrix we want output its values
 * @return the output stream
 */
    friend std::ostream &operator<<(std::ostream &output, const Matrix &matrix);

/**
* overlaod the >> operator, input the matrix values
* @param input the input stream
* @param matrix the matrix we want to load values to
* @return the input stream
*/
    friend std::istream &operator>>(std::istream &input, const Matrix &matrix);

private:
    MatrixDims _matrixDims;
    float *_table{};
};

#endif //MATRIX_H
