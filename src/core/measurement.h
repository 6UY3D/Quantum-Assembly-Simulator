c

#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "state_vector.h"

/**
 * \brief Measures a single qubit in the computational basis (|0>, |1>) 
 *        and collapses the state vector accordingly.
 * \param sv Pointer to the StateVector
 * \param qubit_index Index of the qubit to measure
 * \param out_result Pointer to an integer where the measurement result (0 or 1) is stored
 * \return 0 on success, nonzero on error
 */
int measure_qubit(StateVector* sv, size_t qubit_index, int* out_result);

#ifdef __cplusplus
}
#endif

#endif /* MEASUREMENT_H */
