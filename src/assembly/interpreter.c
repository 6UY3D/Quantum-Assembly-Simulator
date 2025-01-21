#include "interpreter.h"
#include "gate_operations.h"
#include "measurement.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/**
 * \brief Gate definitions (2x2) for common single-qubit operations in float
 *        Format: [r00, i00, r01, i01, r10, i10, r11, i11]
 */
static const float H_GATE[8] = {
    0.707f, 0.0f,  0.707f, 0.0f,
    0.707f, 0.0f, -0.707f, 0.0f
};

static const float X_GATE[8] = {
    0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f
};

static const float Y_GATE[8] = {
    0.0f, 0.0f, 0.0f, -1.0f,
    0.0f, 1.0f, 0.0f,  0.0f
};

static const float Z_GATE[8] = {
    1.0f, 0.0f, 0.0f,  0.0f,
    0.0f, 0.0f, -1.0f, 0.0f
};

// Phase gate S = [1,0;0,i], T = [1,0;0,e^{i\pi/4}]
static const float S_GATE[8] = {
    1.0f, 0.0f,  0.0f, 0.0f,
    0.0f, 0.0f,  0.0f, 1.0f
};

static const float T_GATE[8] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.7071f, 0.7071f // e^{i\pi/4} = 1/sqrt(2) + i/sqrt(2)
};

// If a custom gate name is not recognized, we handle it as identity or throw a warning.
static const float ID_GATE[8] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f
};

/**
 * \brief Returns pointer to a 2x2 float array representing the gate (or ID_GATE if unknown).
 */
static const float* get_single_qubit_gate(const char* gate_name) {
    if (strcasecmp(gate_name, "H") == 0)  return H_GATE;
    if (strcasecmp(gate_name, "X") == 0)  return X_GATE;
    if (strcasecmp(gate_name, "Y") == 0)  return Y_GATE;
    if (strcasecmp(gate_name, "Z") == 0)  return Z_GATE;
    if (strcasecmp(gate_name, "S") == 0)  return S_GATE;
    if (strcasecmp(gate_name, "T") == 0)  return T_GATE;

    // Unknown single-qubit gate => identity
    fprintf(stderr, "Warning: unrecognized single-qubit gate '%s'. Using identity.\n", gate_name);
    return ID_GATE;
}

int interpret_instructions(const InstructionList* instructions, StateVector* sv) {
    if (!instructions || !sv) return -1;

    for (size_t i = 0; i < instructions->size; i++) {
        const Instruction* instr = &instructions->data[i];
        // Check qubit range
        for (size_t q = 0; q < instr->qubit_count; q++) {
            if (instr->qubits[q] >= sv->num_qubits) {
                fprintf(stderr, "Interpret error: qubit index %zu out of range (max %zu).\n",
                        instr->qubits[q], sv->num_qubits - 1);
                return -2;
            }
        }

        switch (instr->type) {
            case INSTR_GATE_SINGLE: {
                const float* gate = get_single_qubit_gate(instr->gate_name);
                if (apply_single_qubit_gate(sv, gate, instr->qubits[0]) != 0) {
                    fprintf(stderr, "Interpret error: failed to apply single-qubit gate '%s'.\n",
                            instr->gate_name);
                    return -3;
                }
                break;
            }
            case INSTR_GATE_MULTI: {
                // Currently, only "CNOT" or multi-qubit gates recognized
                // We check if gate_name is "CNOT"
                if (strcasecmp(instr->gate_name, "CNOT") == 0 && instr->qubit_count == 2) {
                    if (apply_cnot(sv, instr->qubits[0], instr->qubits[1]) != 0) {
                        fprintf(stderr, "Interpret error: failed to apply CNOT.\n");
                        return -4;
                    }
                } else {
                    fprintf(stderr, "Interpret warning: unrecognized multi-qubit gate '%s'.\n", instr->gate_name);
                }
                break;
            }
            case INSTR_MEASURE: {
                // measure_qubit => collapses the state
                int outcome = -1;
                if (measure_qubit(sv, instr->qubits[0], &outcome) != 0) {
                    fprintf(stderr, "Interpret error: measure_qubit failed.\n");
                    return -5;
                }
                // You could store the outcome in a classical register if you want
                printf("Measurement of qubit %zu => %d\n", instr->qubits[0], outcome);
                break;
            }
            case INSTR_UNKNOWN:
            default:
                // Possibly an unrecognized instruction with partial data
                fprintf(stderr, "Interpret warning: unknown instruction type for gate '%s'.\n",
                        instr->gate_name);
                break;
        }
    }

    return 0;
}

/*
 * Basic test stub (optional).
 * Compile with (assuming other .o files are built):
 *   gcc -o test_interpreter interpreter.c parser.c lexer.c ../core/gate_operations.c ../core/measurement.c ../core/state_vector.c
 * Then run `./test_interpreter`.
 */
#ifdef TEST_INTERPRETER
#include "lexer.h"
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand((unsigned)time(NULL));
    // Example QASM lines:
    const char* lines[] = {
        "H 0",
        "CNOT 0 1",
        "MEASURE 1",
        "// This is a comment",
        "Z 1",
        "MEASURE 0"
    };

    // Lex
    TokenList token_list;
    init_token_list(&token_list);
    for (int i = 0; i < 6; i++) {
        lex_line(lines[i], &token_list);
    }

    // Parse
    InstructionList instr_list;
    init_instruction_list(&instr_list);
    parse_tokens(&token_list, &instr_list);

    // Prepare StateVector (2 qubits)
    StateVector sv;
    init_state_vector(&sv, 2);

    // Interpret
    interpret_instructions(&instr_list, &sv);

    // Cleanup
    free_token_list(&token_list);
    free_instruction_list(&instr_list);
    free_state_vector(&sv);

    return 0;
}
#endif
