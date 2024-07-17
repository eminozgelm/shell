#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

// Constants
#define MAX_LENGTH 256
#define MAX_TOKENS 100
#define BIN_PATH ".\\bin\\"

// Functions
void tokenize(char* input, char** tokens, int* tokenCount);
bool execute_command(char **tokens, int token_count);

int main() {

  char command[MAX_LENGTH];
  
  while(1)
  { 
      char *tokens[MAX_TOKENS];
      int tokenCount = 0;

      printf("$ ");  
      fflush(stdout);

      if(fgets(command, sizeof(command), stdin) == NULL) break;

      command[strcspn(command, "\n")] = 0;

      tokenize(command, tokens, &tokenCount);

      if(tokens > 0){

        if(strcmp(tokens[0], "exit") == 0){
          break;
        }
        else {
        
          if(!execute_command(tokens, tokenCount)){
            printf("%s: \e[31mcommand\e[0m not found\n", tokens[0]);
          }
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


bool execute_command(char **tokens, int token_count) {
    char path[MAX_LENGTH];
    snprintf(path, sizeof(path), "%s%s", BIN_PATH, tokens[0]);

    if (access(path, X_OK) == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execv(path, tokens);
            exit(EXIT_FAILURE);  // If execv returns, it must have failed
        } else if (pid > 0) {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
            return true;
        } else {
            perror("fork");
            return false;
        }
    }
    return 0;
}


