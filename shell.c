#include "shell.h"

void parseInput(char* input, char** args) {
    char *token;
    int i = 0;

    token = strtok(input," \t\n");
    while(token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }

    args[i] = NULL;
}

int main(int argc, char* argv[]) {
    char buffer[MAX_LINE];
    char *args[MAX_ARGS];

    
    for (int i = 0; i < 20; i++) {
        printf("*");
    }

    printf("\n");
    
    printf("Welcome to my shell\n");
    for (int i = 0; i < 20; i++) {
        printf("*");
    }

    while(1) {
        printf("\nmyShell> ");

        // If EOF or any other issues are encountered, exit the shell
        if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\n");
            break;
        }

        parseInput(buffer, args);

        // Empty command
        if(args[0] == NULL) {
            continue; 
        }

        if(strcmp(args[0], "exit") == 0) {
            printf("Exiting shell...\n");
            exit(0);
        }
        pid_t pid = fork();
        
        // If the process is the child process execute the command, else wait for the child process to end
        if(pid == 0) {
            execvp(args[0], args);
            perror("Running command failed");
            exit(1);
        } else {
            wait(NULL);

        }
    }
    return 0;
}