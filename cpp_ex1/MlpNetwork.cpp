#include "MlpNetwork.h"

/**
* mlp constructor
* @param weights the weights
* @param biases the biases
*/
MlpNetwork::MlpNetwork(Matrix *weights, Matrix *biases)
{
    this->_d1 = Dense(weights[FIRST_LAYER], biases[FIRST_LAYER], Relu);
    this->_d2 = Dense(weights[SEC_LAYER], biases[SEC_LAYER], Relu);
    this->_d3 = Dense(weights[THIRD_LAYER], biases[THIRD_LAYER], Relu);
    this->_d4 = Dense(weights[FOURTH_LAYER], biases[FOURTH_LAYER], Softmax);
}

/**
* overload () operator, activate the mlp process on the input matrix
* @param matrix the input matrix
* @return matrix after the mlp process
*/
Digit MlpNetwork::operator()(const Matrix &matrix)
{
    Matrix m = Matrix(matrix);
    m = _d1(m);
    m = _d2(m);
    m = _d3(m);
    m = _d4(m);
    float prob = NO_PROB;
    int idx = NO_IDX;
    for (int i = 0; i < m.getRows(); ++i)
    {
        if (m[i] > prob)
        {
            prob = m[i];
            idx = i;
        }
    }
    Digit digit;
    digit.probability = prob;
    digit.value = idx;
    return digit;
}
