/**
 * SymProcSleuth is an implementation of GetProcAddress that uses the
 * SymFromAddr and SymEnumSymbols Windows API functions to look up exported
 * function addresses by name from a module handle. It caches the results
 * to avoid repeated symbol lookups. FreeSymbols releases the cache.
 */
#include <windows.h>
#include <Dbghelp.h>
#include <stdio.h>
#include <stdbool.h>

#pragma comment(lib, "Dbghelp.lib")

// Define a function pointer type for MessageBoxW for the example usage below 
typedef int(WINAPI *MessageBoxWFunc)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType); 

// Define a structure to hold function entry details
typedef struct {
    char* name;      // Function name
    FARPROC address; // Function address
} FuncEntry;

// Define a structure to hold module entry details
typedef struct {
    HMODULE module;    // Handle to the module
    FuncEntry* funcs;  // Pointer to the functions within the module
    size_t numFuncs;   // Number of functions
} ModuleEntry;

static ModuleEntry* modules = NULL; // Dynamically allocated array of module entries
static size_t numModules = 0;       // Number of module entries

// Callback function for enumerating symbols. Adds function entries to the module entry.
BOOL CALLBACK EnumSymbolsCallback(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext) {
    ModuleEntry* moduleEntry = (ModuleEntry*)UserContext;
    // Check if symbol is a function
    if (pSymInfo->Flags & 0x00000200) {
        // Reallocate funcs array to accommodate the new function
        moduleEntry->funcs = realloc(moduleEntry->funcs, (moduleEntry->numFuncs + 1) * sizeof(FuncEntry));
        // Duplicate the symbol name and save it along with its address
        moduleEntry->funcs[moduleEntry->numFuncs].name = _strdup(pSymInfo->Name);
        moduleEntry->funcs[moduleEntry->numFuncs].address = (FARPROC)pSymInfo->Address;
        // Increment the number of functions
        moduleEntry->numFuncs++;
    }
    return TRUE;
}

// Finds the address of a symbol (function) within a specified module.
FARPROC SymProcAddress(HMODULE hModule, LPCSTR lpProcName) {
    for (size_t i = 0; i < numModules; i++) {
        // Check if the module matches the one we're looking for
        if (modules[i].module == hModule) {
            // Search for the function by name within the module
            for (size_t j = 0; j < modules[i].numFuncs; j++) {
                if (_stricmp(modules[i].funcs[j].name, lpProcName) == 0) {
                    // If found, return its address
                    return modules[i].funcs[j].address;
                }
            }
            // If the function was not found, set error and return NULL
            SetLastError(127); // ERROR_PROC_NOT_FOUND
            return NULL;
        }
    }

    // If module is not already in the list, add it and enumerate its symbols
    ModuleEntry newModule;
    newModule.module = hModule;
    newModule.funcs = NULL;
    newModule.numFuncs = 0;

    // Initialize symbols enumeration for the process and specified module
    HANDLE hProc = GetCurrentProcess();
    SymInitialize(hProc, NULL, TRUE);
#ifdef _WIN64
    // Enumerate symbols for 64-bit module
    if (!SymEnumSymbols(hProc, (DWORD64)hModule, NULL, EnumSymbolsCallback, &newModule)) {
        SymCleanup(hProc);
        return NULL;
    }
#else
    // Enumerate symbols for 32-bit module
    if (!SymEnumSymbols(hProc, (DWORD)hModule, NULL, EnumSymbolsCallback, &newModule)) {
        SymCleanup(hProc);
        return NULL;
    }
#endif

    // Add the new module to the modules array
    modules = realloc(modules, (numModules + 1) * sizeof(ModuleEntry));
    modules[numModules] = newModule;
    numModules++;

    // Try to find the function again in the newly added module
    for (size_t i = 0; i < newModule.numFuncs; i++) {
        if (_stricmp(newModule.funcs[i].name, lpProcName) == 0) {
            return newModule.funcs[i].address;
        }
    }

    // If the function is still not found, set error and return NULL
    SetLastError(127); // ERROR_PROC_NOT_FOUND
    return NULL;
}

// Frees allocated memory for symbols and resets module information
void FreeSymbols() {
    for (size_t i = 0; i < numModules; i++) {
        // Free memory allocated for function names
        for (size_t j = 0; j < modules[i].numFuncs; j++) {
            free(modules[i].funcs[j].name);
        }
        // Free memory allocated for function entries
        free(modules[i].funcs);
    }
    // Free memory allocated for module entries
    free(modules);
    modules = NULL;
    numModules = 0;
}

// Example usage demonstrating how to use the implemented functions

int main()
{
    HMODULE hModule = NULL;

    // Load "user32.dll" module
    hModule = LoadLibraryA("user32.dll");

    // Find the address of "MessageBoxW" function within the loaded module
    MessageBoxWFunc MessageBoxWPtr = (MessageBoxWFunc)(SymProcAddress(hModule, "MessageBoxW"));

    // Call the found function to display a message box
    MessageBoxWPtr(NULL, L"Lol who said we needed GetProcAddress() ?? xD", L"Hi from a7t0fwa7", MB_OK);

    // Free allocated symbols information
    FreeSymbols();

    return 0;
}
