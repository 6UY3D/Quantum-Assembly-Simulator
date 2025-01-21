c

#ifndef INTERPRETER_H
#define INTERPRETER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "parser.h"
#include "state_vector.h"

/**
 * \brief Interprets a list of quantum assembly instructions and applies them to the given state vector.
 * \param instructions InstructionList to interpret
 * \param sv Pointer to a StateVector
 * \return 0 on success, nonzero on error
 */
int interpret_instructions(const InstructionList* instructions, StateVector* sv);

#ifdef __cplusplus
}
#endif

#endif /* INTERPRETER_H */
