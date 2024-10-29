#ifndef _MODEL_H
#define _MODEL_H

#include "matrix.h"
#include "functions.h"

typedef struct DenseLayer {
    matrix w;
    matrix b;
    ActivationFn f;
} DenseLayer;

typedef struct SequentialModel {
    size_t nlayers;
    DenseLayer *layers;
} SequentialModel;

matrix forward(SequentialModel);

#endif // _MODEL_H