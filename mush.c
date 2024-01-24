#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    while(1) {
        char input[2048]; // array for inputs
        printf("$::");
        fflush(stdout);
        fgets(input, sizeof input, stdin);

        char *token;  // token for strtok
        char *args[128]; // array for arguments
        int i = 0;

        // Loop for storing the input tokenized in the args
        token = strtok(input, " \t\n\r");
        while (token != NULL && i < 128) {
            args[i++] = token;
                token = strtok(NULL, " \t\n\r");
        }
        args[i] = NULL;

        // Handling the running of commands
        // Checks if cd is first argument
        if (strcmp(args[0], "cd") == 0) {
            // if args[1] is Null print an error
            if (args[1] == NULL) {
                printf("cd: expected argument");
            } else if (chdir(args[1]) == -1) {
            perror("cd");
            }
        // Checks for exit
        } else if (strcmp(args[0], "exit") == 0) {
            exit(0);

        // forks the process to run a command. 
        } else {
            pid_t pid = fork();

            if (pid == 0) {
                execvp(args[0], args);
                perror("exec error");
                exit(1);
            } else {
                wait(NULL);
            }
        }
    }   
}