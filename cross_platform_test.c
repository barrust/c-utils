/*
 * Cross-platform fileutils demonstration
 * This program demonstrates the cross-platform capabilities of the fileutils library
 * Compile on Windows: gcc -D_WIN32 -o cross_platform_test.exe cross_platform_test.c src/fileutils.c
 * Compile on POSIX:   gcc -o cross_platform_test cross_platform_test.c src/fileutils.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "src/fileutils.h"

int main(void) {
    printf("=== Cross-Platform File Utils Test ===\n\n");
    
    // Test current working directory
    char* cwd = fs_cwd();
    printf("Current working directory: %s\n", cwd);
    
    // Test path resolution
    char* resolved = fs_resolve_path(".");
    printf("Resolved current path: %s\n", resolved);
    
    // Test path combination with different separators
    char* combined1 = fs_combine_filepath(cwd, "test_file.txt");
    printf("Combined path (native): %s\n", combined1);
    
    // Test platform separator detection
#ifdef _WIN32
    printf("Platform: Windows\n");
    printf("Primary path separator: \\\\ \n");
    printf("Alternative path separator: /\n");
    char* combined2 = fs_combine_filepath("C:\\temp", "file.txt");
    printf("Windows-style path: %s\n", combined2);
    free(combined2);
#else
    printf("Platform: POSIX (Linux/Unix/macOS)\n");
    printf("Primary path separator: /\n");
    printf("Alternative path separator: \\\\ \n");
    char* combined2 = fs_combine_filepath("/tmp", "file.txt");
    printf("POSIX-style path: %s\n", combined2);
    free(combined2);
#endif
    
    // Test directory operations
    printf("\n=== Directory Operations ===\n");
    
    // Create a test directory
    int result = fs_mkdir("test_cross_platform", false);
    if (result == FS_SUCCESS) {
        printf("Successfully created directory: test_cross_platform\n");
    } else if (result == FS_EXISTS) {
        printf("Directory already exists: test_cross_platform\n");
    } else {
        printf("Failed to create directory: test_cross_platform\n");
    }
    
    // Check if it exists and identify it
    int path_type = fs_identify_path("test_cross_platform");
    if (path_type == FS_DIRECTORY) {
        printf("Confirmed: test_cross_platform is a directory\n");
        
        // List contents (should be empty)
        int num_items;
        char** items = fs_list_dir("test_cross_platform", &num_items);
        printf("Directory contains %d items\n", num_items);
        
        if (items) {
            for (int i = 0; i < num_items; i++) {
                printf("  - %s\n", items[i]);
                free(items[i]);
            }
            free(items);
        }
    }
    
    // Clean up test directory
    result = fs_rmdir("test_cross_platform");
    if (result == FS_SUCCESS) {
        printf("Successfully removed directory: test_cross_platform\n");
    }
    
    // Test file operations
    printf("\n=== File Operations ===\n");
    
    // Create a test file
    result = fs_touch("test_file.txt");
    if (result == FS_SUCCESS) {
        printf("Successfully created file: test_file.txt\n");
        
        // Initialize file object
        file_t f = f_init("test_file.txt");
        if (f) {
            printf("File info:\n");
            printf("  Absolute path: %s\n", f_absolute_path(f));
            printf("  Base directory: %s\n", f_basedir(f));
            printf("  Filename: %s\n", f_filename(f));
            printf("  File size: %zu bytes\n", f_filesize(f));
            printf("  Is symlink: %s\n", f_is_symlink(f) ? "Yes" : "No");
            
            f_free(f);
        }
        
        // Clean up test file
        result = fs_remove_file("test_file.txt");
        if (result == FS_SUCCESS) {
            printf("Successfully removed file: test_file.txt\n");
        }
    }
    
    // Clean up allocated memory
    free(cwd);
    free(resolved);
    free(combined1);
    
    printf("\n=== Test completed successfully ===\n");
    return 0;
}
