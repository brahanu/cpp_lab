//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4
#define FIRST_LAYER 0
#define SEC_LAYER 1
#define THIRD_LAYER 2
#define FOURTH_LAYER 3
#define NO_PROB 0.0
#define NO_IDX 0

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
                                  {64,  128},
                                  {20,  64},
                                  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
                               {64,  1},
                               {20,  1},
                               {10,  1}};

/**
 * MlpNetwork
 */
class MlpNetwork
{
public:
    /**
     * def constructor
     */
    MlpNetwork() = default; // default const
    /**
     * def destructor
     */
    ~MlpNetwork() = default; // default dest
    /**
     * mlp constructor
     * @param weights the weights
     * @param biases the biases
     */
    MlpNetwork(Matrix *weights, Matrix *biases);

    /**
     * overload () operator, activate the mlp process on the input matrix
     * @param matrix the input matrix
     * @return matrix after the mlp process
     */
    Digit operator()(const Matrix &matrix);


private:
    Dense _d1, _d2, _d3, _d4;
};

#endif // MLPNETWORK_H
