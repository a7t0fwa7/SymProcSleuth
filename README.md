
# SymProcSleuth

SymProcSleuth is a C implementation of the `SymProcAddress` function, which is an alternative to the `GetProcAddress` function for retrieving the address of a function within a module. This implementation is based on the original C++ code found in the [SymProcAddress repository](https://github.com/MzHmO/SymProcAddress/blob/main/SymProcAddress/main.cpp) by MzHmO.

## Differences and Enhancements

While the core functionality of SymProcSleuth remains the same as the original C++ implementation, there are few differences and enhancements made in this C version:

1. **Language**: The code has been ported from C++ to C, making it more accessible to developers who prefer or require a C implementation.
2. **Struct-based Module Information**: Instead of using C++'s `std::map` to store module information, SymProcSleuth introduces the `ModuleInfo` struct to hold the module handle and a dynamically allocated array of `FuncEntry` structs for storing function names and addresses.
3. **Dynamic Array for Module Storage**: SymProcSleuth uses a dynamically allocated array (`modules`) to store the `ModuleInfo` structs for each processed module. This allows for efficient storage and retrieval of module information.
4. **Explicit Memory Management**: Due to the nature of C, SymProcSleuth requires explicit memory management using `malloc`, `realloc`, and `free`. The code properly allocates and deallocates memory for the `ModuleInfo` and `FuncEntry` structs.
5. **String Duplication**: Since C doesn't have built-in string handling like C++, SymProcSleuth uses `_strdup` to duplicate function name strings and store them in the `FuncEntry` structs.
6. **Callback Function**: The `EnumSymbolsCallback` function is implemented as a regular C function instead of a lambda function. It receives a pointer to the `ModuleInfo` struct as the `UserContext` parameter, allowing it to store function information directly in the module's `funcs` array.
7. **Multiple Module Enumeration**: SymProcSleuth enhances the original implementation by efficiently handling multiple enumerations of the same module. It checks if a module has already been processed before enumerating its symbols, preventing duplicate entries and improving performance.
8. **Error Handling**: SymProcSleuth sets the last error to `ERROR_PROC_NOT_FOUND` using `SetLastError` when a function is not found within a module, providing proper error handling.
9. **Memory Cleanup**: The `FreeSymbols` function is introduced to free the dynamically allocated memory used by SymProcSleuth when it is no longer needed. This ensures proper cleanup and prevents memory leaks.

## Usage

To use SymProcSleuth in your C project, follow these steps:

1. Include the necessary header files: `windows.h`, `Dbghelp.h`, `stdio.h`, and `stdbool.h`.
2. Link against the `Dbghelp.lib` library.
3. Call the `SymProcAddress` function with the module handle and the desired function name to retrieve the function address.
4. Use the retrieved function address to call the desired function.
5. When done, call the `FreeSymbols` function to free the dynamically allocated memory.

For a complete example of how to use SymProcSleuth, refer to the `main` function in the provided code.

## Compatibility

SymProcSleuth is designed to work on Windows and requires the `Dbghelp.lib` library. It is compatible with both 32-bit and 64-bit Windows systems.

## License

SymProcSleuth is released under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code as per the terms of the license.

## Acknowledgements

Special thanks to MzHmO for the original C++ implementation of `SymProcAddress`, which served as the basis for SymProcSleuth.

Reference:
[1] https://github.com/MzHmO/SymProcAddress/blob/main/SymProcAddress/main.cpp
