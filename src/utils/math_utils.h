c

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * \brief Multiply two complex numbers (a + i*b) * (c + i*d) = (a*c - b*d) + i(a*d + b*c).
 * \param a Real part of first operand
 * \param b Imag part of first operand
 * \param c Real part of second operand
 * \param d Imag part of second operand
 * \param out_real Pointer to store the real part of result
 * \param out_imag Pointer to store the imag part of result
 */
void complex_multiply(float a, float b, float c, float d, float* out_real, float* out_imag);

/**
 * \brief Normalizes an array of complex amplitudes (real[] + i imag[]) by dividing each element by the sqrt of total magnitude.
 * \param real Array of real parts
 * \param imag Array of imaginary parts
 * \param length Length of the arrays
 * \return 0 on success, nonzero on error (e.g. zero-norm)
 */
int normalize_complex_array(float* real, float* imag, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* MATH_UTILS_H */
