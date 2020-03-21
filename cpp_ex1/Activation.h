//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

#define FLOAT_NORM 1.f
#define MIN_RELU_VAL 0
/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

/**
 * Activation class
 */
class Activation
{
public:
    /**
     * default constructor
     */
    Activation() = default;

    /**
     * constructor for the project
     * @param actType the activationtype
     */
    explicit Activation(ActivationType actType);

    /**
     * getter for teh activationtype
     * @return  the activation type
     */
    ActivationType getActivationType();

    /**
     * overload the () function, activate the activation type function on the input matrix
     * @param input the input matrix
     * @return matrix after the () operator
     */
    Matrix operator()(const Matrix &input);

    /**
     * overload the () function, activate the activation type function on the input matrix
     * @param input the input matrix
     * @return matrix after the () operator
     */
    Matrix operator()(const Matrix &input) const;

    /**
     * default destructor
     */
    ~Activation() = default;

private:
    /**
     * data member the activation type
     */
    ActivationType activationType;

    /**
     * relu activation function
     * @param matrix  the input matrix
     * @return matrix after the activation function
     */
    static Matrix _reluActivation(const Matrix &matrix);

    /**
     * softmax activation function
     * @param matrix  the input matrix
     * @return matrix after the activation function
     */
    static Matrix _softMaxActivation(const Matrix &matrix);
};

#endif //ACTIVATION_H
