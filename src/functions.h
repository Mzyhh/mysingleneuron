#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "globals.h"

typedef struct ActivationFn {
    elem (*f)(elem);
    elem (*deriv)(elem);
} ActivationFn;

ActivationFn sigmoid_fn();

ActivationFn relu_fn();

ActivationFn tanh_fn();

ActivationFn sinh_fn();

ActivationFn id_fn();

#endif // _FUNCTION_H