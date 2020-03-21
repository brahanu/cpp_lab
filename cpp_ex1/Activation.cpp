#include <cmath>
#include "Activation.h"

/**
 * constructor for the project
 * @param actType the activationtype
 */
Activation::Activation(ActivationType actType)
{
    this->activationType = actType;
}

/**
 * getter for teh activationtype
 * @return  the activation type
 */
ActivationType Activation::getActivationType()
{
    return this->activationType;
}

/**
 * overload the () function, activate the activation type function on the input matrix
 * @param input the input matrix
 * @return matrix after the () operator
 */
Matrix Activation::operator()(const Matrix &input)
{
    if (this->getActivationType() == Relu)
    {
        return _reluActivation(input);
    }
    return _softMaxActivation(input);
}

/**
 * overload the () function, activate the activation type function on the input matrix
 * @param input the input matrix
 * @return matrix after the () operator
 */
Matrix Activation::operator()(const Matrix &input) const
{
    if (this->activationType == Relu)
    {
        return _reluActivation(input);
    }
    return _softMaxActivation(input);
}

/**
 * relu activation function
 * @param matrix  the input matrix
 * @return matrix after the activation function
 */
Matrix Activation::_reluActivation(const Matrix &matrix)
{
    int size = matrix.getRows() * matrix.getCols();
    Matrix out = Matrix(matrix);
    for (int i = 0; i < size; ++i)
    {
        if (out[i] < MIN_RELU_VAL)
        {
            out[i] = MIN_RELU_VAL;
        }
    }
    return out;
}

/**
 * softmax activation function
 * @param matrix  the input matrix
 * @return matrix after the activation function
 */
Matrix Activation::_softMaxActivation(const Matrix &matrix)
{
    int size = matrix.getRows() * matrix.getCols();
    Matrix out = Matrix(matrix);
    float sum = 0;
    for (int i = 0; i < size; ++i)
    {
        out[i] = std::exp(out[i]);
        sum += out[i];
    }
    float scalar = (FLOAT_NORM / sum);
    return scalar * out;
}



