#include "functions.h"
#include <math.h>

elem sigmoid(const elem x)
{
    return 1.0 / (1.0 + exp(-x));
}

elem sigmoid_deriv(const elem x)
{
    return exp(-x) / pow(1.0 + exp(-x), 2);
}

elem relu(const elem x)
{
    return x > 0.0 ? x : 0.0;
}
elem relu_deriv(const elem x)
{
    return x >= 0.0;
}

elem tanh(const elem x)
{
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

elem tanh_deriv(const elem x)
{
    return 1.0 / pow(exp(x) + exp(-x), 2);
}

elem sinh(const elem x)
{
    return (exp(x) - exp(-x)) / 2;
}
elem sinh_deriv(const elem x)
{
    return (exp(x) + exp(x)) / 2;
}

elem id(const elem x)
{
    return x;
}
elem id_deriv(const elem x)
{
    return 1.0;
}


ActivationFn sigmoid_fn()
{
    ActivationFn result = { sigmoid, sigmoid_deriv };
    return result;
}

ActivationFn relu_fn()
{
    ActivationFn result = { relu, relu_deriv };
    return result;
}

ActivationFn tanh_fn()
{
    ActivationFn result = { tanh, tanh_deriv };
    return result;
}

ActivationFn sinh_fn()
{
    ActivationFn result = { sinh, sinh_deriv };
    return result;
}

ActivationFn id_fn()
{
    ActivationFn result = { id, id_deriv };
    return result;
}