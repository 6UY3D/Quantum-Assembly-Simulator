#include "gate_operations.h"
#include <stdio.h>
#include <math.h>
#include <immintrin.h>  // SSE/AVX intrinsics
#include <stdint.h>

static inline void complex_mul_accumulate_sse(
    float* __restrict out_real,
    float* __restrict out_imag,
    float a_r, float a_i, 
    float b_r, float b_i)
{
    // out += a * b (complex multiply)
    // out_real += a_r * b_r - a_i * b_i
    // out_imag += a_r * b_i + a_i * b_r

    asm volatile(
        // Load out_real, out_imag into SSE registers
        "movss  (%%rdi), %%xmm0       \n\t"  // xmm0 = out_real
        "movss  (%%rsi), %%xmm1       \n\t"  // xmm1 = out_imag

        // a_r * b_r
        "movss  %[ar], %%xmm2        \n\t"
        "mulss  %[br], %%xmm2        \n\t"

        // a_i * b_i
        "movss  %[ai], %%xmm3        \n\t"
        "mulss  %[bi], %%xmm3        \n\t"

        // out_real += a_r*b_r - a_i*b_i
        "subss  %%xmm3, %%xmm2       \n\t"
        "addss  %%xmm2, %%xmm0       \n\t"

        // a_r * b_i
        "movss  %[ar], %%xmm4        \n\t"
        "mulss  %[bi], %%xmm4        \n\t"

        // a_i * b_r
        "movss  %[ai], %%xmm5        \n\t"
        "mulss  %[br], %%xmm5        \n\t"

        // out_imag += a_r*b_i + a_i*b_r
        "addss  %%xmm5, %%xmm4       \n\t"
        "addss  %%xmm4, %%xmm1       \n\t"

        // Store back to memory
        "movss  %%xmm0, (%%rdi)       \n\t"
        "movss  %%xmm1, (%%rsi)       \n\t"
        :
        : [ar]"m"(a_r), [ai]"m"(a_i),
          [br]"m"(b_r), [bi]"m"(b_i),
          "D"(out_real), "S"(out_imag)
        : "xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","memory"
    );
}

static inline void apply_single_qubit_2x2(StateVector* sv, const float* gate, size_t qubit_index) {
    // gate = [r00, i00, r01, i01, r10, i10, r11, i11]
    // partial transform for each pair of amplitudes that differ in qubit_index
    size_t length = (size_t)1 << sv->num_qubits;
    size_t block_size = (size_t)1 << qubit_index;

    // For each block of size 2^(qubit_index+1), we update pairs (|...0...>, |...1...>)
    for (size_t start = 0; start < length; start += (block_size << 1)) {
        for (size_t offset = 0; offset < block_size; offset++) {
            size_t i0 = start + offset;
            size_t i1 = i0 + block_size;

            float r0 = sv->real[i0];
            float i0r = sv->imag[i0];
            float r1 = sv->real[i1];
            float i1r = sv->imag[i1];

            // Apply 2x2 matrix multiplication in-place:
            // new_amp(i0) = gate[0,0]*amp(i0) + gate[0,1]*amp(i1)
            // new_amp(i1) = gate[1,0]*amp(i0) + gate[1,1]*amp(i1)
            // We can do manual expansions or use an SSE approach

            float new_r0 = gate[0] * r0 - gate[1] * i0r 
                         + gate[2] * r1 - gate[3] * i1r;
            float new_i0 = gate[0] * i0r + gate[1] * r0 
                         + gate[2] * i1r + gate[3] * r1;

            float new_r1 = gate[4] * r0 - gate[5] * i0r 
                         + gate[6] * r1 - gate[7] * i1r;
            float new_i1 = gate[4] * i0r + gate[5] * r0 
                         + gate[6] * i1r + gate[7] * r1;

            sv->real[i0] = new_r0;
            sv->imag[i0] = new_i0;
            sv->real[i1] = new_r1;
            sv->imag[i1] = new_i1;
        }
    }
}

int apply_single_qubit_gate(StateVector* sv, const float* gate, size_t qubit_index) {
    if (!sv || !gate) return -1;
    if (qubit_index >= sv->num_qubits) return -2;
    apply_single_qubit_2x2(sv, gate, qubit_index);
    return 0;
}

/**
 * \brief Helper function for applying CNOT logic:
 *        If control qubit is 1, flip target qubit.
 */
int apply_cnot(StateVector* sv, size_t control_qubit, size_t target_qubit) {
    if (!sv) return -1;
    if (control_qubit >= sv->num_qubits || target_qubit >= sv->num_qubits) return -2;
    if (control_qubit == target_qubit) return -3; // not valid for standard CNOT

    size_t length = (size_t)1 << sv->num_qubits;

    // We iterate over the entire state vector, flipping amplitudes of target_qubit
    // only when control_qubit is set to 1.
    // That means we check bit pattern of the index.
    // If the bit at control_qubit == 1, we swap the amplitudes at target_qubit = 0 vs 1.

    // For each index i in [0..2^n - 1], if i has control_qubit=1,
    // we swap (i with i^(1<<target_qubit)) => toggling target_qubit in the index.

    // We do each swap only once, so only process i if i < i ^ (1<<target_qubit) to avoid double swap.
    size_t control_mask = (size_t)1 << control_qubit;
    size_t target_mask  = (size_t)1 << target_qubit;

    for (size_t i = 0; i < length; i++) {
        if ((i & control_mask) != 0) {
            size_t j = i ^ target_mask;
            if (j > i) {
                float tmp_r = sv->real[i];
                float tmp_i = sv->imag[i];
                sv->real[i] = sv->real[j];
                sv->imag[i] = sv->imag[j];
                sv->real[j] = tmp_r;
                sv->imag[j] = tmp_i;
            }
        }
    }

    return 0;
}

/*
 * Basic test stub (optional). 
 * Compile with:
 *   gcc -o test_gate gate_operations.c state_vector.c
 * Then run `./test_gate`.
 */
#ifdef TEST_GATE_OPERATIONS
#include <stdlib.h>

int main(void) {
    StateVector sv;
    init_state_vector(&sv, 2); // 2 qubits => size 4

    float hadamard[8] = {
        // Gate (H) = 1/sqrt(2) * [ [1,  1], [1, -1] ]
        // real(0,0), imag(0,0), real(0,1), imag(0,1), ...
        0.707f, 0.0f,  0.707f, 0.0f,
        0.707f, 0.0f, -0.707f, 0.0f
    };

    printf("Before gate:\n");
    print_state_vector(&sv, 4);
    apply_single_qubit_gate(&sv, hadamard, 0);
    printf("After applying H on qubit 0:\n");
    print_state_vector(&sv, 4);

    apply_cnot(&sv, 0, 1);
    printf("After CNOT (control=0, target=1):\n");
    print_state_vector(&sv, 4);

    free_state_vector(&sv);
    return 0;
}
#endif
