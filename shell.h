#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
<<<<<<< HEAD
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
=======
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <linux/limits.h>
>>>>>>> 0714c49 (Finished implementation of cd command.)

#define MAX_LINE 1024
#define MAX_ARGS 64

void parseInput(char* input, char** args);

#endif