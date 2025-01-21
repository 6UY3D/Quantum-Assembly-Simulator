c

#ifndef CIRCUIT_OPTIMIZER_H
#define CIRCUIT_OPTIMIZER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../assembly/parser.h"  // for InstructionList, etc.

/**
 * \brief Analyzes the InstructionList, simplifying redundant or consecutive gates.
 * \param instructions Pointer to an InstructionList to optimize
 * \return 0 on success, nonzero on error
 *
 * Typical optimizations might include:
 *  - Merging consecutive single-qubit gates on the same qubit (e.g., X followed by X => Identity)
 *  - Removing no-op gates
 *  - Combining adjacent Z, Z => Identity, etc.
 */
int optimize_circuit(InstructionList* instructions);

#ifdef __cplusplus
}
#endif

#endif /* CIRCUIT_OPTIMIZER_H */
