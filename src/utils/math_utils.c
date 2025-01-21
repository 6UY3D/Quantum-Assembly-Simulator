#include "math_utils.h"
#include <math.h>
#include <stdio.h>

void complex_multiply(float a, float b, float c, float d, float* out_real, float* out_imag) {
    if (!out_real || !out_imag) return;
    *out_real = a*c - b*d;
    *out_imag = a*d + b*c;
}

int normalize_complex_array(float* real, float* imag, size_t length) {
    if (!real || !imag || length == 0) return -1;

    float sum_sq = 0.0f;
    for (size_t i = 0; i < length; i++) {
        float r = real[i];
        float im = imag[i];
        sum_sq += r*r + im*im;
    }
    if (sum_sq < 1e-12f) {
        // Avoid division by zero
        return -2;
    }
    float scale = 1.0f / sqrtf(sum_sq);
    for (size_t i = 0; i < length; i++) {
        real[i] *= scale;
        imag[i] *= scale;
    }
    return 0;
}
