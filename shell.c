#include "shell.h"


void parseInput(char* input, char** args) {
    char *token;
    int i = 0;

    token = strtok(input," \t\n");
    while(token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
        if(token == '"') {
            while()
            token = strtok(input, '"');
        }
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
    printf("\n");

    while(1) {
        printf("myShell: %s$ ", getcwd(cwd, sizeof(cwd)));
        
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
            if(pid == 0) {
                int fd;
                // Handling input redirection
                if(args[2] && strcmp(args[1], "<") == 0) {
                    
                    // Opening the file 
                    fd = open(args[2], O_RDONLY);
                    if(fd < 0) {
                        perror("open failed");
                        continue;
                    }
                    
                    // Replacing the stdin with the new file descriptor, fd
                    if(dup2(fd, 0) < 0) {
                        perror("dup2 failed");
                        continue;
                    }
                    close(fd);
                    args[1] = NULL;
                    args[2] = NULL;
                } else if(args[2] && strcmp(args[1], ">") == 0) {
                    fd = open(args[2], O_WRONLY | O_CREAT | O_EXCL | O_TRUNC);
                    if(fd < 0) {
                        perror("open failed");
                        continue;
                    }                    
                    
                    if(dup2(fd, 1) < 0) {
                        perror("dup2 failed");
                        continue;
                    }
                    close(fd);

                    args[1] = NULL;
                    args[2] = NULL;

                }
                execvp(args[0], args);
                perror("Running command failed");
                exit(1);
            } else {
                wait(NULL);

            }
        }
    }
    return 0;
}