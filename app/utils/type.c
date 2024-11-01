#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAX_PATH 1024
#define BIN_PATH "/home/olfi/shell/app/bin/"

// return true if the file specified by the filename exists
bool file_exists(const char *filename)
{
    return access(filename, F_OK) == 0;
}

// search for the command in the system PATH
bool search_in_path(const char *command, char *result)
{
    char *path = getenv("PATH");
    if (path == NULL) {
        return false;
    }

    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");

    while (dir != NULL) {
        snprintf(result, MAX_PATH, "%s/%s", dir, command);
        if (file_exists(result)) {
            free(path_copy);
            return true;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return false;
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
        printf("\e[31m%s\e[0m is a shell \e[31mbuiltin\e[0m\n", command);
    } else if (search_in_path(command, path)) {
        printf("%s is %s\n", command, path);
    } else {
        printf("%s: not found\n", command);
    }

    return 0;
}