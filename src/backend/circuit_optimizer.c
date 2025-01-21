c

#include "circuit_optimizer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * \brief We'll store short sequences for single-qubit gates that produce Identity:
 *        - X then X => Identity
 *        - Y then Y => Identity
 *        - Z then Z => Identity
 *        - H then H => Identity
 *        - S then S then S then S => Identity
 *        - T repeated 8 times => Identity
 *
 * For demonstration, we'll only handle a few simple pairs.
 */
static int gates_cancel_out(const char* g1, const char* g2) {
    // Return 1 if g1 followed by g2 => identity for the same qubit.
    if (!g1 || !g2) return 0;
    if (strcasecmp(g1, "X") == 0 && strcasecmp(g2, "X") == 0) return 1;
    if (strcasecmp(g1, "Y") == 0 && strcasecmp(g2, "Y") == 0) return 1;
    if (strcasecmp(g1, "Z") == 0 && strcasecmp(g2, "Z") == 0) return 1;
    if (strcasecmp(g1, "H") == 0 && strcasecmp(g2, "H") == 0) return 1;

    return 0;
}

int optimize_circuit(InstructionList* instructions) {
    if (!instructions) return -1;

    // We'll do a single pass, merging or removing consecutive gates that form an identity.
    // A more advanced approach would do multiple passes until no more changes can be applied.
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < instructions->size; read_idx++) {
        Instruction* current = &instructions->data[read_idx];

        if (current->type == INSTR_GATE_SINGLE && read_idx + 1 < instructions->size) {
            Instruction* next = &instructions->data[read_idx + 1];
            // If next is also single-qubit gate on the same qubit
            if (next->type == INSTR_GATE_SINGLE && 
                current->qubits[0] == next->qubits[0]) {

                // Check if g1 + g2 => Identity
                if (gates_cancel_out(current->gate_name, next->gate_name)) {
                    // Skip both instructions => remove them
                    read_idx++; // consume the next one as well
                    continue;
                }
            }
        }
        // If we get here, we keep the instruction.
        if (write_idx != read_idx) {
            instructions->data[write_idx] = *current; // copy
        }
        write_idx++;
    }

    // Now set the new size
    instructions->size = write_idx;

    // Additional passes or advanced merges can be done here.
    return 0;
}
