c

#include "measurement.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/**
 * \brief Utility function to compute probability of qubit=0 or qubit=1
 * \return Probability that the qubit is 0 or 1
 */
static float measure_probability(const StateVector* sv, size_t qubit_index, int outcome) {
    float prob = 0.0f;
    size_t length = (size_t)1 << sv->num_qubits;
    size_t mask = (size_t)1 << qubit_index;

    for (size_t i = 0; i < length; i++) {
        int bit = (i & mask) ? 1 : 0;
        if (bit == outcome) {
            float r = sv->real[i];
            float im = sv->imag[i];
            prob += r*r + im*im;
        }
    }
    return prob;
}

/**
 * \brief Collapses the state vector after measurement. 
 *        If result=0, keep only amplitudes where qubit_index=0; otherwise keep qubit_index=1.
 */
static void collapse(StateVector* sv, size_t qubit_index, int result) {
    size_t length = (size_t)1 << sv->num_qubits;
    size_t mask = (size_t)1 << qubit_index;

    for (size_t i = 0; i < length; i++) {
        int bit = (i & mask) ? 1 : 0;
        if (bit != result) {
            sv->real[i] = 0.0f;
            sv->imag[i] = 0.0f;
        }
    }
}

/**
 * \brief Normalizes the state vector after collapse.
 */
static void normalize(StateVector* sv) {
    size_t length = (size_t)1 << sv->num_qubits;
    float norm = 0.0f;
    for (size_t i = 0; i < length; i++) {
        float r = sv->real[i];
        float im = sv->imag[i];
        norm += r*r + im*im;
    }
    if (norm < 1e-12f) {
        // State collapsed to zero vector? This is theoretically impossible
        // if measurement was done correctly, but handle numeric issues anyway.
        return; 
    }
    float scale = 1.0f / sqrtf(norm);
    for (size_t i = 0; i < length; i++) {
        sv->real[i] *= scale;
        sv->imag[i] *= scale;
    }
}

int measure_qubit(StateVector* sv, size_t qubit_index, int* out_result) {
    if (!sv || !out_result) return -1;
    if (qubit_index >= sv->num_qubits) return -2;

    // Probability that qubit_index is 0
    float p0 = measure_probability(sv, qubit_index, 0);

    // Generate random number to decide measurement outcome
    float rand_val = (float)rand()/(float)RAND_MAX;
    int outcome = (rand_val < p0) ? 0 : 1;

    // Collapse the state vector
    collapse(sv, qubit_index, outcome);
    normalize(sv);

    // Save outcome
    *out_result = outcome;
    return 0;
}

/*
 * Basic test stub (optional).
 * Compile with:
 *   gcc -o test_measure measurement.c state_vector.c gate_operations.c
 * Then run `./test_measure`.
 */
#ifdef TEST_MEASUREMENT
#include <time.h>

int main(void) {
    srand((unsigned)time(NULL));

    StateVector sv;
    init_state_vector(&sv, 1); // 1 qubit => size 2
    // The state is already |0>, so p0 = 1, p1 = 0

    int result = -1;
    measure_qubit(&sv, 0, &result);
    printf("Measurement result: %d\n", result);
    // Expect mostly 0 given |0> state.

    free_state_vector(&sv);
    return 0;
}
#endif
