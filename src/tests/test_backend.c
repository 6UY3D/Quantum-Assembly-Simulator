#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Include backend headers
#include "../backend/circuit_optimizer.h"
#include "../backend/parallel_execution.h"
#include "../backend/memory_management.h"

// Include assembly for InstructionList
#include "../assembly/parser.h"
#include "../assembly/lexer.h"

// Include core for state vector ops
#include "../core/state_vector.h"
#include "../core/gate_operations.h"
#include "../core/measurement.h"

static void test_circuit_optimizer() {
    // We'll create a small instruction list with redundant gates, 
    // then run optimize_circuit and check the final list.

    TokenList token_list;
    init_token_list(&token_list);

    // "X 0", "X 0" => should become identity (removed)
    lex_line("X 0", &token_list);
    lex_line("X 0", &token_list);

    // "Y 1", "Z 0", "Y 1" => Y1 + Y1 => identity (on qubit 1), leaving "Z 0"
    lex_line("Y 1", &token_list);
    lex_line("Z 0", &token_list);
    lex_line("Y 1", &token_list);

    // Parse
    InstructionList instr_list;
    init_instruction_list(&instr_list);
    parse_tokens(&token_list, &instr_list);

    // Expect 5 instructions before optimization
    if (instr_list.size != 5) {
        fprintf(stderr, "test_circuit_optimizer: setup mismatch.\n");
        exit(EXIT_FAILURE);
    }

    // Optimize
    optimize_circuit(&instr_list);

    // Expected outcome:
    // X0, X0 => removed
    // Y1, Z0, Y1 => Y1, Y1 => removed, leaving Z0
    // So final list should have 1 instruction => "Z 0"
    if (instr_list.size != 1) {
        fprintf(stderr, "test_circuit_optimizer: size mismatch after optimization, got %zu.\n", instr_list.size);
        exit(EXIT_FAILURE);
    }
    if (strcasecmp(instr_list.data[0].gate_name, "Z") != 0 ||
        instr_list.data[0].qubits[0] != 0) {
        fprintf(stderr, "test_circuit_optimizer: wrong instruction after optimization.\n");
        exit(EXIT_FAILURE);
    }

    free_token_list(&token_list);
    free_instruction_list(&instr_list);
}

static void test_parallel_execution() {
    // We'll apply a single-qubit gate in parallel and compare results 
    // to a single-threaded approach.

    StateVector sv_parallel, sv_single;
    init_state_vector(&sv_parallel, 3);  // 3 qubits => 8 amplitudes
    init_state_vector(&sv_single, 3);

    // Create a random gate for demonstration or just reuse X
    float x_gate[8] = {
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f
    };

    // Apply in single-thread
    apply_single_qubit_gate(&sv_single, x_gate, 1);

    // Apply in parallel
    parallel_apply_single_qubit_gate(&sv_parallel, x_gate, 1, 4 /* threads */);

    // Compare
    size_t len = (size_t)(1 << sv_single.num_qubits);
    float eps = 1e-6f;
    for (size_t i = 0; i < len; i++) {
        if (fabsf(sv_single.real[i] - sv_parallel.real[i]) > eps ||
            fabsf(sv_single.imag[i] - sv_parallel.imag[i]) > eps) {
            fprintf(stderr, "test_parallel_execution mismatch at index %zu.\n", i);
            exit(EXIT_FAILURE);
        }
    }

    free_state_vector(&sv_parallel);
    free_state_vector(&sv_single);
}

static void test_memory_management() {
    // Just confirm aligned_malloc and aligned_free work without crashing 
    // and produce valid alignment
    const size_t alignment = 32;
    const size_t size = 1024; // bytes

    void* ptr = aligned_malloc(size, alignment);
    if (!ptr) {
        fprintf(stderr, "test_memory_management: aligned_malloc returned NULL.\n");
        exit(EXIT_FAILURE);
    }

    // Check alignment
    if (((uintptr_t)ptr % alignment) != 0) {
        fprintf(stderr, "test_memory_management: pointer is not %zu-byte aligned.\n", alignment);
        exit(EXIT_FAILURE);
    }

    // Fill it
    memset(ptr, 0xAA, size);

    aligned_free(ptr);
}

int main(void) {
    printf("Running test_backend...\n");
    test_circuit_optimizer();
    test_parallel_execution();
    test_memory_management();
    printf("All test_backend tests passed!\n");
    return 0;
}
