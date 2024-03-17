# SymProcSleuth

SymProcSleuth is a C implementation of the `SymProcAddress` function, which is an alternative to the `GetProcAddress` function for retrieving the address of a function within a module. This implementation is based on the original C++ code from the [SymProcAddress repository](https://github.com/MzHmO/SymProcAddress/blob/main/SymProcAddress/main.cpp) by MzHmO.

## Differences and Enhancements

The C version of the code, SymProcSleuth, introduces several differences and enhancements compared to the original C++ implementation:

1. **Language**: The code has been translated from C++ to pure C, making it more portable and compatible with a wider range of systems and compilers.
2. **Struct-based Module Information**: Instead of using C++-specific features like `std::map`, the C version utilizes a custom `ModuleInfo` struct to store information about each processed module, including the module handle and a dynamically allocated array of function entries.
3. **Dynamic Memory Allocation**: The C version employs dynamic memory allocation using `malloc`, `realloc`, and `free` to manage the storage of module information and function entries. This allows for flexibility in handling multiple modules and functions.
4. **Callback Function**: The `EnumSymbolsCallback` function is implemented as a regular C function instead of a lambda function. It is used as a callback for the `SymEnumSymbols` function to process the enumerated symbols.
5. **Multiple Module Enumeration**: SymProcSleuth has been enhanced to handle multiple enumerations of the same module efficiently. It maintains a list of processed modules and checks if a module has already been processed before enumerating its symbols, avoiding duplicate entries.
6. **Error Handling**: The C version includes proper error handling, setting the last error to `ERROR_PROC_NOT_FOUND` when a function is not found within a module.
7. **Memory Management**: SymProcSleuth provides a `FreeSymbols` function to free the dynamically allocated memory used for storing module information and function entries. This ensures proper cleanup and prevents memory leaks.

## Compilation Instructions

To compile the SymProcSleuth code, follow these steps:

### Windows (using Visual Studio)

1. Open a Visual Studio Developer Command Prompt.
2. Navigate to the directory containing the `SymProcSleuth.c` file.
3. Run the following command to compile the code:

   ```
   cl /EHsc SymProcSleuth.c /link dbghelp.lib
   ```

   This command compiles the `SymProcSleuth.c` file with exception handling enabled (`/EHsc`) and links against the `dbghelp.lib` library.

### Windows (using MinGW)

1. Open a command prompt.
2. Navigate to the directory containing the `SymProcSleuth.c` file.
3. Run the following command to compile the code:

   ```
   gcc -o SymProcSleuth.exe SymProcSleuth.c -ldbghelp
   ```

   This command compiles the `SymProcSleuth.c` file and links against the `dbghelp` library, generating an executable named `SymProcSleuth.exe`.

Make sure you have the necessary compiler (Visual Studio or MinGW) installed and properly configured on your system. Additionally, ensure that the `dbghelp.lib` library is available in your system's library search path.

After successful compilation, you can run the resulting executable (`SymProcSleuth.exe`) to test the functionality of the `SymProcAddress` function.

## License

This code is released under the [MIT License](LICENSE).

References:
[1] https://github.com/MzHmO/SymProcAddress/blob/main/SymProcAddress/main.cpp

Special thanks to MzHmO for the original C++ implementation of `SymProcAddress`, which served as the basis for SymProcSleuth.
