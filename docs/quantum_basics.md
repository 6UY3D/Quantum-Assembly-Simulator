# Quantum Basics

This document provides a concise introduction to the core principles of quantum computing, including qubits, superposition, and entanglement. It’s intended for readers who may be new to quantum concepts but want to explore how the **Quantum Assembly Simulator** works under the hood.

## 1. Qubits
A **qubit** is the basic unit of quantum information, analogous to a bit in classical computing but capable of existing in a superposition of states.

- **Representation**: A qubit is typically represented by a two-dimensional complex vector:
  \[
    \lvert \psi \rangle = \alpha \lvert 0 \rangle + \beta \lvert 1 \rangle
  \]
  where \( \alpha \) and \( \beta \) are complex numbers such that \( |\alpha|^2 + |\beta|^2 = 1 \).

## 2. Superposition
- **Definition**: A qubit can simultaneously be in both \(\lvert 0 \rangle\) and \(\lvert 1 \rangle\) states until measured.
- **Simulator Handling**: The simulator’s state vector tracks these complex amplitudes, updating them as gates are applied.

## 3. Entanglement
- **Meaning**: Two or more qubits can share a combined state that cannot be factored into individual qubit states.
- **Example**: A Bell state like:
  \[
    \frac{1}{\sqrt{2}} \left( \lvert 00 \rangle + \lvert 11 \rangle \right)
  \]
- **Importance**: Entanglement is a key resource for quantum communication and algorithms such as quantum teleportation.

## 4. Quantum Gates
- **Basic Gates**:
  - **Hadamard (H)**: Creates superposition.
  - **Pauli-X (X)**: Flips \(\lvert 0 \rangle \leftrightarrow \lvert 1 \rangle\).
  - **Pauli-Z (Z)**: Phase-flips \(\lvert 0 \rangle\) to \(\lvert 0 \rangle\) and \(\lvert 1 \rangle\) to \(-\lvert 1 \rangle\).
  - **Phase (S, T)**: Introduce controlled phase shifts.
- **Simulator Implementation**:
  - Represented by \(2 \times 2\) or \(2^n \times 2^n\) matrices, depending on the number of qubits.
  - Applied to the state vector to transform qubit amplitudes.

## 5. Measurement
- **Process**: Collapses a qubit’s state to \(\lvert 0 \rangle\) or \(\lvert 1 \rangle\) probabilistically.
- **Simulator Behavior**:
  - Randomly selects outcomes based on amplitudes.
  - Updates the state vector to reflect the measurement outcome.

## Next Steps
Now that you have a basic understanding of quantum mechanics, you’re ready to explore our **Simulator Architecture** in more detail. Check out [simulator_architecture.md](simulator_architecture.md) for an inside look at how the simulator is built.
