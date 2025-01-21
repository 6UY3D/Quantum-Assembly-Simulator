# Utilities for Quantum Assembly Simulator

This folder contains utility modules that provide supporting functionality to the main simulator:

1. **File I/O**  
   - Read quantum assembly files from disk.  
   - Potentially write logs, config files, or simulator output.

2. **Logger**  
   - A simple logging system with log levels (INFO, DEBUG, WARN, ERROR).  
   - Can be expanded to write logs to file or integrate with a third-party library.

3. **Math Utils**  
   - Helper functions for common mathematical tasks.  
   - Includes function for complex arithmetic, numeric tolerance checks, etc.

**How to Use**  
- Include the relevant headers (`file_io.h`, `logger.h`, `math_utils.h`) in your other modules as needed.  
- Link these object files alongside the simulator’s core, assembly, and backend object files.

**Compilation**  
An example command (adapt paths and flags for your system):
```bash
cd src/utils
gcc -O3 -msse4.2 -c file_io.c logger.c math_utils.c
```
# Integration and Notes
1. Include Paths:
- Make sure to add -I./src/utils in your compilation commands so other modules can include these headers.
2. Logging:
- Replace fprintf(stderr, ...) calls throughout the code with log_message(LOG_LEVEL_ERROR, ...) or appropriate log levels for consistent logging across the project.
3. File I/O:
- You can use read_file_to_string(...) to read entire .qasm programs at once. Alternatively, you might prefer line-by-line reading with a typical fgets approach. This depends on the size of your quantum assembly files.
4. Math Utilities
- The example code is intentionally minimal. You can add matrix-multiplication routines, random number generation, or advanced linear algebra helpers as needed.
