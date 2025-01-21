#include "qubit.h"
#include <math.h>
#include <stdio.h>

/**
 * \brief Helper function to compute squared magnitude.
 */
static inline float magnitude_squared(float r, float i) {
    return r*r + i*i;
}

void init_qubit_zero(Qubit* q) {
    if (!q) return;
    // |0> = (1, 0); |1> = (0, 0)
    q->state[0].r = 1.0f; 
    q->state[0].i = 0.0f;
    q->state[1].r = 0.0f;
    q->state[1].i = 0.0f;
}

int init_qubit_state(Qubit* q, float alpha_r, float alpha_i, float beta_r, float beta_i) {
    if (!q) return -1;

    float norm = magnitude_squared(alpha_r, alpha_i) 
               + magnitude_squared(beta_r, beta_i);
    if (fabsf(norm - 1.0f) > 1e-5f) {
        fprintf(stderr, "Error: Qubit state not normalized (norm=%f)\n", norm);
        return -2;
    }

    q->state[0].r = alpha_r;
    q->state[0].i = alpha_i;
    q->state[1].r = beta_r;
    q->state[1].i = beta_i;
    return 0;
}

/* 
 * A basic test stub (optional). 
 * Compile with:
 *    gcc -o test_qubit qubit.c 
 * Then run `./test_qubit`.
 */
#ifdef TEST_QUBIT
#include <stdlib.h>

int main(void) {
    Qubit q;
    init_qubit_zero(&q);
    printf("Qubit after init_zero => |0>: (%f, %f), |1>: (%f, %f)\n",
           q.state[0].r, q.state[0].i, q.state[1].r, q.state[1].i);

    init_qubit_state(&q, 0.707f, 0.0f, 0.707f, 0.0f);
    printf("Qubit after init_state => |0>: (%f, %f), |1>: (%f, %f)\n",
           q.state[0].r, q.state[0].i, q.state[1].r, q.state[1].i);
    return 0;
}
#endif
