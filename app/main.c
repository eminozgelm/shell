#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

// Constants
#define MAX_LENGTH 256
#define MAX_TOKENS 100
#define BIN_PATH "/home/olfi/shell/app/bin/"

// Functions
void tokenize(char* input, char** tokens, int* tokenCount);
int builtin_cd(char **args);
int execute_command(char **tokens, int token_count);
bool search_in_path(const char *command, char *result);

int main() {
    char command[MAX_LENGTH];
    
    while(1) { 
        char *tokens[MAX_TOKENS];
        int tokenCount = 0;
        
        printf("$ ");  
        fflush(stdout);
        
        if(fgets(command, sizeof(command), stdin) == NULL) break;
        command[strcspn(command, "\n")] = 0;
        
        tokenize(command, tokens, &tokenCount);
        tokens[tokenCount] = NULL;  // Null-terminate the tokens array
        
        if(tokenCount > 0) {
            if(strcmp(tokens[0], "exit") == 0) {
                break;
            }
            
            if(execute_command(tokens, tokenCount) == -1) {
                printf("%s: \e[31mcommand\e[0m not found\n", tokens[0]);
            }
        } 
    }
    return 0;
}

void tokenize(char* input, char** tokens, int* tokenCount)
{
  char* token = strtok(input, " ");
  *tokenCount = 0;

  while(token != NULL && *tokenCount < MAX_TOKENS) {
        tokens[*tokenCount] = token;
        (*tokenCount)++;
        token = strtok(NULL, " \t\n");
  }
}

int builtin_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
        return 1;
    } else {
      char* env_path = args[1];
      if(strcmp(args[1], "~") == 0){
        env_path = getenv("HOME");
      }
      if (chdir(env_path) != 0) {
            fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
            return 1;
        }
    }
    return 1;
}


bool search_in_path(const char *command, char *result) {
    char *path = getenv("PATH");
    if (path == NULL) {
        return false;
    }

    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");

    while (dir != NULL) {
        snprintf(result, MAX_LENGTH, "%s/%s", dir, command);
        if (access(result, X_OK) == 0) {
            free(path_copy);
            return true;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return false;
}

int execute_command(char **tokens, int token_count) {
    
    if (strcmp(tokens[0], "cd") == 0) {
        return builtin_cd(tokens);
    }

    char path[MAX_LENGTH];
    
    // First, check in the custom bin directory
    snprintf(path, sizeof(path), "%s%s", BIN_PATH, tokens[0]);
    
    // If not found in custom bin, search in PATH
    if (access(path, X_OK) != 0) {
        if (!search_in_path(tokens[0], path)) {
            return -1; // Command not found
        }
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execv(path, tokens);
        // If execv returns, it must have failed
        fprintf(stderr, "execv failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    } else {
        perror("fork");
        return -1;
    }
}


