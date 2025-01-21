c

#ifndef QUBIT_H
#define QUBIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * \brief A simple complex number struct for single-qubit usage or other utilities.
 *        In multi-qubit simulations, we often rely on the global state vector instead.
 */
typedef struct Complex {
    float r;  /**< Real part */
    float i;  /**< Imag part */
} Complex;

/**
 * \brief Single qubit struct demonstrating local usage. 
 *        Production multi-qubit usage is handled via state_vector.h
 */
typedef struct Qubit {
    Complex state[2];
} Qubit;

/**
 * \brief Initializes a Qubit in the |0> state.
 * \param q Pointer to a Qubit
 */
void init_qubit_zero(Qubit* q);

/**
 * \brief Initializes a Qubit in a user-specified state (normalized).
 * \param q Pointer to a Qubit
 * \param alpha_r Real part of amplitude for |0>
 * \param alpha_i Imag part of amplitude for |0>
 * \param beta_r  Real part of amplitude for |1>
 * \param beta_i  Imag part of amplitude for |1>
 * \return 0 on success, nonzero on error (e.g., normalization error)
 */
int init_qubit_state(Qubit* q, float alpha_r, float alpha_i, float beta_r, float beta_i);

#ifdef __cplusplus
}
#endif

#endif /* QUBIT_H */
