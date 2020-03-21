
#include "Dense.h"

/**
 * Dense constructor
 * @param w weight matrix
 * @param bias bias matrix
 * @param actType activation type
 */
Dense::Dense(Matrix &w, Matrix &bias, ActivationType actType) :
        _weights(w), _bias(bias), _activation(Activation(actType))
{
}

/**
 * getter for the weights matrix
 * @return the weight matrix
 */
Matrix &Dense::getWeights()
{
    return this->_weights;
}

/**
 * getter for teh bias matrix
 * @return the bias matrix
 */
Matrix &Dense::getBias()
{
    return this->_bias;
}

/**
 * getter for the activation data member
 * @return the activation
 */
Activation &Dense::getActivation()
{
    return this->_activation;
}

/**
 * getter for the weights matrix
 * @return the weight matrix
 */
Matrix Dense::getWeights() const
{
    return this->_weights;
}

/**
 * getter for teh bias matrix
 * @return the bias matrix
 */
Matrix Dense::getBias() const
{
    return this->_bias;
}

/**
 * getter for the activation data member
 * @return the activation
 */
Activation Dense::getActivation() const
{
    return this->_activation;
}

/**
 * overload the () operator
 * @param input , activate the layer on teh input matrix
 * @return matrix after the layer process
 */
Matrix Dense::operator()(Matrix &input)
{
    Matrix out = (this->_weights * input) + this->_bias;
    out = this->_activation(out);
    return out;
}

/**
 * overload the () operator
 * @param input , activate the layer on teh input matrix
 * @return matrix after the layer process
 */
Matrix Dense::operator()(Matrix &input) const
{
    Matrix out = (this->_weights * input) + this->_bias;
    out = this->_activation(out);
    return out;
}
