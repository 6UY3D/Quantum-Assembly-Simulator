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
