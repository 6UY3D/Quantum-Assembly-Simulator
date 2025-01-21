c


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Include core headers
#include "../core/qubit.h"
#include "../core/state_vector.h"
#include "../core/gate_operations.h"
#include "../core/measurement.h"

// Utility macro to assert approximate equality
#define ASSERT_FLOAT_CLOSE(a, b, tol) \
    do { \
        if (fabsf((a) - (b)) > (tol)) { \
            fprintf(stderr, "Assertion failed: %s != %s (within %.6f), got %f vs %f\n", #a, #b, tol, (float)(a), (float)(b)); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

static void test_qubit_init() {
    Qubit q;
    init_qubit_zero(&q);
    // Expect q.state[0] = (1,0) and q.state[1] = (0,0)
    ASSERT_FLOAT_CLOSE(q.state[0].r, 1.0f, 1e-6);
    ASSERT_FLOAT_CLOSE(q.state[1].r, 0.0f, 1e-6);

    // Try a normalized custom state
    int rc = init_qubit_state(&q, 0.707f, 0.0f, 0.707f, 0.0f);
    if (rc != 0) {
        fprintf(stderr, "init_qubit_state returned error for valid state.\n");
        exit(EXIT_FAILURE);
    }
}

static void test_state_vector_init() {
    StateVector sv;
    int rc = init_state_vector(&sv, 3); // 3 qubits => size 8
    if (rc != 0) {
        fprintf(stderr, "init_state_vector returned error.\n");
        exit(EXIT_FAILURE);
    }
    // Check the first amplitude is 1.0 + 0.0i => |000>
    ASSERT_FLOAT_CLOSE(sv.real[0], 1.0f, 1e-6);
    ASSERT_FLOAT_CLOSE(sv.imag[0], 0.0f, 1e-6);

    // All others should be 0
    for (size_t i = 1; i < 8; i++) {
        ASSERT_FLOAT_CLOSE(sv.real[i], 0.0f, 1e-6);
        ASSERT_FLOAT_CLOSE(sv.imag[i], 0.0f, 1e-6);
    }

    free_state_vector(&sv);
}

static void test_gate_operations() {
    StateVector sv;
    init_state_vector(&sv, 1); // single qubit

    // Gate X
    float x_gate[8] = {
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f
    };

    // Initial state = |0>, apply X => |1>
    apply_single_qubit_gate(&sv, x_gate, 0);
    // Now we expect amplitude(0) = 0, amplitude(1) = 1
    ASSERT_FLOAT_CLOSE(sv.real[0], 0.0f, 1e-6);
    ASSERT_FLOAT_CLOSE(sv.real[1], 1.0f, 1e-6);

    free_state_vector(&sv);
}

static void test_measurement() {
    srand((unsigned)time(NULL));

    StateVector sv;
    init_state_vector(&sv, 1); // single qubit => |0>

    int outcome = -1;
    measure_qubit(&sv, 0, &outcome);

    // Since it's initially |0>, we should get outcome=0 ~100% of the time 
    // (within floating random tolerance, but it's basically guaranteed for perfect code).
    if (outcome != 0) {
        fprintf(stderr, "Measurement test: expected 0, got %d\n", outcome);
        exit(EXIT_FAILURE);
    }

    free_state_vector(&sv);
}

int main(void) {
    printf("Running test_core...\n");
    test_qubit_init();
    test_state_vector_init();
    test_gate_operations();
    test_measurement();
    printf("All test_core tests passed!\n");
    return 0;
}
