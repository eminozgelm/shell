#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_PATH 1024
#define BIN_PATH "./bin/"

int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

int is_executable(const char *path) {
    return access(path, X_OK) == 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: type <command>\n");
        return 1;
    }

    const char *command = argv[1];
    char path[MAX_PATH];

    // Check if it's a command in the bin directory
    snprintf(path, sizeof(path), "%s%s", BIN_PATH, command);
    
    if (file_exists(path)) {
        if (is_executable(path)) {
            printf("\e[31m%s\e[0m is a shell \e[31mbuiltin\e[0m\n", command);
        } else {
            printf("%s: not found\n", command);
        }
        return 0;
    }

   

    // If not found anywhere
    printf("%s: not found\n", command);
    return 1;
}