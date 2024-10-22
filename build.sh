#!/bin/sh

set -xe

#clang -Wall -Wextra -o gate gate.c -lm
#clang -Wall -Wextra -o xor xor.c -lm
clang -Wall -Wextra -o matrix matrix.c
