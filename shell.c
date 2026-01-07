#include "shell.h"

int main(int argc, char* argv[]) {
    for (int i = 0; i < 20; i++) {
        printf("*");
    }
    printf("Welcome to my shell")
    for (int i = 0; i < 20; i++) {
        printf("*");
    }


    while(1) {
        int pid = fork();

        // If the process is the child process execute the command, else wait for the child process to end
        if(pid == 0) {
            if(strcmp(args[0]), "exit" == 0) {
                exit(0);
            }
            execvp(arg[0], argv);
            perror("Running command failed");
            exit(1);
	} else {
            wait(NULL);
        }
    }
}

