#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include assembly headers
#include "../assembly/lexer.h"
#include "../assembly/parser.h"
#include "../assembly/interpreter.h"
#include "../core/state_vector.h"

static void test_lexer() {
    TokenList token_list;
    init_token_list(&token_list);

    const char* line = "H 0  // A test line";
    lex_line(line, &token_list);

    // We expect: [TOKEN_GATE: "H"], [TOKEN_INTEGER: "0"], [TOKEN_COMMENT: "// A test line"]
    if (token_list.size != 3) {
        fprintf(stderr, "test_lexer failed. Expected 3 tokens, got %zu.\n", token_list.size);
        exit(EXIT_FAILURE);
    }
    if (token_list.data[0].type != TOKEN_GATE ||
        token_list.data[1].type != TOKEN_INTEGER ||
        token_list.data[2].type != TOKEN_COMMENT) {
        fprintf(stderr, "test_lexer token types mismatch.\n");
        exit(EXIT_FAILURE);
    }

    free_token_list(&token_list);
}

static void test_parser() {
    TokenList token_list;
    init_token_list(&token_list);

    // Example lines representing a small program
    lex_line("H 0", &token_list);
    lex_line("CNOT 0 1", &token_list);
    lex_line("MEASURE 1", &token_list);

    InstructionList instr_list;
    init_instruction_list(&instr_list);
    parse_tokens(&token_list, &instr_list);

    if (instr_list.size != 3) {
        fprintf(stderr, "test_parser failed. Expected 3 instructions, got %zu.\n", instr_list.size);
        exit(EXIT_FAILURE);
    }

    // Check the first instruction (H 0)
    if (strcmp(instr_list.data[0].gate_name, "H") != 0 ||
        instr_list.data[0].qubits[0] != 0) {
        fprintf(stderr, "test_parser mismatch in first instruction.\n");
        exit(EXIT_FAILURE);
    }

    // Check the second (CNOT 0 1)
    if (strcmp(instr_list.data[1].gate_name, "CNOT") != 0 ||
        instr_list.data[1].qubits[0] != 0 ||
        instr_list.data[1].qubits[1] != 1) {
        fprintf(stderr, "test_parser mismatch in second instruction.\n");
        exit(EXIT_FAILURE);
    }

    // Check the third (MEASURE 1)
    if (strcmp(instr_list.data[2].gate_name, "MEASURE") != 0 ||
        instr_list.data[2].qubits[0] != 1) {
        fprintf(stderr, "test_parser mismatch in third instruction.\n");
        exit(EXIT_FAILURE);
    }

    free_token_list(&token_list);
    free_instruction_list(&instr_list);
}

static void test_interpreter() {
    // We'll create tokens for a small circuit and interpret it on a 2-qubit state vector.
    TokenList token_list;
    init_token_list(&token_list);

    lex_line("H 0", &token_list);
    lex_line("CNOT 0 1", &token_list);

    InstructionList instr_list;
    init_instruction_list(&instr_list);
    parse_tokens(&token_list, &instr_list);

    // Prepare state vector
    StateVector sv;
    init_state_vector(&sv, 2); // 2 qubits => |00>

    interpret_instructions(&instr_list, &sv);

    // After H 0 => (1/sqrt(2))(|00> + |10>)
    // After CNOT => (1/sqrt(2))(|00> + |11>)
    // Let's check amplitudes: index 0 => |00>, index 1 => |01>, index 2 => |10>, index 3 => |11>
    // We expect real[0] = 1/sqrt(2), real[3] = 1/sqrt(2), others=0
    float eps = 1e-4f;
    float expected = 0.7071f;
    if (fabsf(sv.real[0] - expected) > eps || fabsf(sv.real[3] - expected) > eps) {
        fprintf(stderr, "test_interpreter mismatch in final amplitudes.\n");
        exit(EXIT_FAILURE);
    }
    if (fabsf(sv.real[1]) > eps || fabsf(sv.real[2]) > eps) {
        fprintf(stderr, "test_interpreter mismatch: unexpected amplitude in |01> or |10>.\n");
        exit(EXIT_FAILURE);
    }

    free_state_vector(&sv);
    free_token_list(&token_list);
    free_instruction_list(&instr_list);
}

int main(void) {
    printf("Running test_assembly...\n");
    test_lexer();
    test_parser();
    test_interpreter();
    printf("All test_assembly tests passed!\n");
    return 0;
}
