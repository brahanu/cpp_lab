//
// Created by brahan on 19/12/2019.
//

#ifndef CPP_EX1_DENSE_H
#define CPP_EX1_DENSE_H

#include "Matrix.h"
#include "Activation.h"

/**
 * Dense class
 */
class Dense
{
public:
    /**
     * def constructor
     */
    Dense() = default;

    /**
     * Dense constructor
     * @param w weight matrix
     * @param bias bias matrix
     * @param actType activation type
     */
    Dense(Matrix &w, Matrix &bias, ActivationType actType);

    /**
     * def destructor
     */
    ~Dense() = default;

    /**
     * getter for the weights matrix
     * @return the weight matrix
     */
    Matrix &getWeights();

    /**
     * getter for the weights matrix
     * @return the weight matrix
     */
    Matrix getWeights() const;

    /**
     * getter for teh bias matrix
     * @return the bias matrix
     */
    Matrix &getBias();

    /**
     * getter for teh bias matrix
     * @return the bias matrix
     */
    Matrix getBias() const;


    /**
     * getter for the activation data member
     * @return the activation
     */
    Activation &getActivation();

    /**
     * getter for the activation data member
     * @return the activation
     */
    Activation getActivation() const;

    /**
     * overload the () operator
     * @param input , activate the layer on teh input matrix
     * @return matrix after the layer process
     */
    Matrix operator()(Matrix &input);

    /**
     * overload the () operator
     * @param input , activate the layer on teh input matrix
     * @return matrix after the layer process
     */
    Matrix operator()(Matrix &input) const;


private:
    Matrix _weights, _bias;
    Activation _activation;
};

#endif //CPP_EX1_DENSE_H

