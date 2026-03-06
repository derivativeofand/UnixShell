#include "shell.h"


void parseInput(char* input, char** args) {
    int in_quotes = 0;      
    char quote_char = 0;
    int i = 0;
    char *p = input;
    char *start = NULL;

    while (*p) {
        // Skip leading spaces/tabs/newlines
        while (*p == ' ' || *p == '\t' || *p == '\n')
            p++;

        if (*p == '\0')
            break;

        start = p;

        if (*p == '"' || *p == '\'') {
            /* quoted token, either "..." or '...' */
            quote_char = *p;
            start = ++p;             
            while (*p && *p != quote_char)
                p++;

            if (*p) {
                *p = '\0';
                p++;                     
            }
            args[i++] = start;
        } else if (*p == '<' || *p == '>') {
            /* redirection operator (<, > or >>) */
            start = p;
            p++;
            if (*start == '>' && *p == '>')
                p++;

            if (*p) {
                *p = '\0';
                p++;
            }
            args[i++] = start;
        } else {
            // Unquoted token
            start = p;
            while (*p && *p != ' ' && *p != '\t' && *p != '\n' &&
                   *p != '<' && *p != '>')
                p++;

            if (*p) {
                *p = '\0';
                p++;
            }
            args[i++] = start;
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
        
        // Print args components
        for (int i = 0; args[i] != NULL; i++) {
            printf("args[%d]: %s\n", i, args[i]);
        }
        
        // Empty command
        if(args[0] == NULL) {
            continue; 
        }
        

        int redirect_index = -1;
        for (int i = 1; args[i] != NULL; i++) {
            if(strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {
                redirect_index = i;
                break;
            }
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
                char *redirect_op = NULL;

                // Save the redirection operator before truncating args
                if (redirect_index != -1) {
                    redirect_op = args[redirect_index];
                    args[redirect_index] = NULL;
                }

                // Input/Ouput redirection handling
                if (redirect_index != -1) {
                    if(strcmp(redirect_op, "<") == 0 &&
                        args[redirect_index + 1] != NULL) {
                        fd = open(args[redirect_index + 1], O_RDONLY);
                        if (fd < 0) {
                            perror("open failed");
                            exit(1);
                        }

                        if (dup2(fd, 0) < 0) {
                            perror("dup2 failed");
                            exit(1);
                        }
                        close(fd);
                    } else if(strcmp(redirect_op, ">") == 0 && args[redirect_index + 1] != NULL) {
                        fd = open(args[redirect_index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd < 0) {
                            perror("open failed");
                            exit(1);
                        }

                        if (dup2(fd, 1) < 0) {
                            perror("dup2 failed");
                            exit(1);
                        }
                        close(fd);
                    } else if(strcmp(redirect_op, ">>") == 0 && args[redirect_index + 1] != NULL) {
                        fd = open(args[redirect_index + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                        if (fd < 0) {
                            perror("open failed");
                            exit(1);
                        }

                        if (dup2(fd, 1) < 0) {
                            perror("dup2 failed");
                            exit(1);
                        }
                        close(fd);
                    } 
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