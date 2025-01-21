#include "state_vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int init_state_vector(StateVector* sv, size_t num_qubits) {
    if (!sv) return -1;
    sv->num_qubits = num_qubits;

    size_t length = ((size_t)1 << num_qubits);
    sv->real = (float*)aligned_alloc(32, length * sizeof(float));
    sv->imag = (float*)aligned_alloc(32, length * sizeof(float));
    if (!sv->real || !sv->imag) {
        free(sv->real);
        free(sv->imag);
        return -2;
    }

    // Initialize all amplitudes to 0
    memset(sv->real, 0, length * sizeof(float));
    memset(sv->imag, 0, length * sizeof(float));

    // Set the first amplitude to 1 => |0...0>
    sv->real[0] = 1.0f;
    return 0;
}

void free_state_vector(StateVector* sv) {
    if (!sv) return;
    if (sv->real) free(sv->real);
    if (sv->imag) free(sv->imag);
    sv->real = NULL;
    sv->imag = NULL;
    sv->num_qubits = 0;
}

void print_state_vector(const StateVector* sv, size_t max_entries) {
    if (!sv) return;
    size_t length = (size_t)1 << sv->num_qubits;
    printf("StateVector (%zu qubits, total size = %zu):\n", sv->num_qubits, length);

    size_t to_print = (max_entries < length) ? max_entries : length;
    for (size_t i = 0; i < to_print; i++) {
        float r = sv->real[i];
        float im = sv->imag[i];
        printf("Index %zu: (%f, %f)\n", i, r, im);
    }
    if (to_print < length) {
        printf("... (truncated)\n");
    }
}

/* 
 * Basic test stub (optional).
 * Compile with:
 *    gcc -o test_state_vector state_vector.c
 * Then run `./test_state_vector`.
 */
#ifdef TEST_STATE_VECTOR
int main(void) {
    StateVector sv;
    if (init_state_vector(&sv, 3) == 0) {
        print_state_vector(&sv, 8);
        free_state_vector(&sv);
    } else {
        printf("Error initializing state vector.\n");
    }
    return 0;
}
#endif
