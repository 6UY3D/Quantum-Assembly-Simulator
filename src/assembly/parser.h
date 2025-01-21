#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lexer.h"
#include <stddef.h>

/**
 * \brief Enumerates all possible instruction types in our quantum assembly language.
 */
typedef enum {
    INSTR_GATE_SINGLE,   /**< Single-qubit gate: "H 0", "X 1", "Z 0", etc. */
    INSTR_GATE_MULTI,    /**< Multi-qubit gate: "CNOT 0 1" */
    INSTR_MEASURE,       /**< "MEASURE <qubit>" */
    INSTR_UNKNOWN
} InstructionType;

/**
 * \brief Data structure representing one instruction in the quantum assembly language.
 *
 * For a single-qubit gate, qubits[0] is used.
 * For a multi-qubit gate, qubits[0] and qubits[1], etc.
 * For measurement, qubits[0] is the measured qubit.
 */
typedef struct {
    InstructionType type;
    char    gate_name[32]; /**< Gate string, e.g. "H", "X", "CNOT", "MEASURE" */
    size_t  qubits[2];     /**< Up to two qubits for standard gates. Extend if needed. */
    size_t  qubit_count;   /**< How many qubits are relevant to this instruction. */
} Instruction;

/**
 * \brief Dynamic array of instructions.
 */
typedef struct {
    Instruction* data;
    size_t       size;
    size_t       capacity;
} InstructionList;

/**
 * \brief Initializes an InstructionList.
 * \param list Pointer to an uninitialized InstructionList
 * \return 0 on success, nonzero on error
 */
int init_instruction_list(InstructionList* list);

/**
 * \brief Appends an instruction to the InstructionList.
 * \param list Pointer to an InstructionList
 * \param instr Instruction to append
 * \return 0 on success, nonzero on error
 */
int append_instruction(InstructionList* list, const Instruction* instr);

/**
 * \brief Frees an InstructionList's resources.
 * \param list Pointer to an InstructionList
 */
void free_instruction_list(InstructionList* list);

/**
 * \brief Parses the provided TokenList into an InstructionList.
 * \param tokens The TokenList from lexer
 * \param instructions Output InstructionList
 * \return 0 on success, nonzero on error
 */
int parse_tokens(const TokenList* tokens, InstructionList* instructions);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */
