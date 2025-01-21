c

#ifndef GATE_OPERATIONS_H
#define GATE_OPERATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "state_vector.h"

/**
 * \brief Applies a 2x2 single-qubit gate to the specified qubit index.
 * \param sv The state vector
 * \param gate A 2x2 matrix in row-major order: gate[0], gate[1], gate[2], gate[3]
 * \param qubit_index The qubit to which this gate is applied (0-based)
 * \return 0 on success, nonzero on error
 *
 * gate layout (complex float):
 * gate[0] = real(0,0), gate[1] = imag(0,0)
 * gate[2] = real(0,1), gate[3] = imag(0,1)
 * gate[4] = real(1,0), gate[5] = imag(1,0)
 * gate[6] = real(1,1), gate[7] = imag(1,1)
 */
int apply_single_qubit_gate(StateVector* sv, const float* gate, size_t qubit_index);

/**
 * \brief Applies a controlled-NOT gate (CNOT) with control and target qubits.
 * \param sv The state vector
 * \param control_qubit Index of the control qubit
 * \param target_qubit Index of the target qubit
 * \return 0 on success, nonzero on error
 */
int apply_cnot(StateVector* sv, size_t control_qubit, size_t target_qubit);

#ifdef __cplusplus
}
#endif

#endif /* GATE_OPERATIONS_H */
