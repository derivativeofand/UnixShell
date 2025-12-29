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
    char cwd[PATH_MAX];
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
        printf("\nmyShell: %s$ ", getcwd(cwd, sizeof(cwd)));
        
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
        
        // Exit command
        if(strcmp(args[0], "exit") == 0) {
            printf("Exiting shell...\n");
            exit(0);
        } else if(strcmp(args[0], "cd") == 0) {
    
                // If the cd command is given without any arguments, change to home directory
                if(args[1] == NULL) {
    
                    // Gets the absolute path of home directory
                    char *home = getenv("HOME");
    
                    // If the HOME environment variable is not set, default to root directory
                    if(home == NULL) {
                        home = "/";
                    }
                    
                    if(chdir(home) != 0) {
                        perror("chdir failed");
                    }
    
                } else {
                    if(chdir(args[1]) != 0) {
                        perror("chdir failed");
                    } 
                }

        } else if(strcmp(args[0], "pwd") == 0) {
            printf("%s\n", getcwd(cwd, sizeof(cwd)));
        } else {
            // If the process is the child process execute the command, else wait for the child process to end
            pid_t pid = fork();
<<<<<<< HEAD
            int fd;
            // Handling input redirection
            if(args[2] && strcmp(args[1], "<") == 0) {
                // Removing the redirection part from args
                printf("%s\n", args[2]);
                args[1] = NULL; 

                // Opening the file 
                fd = open(args[2], O_RDONLY);
                if(fd < 0) {
                    perror("open failed");
                    continue;
                }
                
                // Replacing the stdin with the new file descriptor, fd
                dup2(0, fd);
                close(fd);
            }
=======
>>>>>>> 0714c49 (Finished implementation of cd command.)
            if(pid == 0) {
                execvp(args[0], args);
                perror("Running command failed");
                exit(1);
            } else {
                wait(NULL);
<<<<<<< HEAD
=======

>>>>>>> 0714c49 (Finished implementation of cd command.)
            }
        }
    }
    return 0;
}