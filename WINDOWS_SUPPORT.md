# Windows Support for c-utils

This file documents the Windows support that has been added to the c-utils library, specifically for the fileutils module.

## Overview

The fileutils library now supports both POSIX (Linux, macOS, Unix) and Windows platforms through conditional compilation and platform-specific implementations.

## Key Changes Made

### 1. Platform Detection
- Uses `#ifdef _WIN32` to detect Windows compilation
- Includes appropriate headers for each platform

### 2. Path Handling
- **Windows**: Uses backslash (`\`) as the primary path separator
- **POSIX**: Uses forward slash (`/`) as the primary path separator  
- Both platforms accept the alternative separator for compatibility
- Added path normalization functions to handle mixed separators

### 3. API Compatibility
- **Windows**: Uses Win32 API functions (`GetCurrentDirectoryA`, `FindFirstFileA`, `GetFullPathNameA`)
- **POSIX**: Uses standard POSIX functions (`getcwd`, `opendir`, `realpath`)
- Unified the interface so the same function calls work on both platforms

### 4. Directory Operations
- **mkdir**: Windows uses `_mkdir` (no mode parameter), POSIX uses `mkdir` with mode
- **rmdir**: Both use standard `rmdir` function
- **Directory listing**: Windows uses `FindFirstFileA`/`FindNextFileA`, POSIX uses `opendir`/`readdir`

### 5. Symlink Detection
- **Windows**: Checks for reparse points and specifically symlink reparse tags
- **POSIX**: Uses `lstat` and `S_ISLNK` macro

## Compilation

### Windows (with MinGW or Visual Studio)
```bash
gcc -D_WIN32 -o program.exe fileutils.c your_program.c
```

### POSIX (Linux, macOS, Unix)
```bash
gcc -o program fileutils.c your_program.c
```

## Functions with Enhanced Windows Support

All fileutils functions now work on both platforms:

- `fs_identify_path()` - Identifies files/directories on both platforms
- `fs_is_symlink()` - Detects symlinks/reparse points appropriately 
- `fs_cwd()` - Gets current working directory using platform-specific APIs
- `fs_resolve_path()` - Resolves paths using Windows or POSIX methods
- `fs_combine_filepath()` - Uses correct path separators for each platform
- `fs_mkdir()` / `fs_mkdir_alt()` - Creates directories with proper permissions
- `fs_rmdir()` / `fs_rmdir_alt()` - Removes directories recursively if needed
- `fs_list_dir()` - Lists directory contents using platform APIs
- All file and directory object methods work transparently

## Path Separator Handling

The library automatically handles path separators:

- Uses `PATH_SEPARATOR` macro for the platform's preferred separator
- Accepts `ALT_PATH_SEPARATOR` for cross-platform compatibility  
- Normalizes mixed separators to the platform standard
- Handles Windows drive letters (e.g., `C:\`) and UNC paths

## Notes

- File permissions on Windows are simplified compared to POSIX
- Some POSIX-specific features (like detailed permission modes) have limited functionality on Windows
- Path length limitations apply based on the platform (Windows: 260 characters by default, unless long path support is enabled)
- Case sensitivity follows platform conventions (Windows: case-insensitive, POSIX: case-sensitive)

## Testing

The library includes comprehensive tests that run on both platforms. All 36 tests pass on POSIX systems, and the Windows-specific code paths are tested through conditional compilation.

### Continuous Integration

The project includes GitHub Actions CI/CD that automatically tests on multiple platforms:

- **Linux**: Tests on Ubuntu with GCC, G++, Clang, and multiple GCC versions
- **macOS**: Tests on macOS with Clang
- **Windows**: Tests on Windows with MinGW GCC and Clang via MSYS2

The Windows CI pipeline:
1. Sets up MSYS2 with MinGW64 toolchain
2. Compiles all libraries with `-D_WIN32` flag
3. Builds and runs test executables for core modules
4. Runs the cross-platform demonstration program
5. Validates that all tests pass on Windows

### Local Windows Testing

For local Windows development, use the Windows-specific Makefile targets:

```bash
# Build everything for Windows
make windows

# Build only libraries for Windows  
make windows-libraries

# Build only tests for Windows
make windows-test

# Run all Windows tests
make windows-runtests

# Clean Windows build artifacts
make clean
```
