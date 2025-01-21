c

#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INITIAL_CAPACITY 16

int init_instruction_list(InstructionList* list) {
    if (!list) return -1;
    list->data = (Instruction*)malloc(INITIAL_CAPACITY * sizeof(Instruction));
    if (!list->data) return -2;
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    return 0;
}

int append_instruction(InstructionList* list, const Instruction* instr) {
    if (!list || !instr) return -1;
    if (list->size >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        Instruction* new_data = (Instruction*)realloc(list->data, new_cap * sizeof(Instruction));
        if (!new_data) return -2;
        list->data = new_data;
        list->capacity = new_cap;
    }
    list->data[list->size] = *instr; // Copy
    list->size++;
    return 0;
}

void free_instruction_list(InstructionList* list) {
    if (!list) return;
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

/**
 * \brief Helper function: check if token text is equal to gate name ignoring case.
 */
static int token_equals(const char* token_text, const char* gate_name) {
    return strcasecmp(token_text, gate_name) == 0;
}

/**
 * \brief Attempt to parse an integer from token text.
 */
static int parse_int(const char* text, size_t* out_val) {
    if (!text || !out_val) return -1;
    char* endptr = NULL;
    long val = strtol(text, &endptr, 10);
    if (*endptr != '\0' || val < 0) {
        return -2; // Not a valid nonnegative integer
    }
    *out_val = (size_t)val;
    return 0;
}

int parse_tokens(const TokenList* tokens, InstructionList* instructions) {
    if (!tokens || !instructions) return -1;

    for (size_t i = 0; i < tokens->size; i++) {
        Token tk = tokens->data[i];
        if (tk.type == TOKEN_COMMENT) {
            // Skip comment tokens
            continue;
        }
        if (tk.type == TOKEN_EOF) {
            // End of file => done
            break;
        }

        // We expect a gate or a recognized keyword here
        if (tk.type != TOKEN_GATE) {
            fprintf(stderr, "Parser error: unexpected token '%s', expected a gate or comment.\n", tk.text);
            return -2;
        }

        Instruction instr;
        memset(&instr, 0, sizeof(instr));
        instr.type = INSTR_UNKNOWN;
        strncpy(instr.gate_name, tk.text, sizeof(instr.gate_name) - 1);
        instr.gate_name[sizeof(instr.gate_name) - 1] = '\0';

        // Check if it's a recognized gate, e.g. "H", "X", "Y", "Z", "CNOT", "MEASURE"
        // Single-qubit gates
        if (token_equals(tk.text, "H") || token_equals(tk.text, "X") || 
            token_equals(tk.text, "Y") || token_equals(tk.text, "Z") || 
            token_equals(tk.text, "S") || token_equals(tk.text, "T")) {

            instr.type = INSTR_GATE_SINGLE;
            // Next token should be an integer for the qubit index
            if (i + 1 < tokens->size && tokens->data[i + 1].type == TOKEN_INTEGER) {
                size_t qubit_idx = 0;
                if (parse_int(tokens->data[i + 1].text, &qubit_idx) != 0) {
                    fprintf(stderr, "Parser error: invalid qubit index '%s'.\n", tokens->data[i + 1].text);
                    return -3;
                }
                instr.qubits[0] = qubit_idx;
                instr.qubit_count = 1;
                i++; // consume next token
            } else {
                fprintf(stderr, "Parser error: expected qubit index after gate '%s'.\n", tk.text);
                return -4;
            }
            append_instruction(instructions, &instr);
        }
        else if (token_equals(tk.text, "CNOT")) {
            instr.type = INSTR_GATE_MULTI;
            // Expect 2 integer tokens: control, target
            if (i + 2 < tokens->size && 
                tokens->data[i + 1].type == TOKEN_INTEGER &&
                tokens->data[i + 2].type == TOKEN_INTEGER) {

                size_t ctrl = 0, tgt = 0;
                if (parse_int(tokens->data[i + 1].text, &ctrl) != 0 ||
                    parse_int(tokens->data[i + 2].text, &tgt) != 0) {
                    fprintf(stderr, "Parser error: invalid CNOT parameters.\n");
                    return -5;
                }
                instr.qubits[0] = ctrl;
                instr.qubits[1] = tgt;
                instr.qubit_count = 2;
                i += 2; // consume next two tokens
            } else {
                fprintf(stderr, "Parser error: expected two qubit indices after 'CNOT'.\n");
                return -6;
            }
            append_instruction(instructions, &instr);
        }
        else if (token_equals(tk.text, "MEASURE")) {
            instr.type = INSTR_MEASURE;
            // Expect 1 integer token
            if (i + 1 < tokens->size && tokens->data[i + 1].type == TOKEN_INTEGER) {
                size_t qubit_idx = 0;
                if (parse_int(tokens->data[i + 1].text, &qubit_idx) != 0) {
                    fprintf(stderr, "Parser error: invalid qubit index '%s' for measurement.\n", tokens->data[i + 1].text);
                    return -7;
                }
                instr.qubits[0] = qubit_idx;
                instr.qubit_count = 1;
                i++; // consume next token
            } else {
                fprintf(stderr, "Parser error: expected qubit index after 'MEASURE'.\n");
                return -8;
            }
            append_instruction(instructions, &instr);
        }
        else {
            // Possibly an unrecognized gate
            fprintf(stderr, "Parser warning: unrecognized gate '%s'.\n", tk.text);
            // We could parse it as single-qubit if there's a param:
            if (i + 1 < tokens->size && tokens->data[i + 1].type == TOKEN_INTEGER) {
                instr.type = INSTR_GATE_SINGLE;
                size_t qubit_idx = 0;
                if (parse_int(tokens->data[i + 1].text, &qubit_idx) == 0) {
                    instr.qubits[0] = qubit_idx;
                    instr.qubit_count = 1;
                    i++;
                }
            }
            append_instruction(instructions, &instr);
        }
    }

    return 0;
}
