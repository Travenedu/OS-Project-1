#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128
#define PATH_MAX 4096
#define MAX_ENV_VAR_NAME_LENGTH 64
#define MAX_ENV_VAR_VALUE_LENGTH 1024

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;

void cancelHandler(int signum){
  printf("\n");
}

void handler(int signum){
  exit(0);
}



int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
    char cwd[PATH_MAX];

    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];

    //define this variable
  
    while (true) {
      memset(arguments, 0, MAX_COMMAND_LINE_ARGS);
        do{ 
            // Print the shell prompt.
            getcwd(cwd, sizeof(cwd));
            printf("%s%s>", cwd, prompt);
            fflush(stdout);
            
          
// cd: changes the current working directory
          
// pwd: prints the current working directory
      //if ()
// echo: prints a message and the values of environment variables
// exit: terminates the shell
// env: prints the current values of the environment variables
// setenv: sets an environment variable
            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
              }
 
        } while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory DONE
			  
			
        // 1. Tokenize the command line input (split it on whitespace)
        command_line[strlen(command_line) -1] = '\0';

        int i = 0;
        bool verification = false;
        int argumentCount = 0;
        char* input = strtok(command_line, delimiters);
        char copy[MAX_COMMAND_LINE_LEN];
        char retrieveVal[MAX_COMMAND_LINE_LEN];
        char* next;

        
        while (true){
          if(input != NULL){
            if(input[0] == '$' && strlen(input) > 1 && argumentCount > 0){
              strcpy(copy, input + 1);
              verification = true;
              char* enviroValue = getenv(copy);
              if(enviroValue == NULL){
                strcpy(retrieveVal, "");
              } else {
              strcpy(retrieveVal, enviroValue);
              }
            }
          next = (char*) malloc(MAX_COMMAND_LINE_LEN * sizeof(char));
          strcpy(next, verification ? retrieveVal : input);
          arguments[i++] = next;
          input = strtok(NULL, delimiters);
          ++argumentCount;

          } else {
            break;
          }
        }

        // 2. Implement Built-In Commands
        if (strcmp(arguments[0], "cd") == 0){
          chdir(arguments[1]);
        }
          
        else if (strcmp(arguments[0], "pwd") == 0){
          printf("%s\n", prompt);
        }
          
        else if (strcmp(arguments[0], "echo") == 0){  
          int i = 1;
          while (arguments[i] != NULL){
            if (*arguments[i] == '$'){
              printf("%s\n", getenv((arguments[i] + 1)));
            }
            else {
            printf("%s ", arguments[i]);
            }
            
            i++;
          }
          printf("\n");
        }
          
        else if (strcmp(arguments[0], "exit") == 0){
          exit(0);
        }
          
        else if (strcmp(arguments[0], "env") == 0){
          printf("%s\n", getenv(arguments[1]));
        }
          
        else if (strcmp(arguments[0], "setenv") == 0){
          char name[MAX_ENV_VAR_NAME_LENGTH];
          char value[MAX_ENV_VAR_VALUE_LENGTH];
          char* p = strtok(arguments[1], "=");
          strcpy(name,p);
          while(p != NULL){
            strcpy(value, p);
            p = strtok(NULL, "=");
          }
          setenv(name, value, 1);
        } else if (strcmp(arguments[0], "env") == 0){
          if (argumentCount == 1){
            char ** envs = environ;
            for (;* envs; envs++){
              printf("%s\n", * envs);
            }
          }else {
            char * envValue = getenv(arguments[1]);
            if (envValue != NULL){
              printf("%s\n", envValue);
            }
          }
        }
        // 3. Create a child process which will execute the command line input
        else {
          pid_t pid = fork();
          char* cmd = arguments[0];
          int background = 0;

          if(strcmp(arguments[argumentCount - 1], "&") == 0){
            arguments[argumentCount - 1] = NULL;
            background = 1;
          }
            if (pid == 0){
              signal(SIGALRM, handler);
              alarm(10);
              if (background){//MAYBE?
                int fd = open("/dev/null", O_WRONLY);
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
              }
              if (execvp(cmd, arguments) == -1){
                printf("execvp() failed: No such file or directory. An error has occurred\n");
              }
              alarm(0);
              exit(0);
            } else {
              if (background == 0){
                signal(SIGINT, cancelHandler);
                waitpid(pid, NULL, WUNTRACED);
              }
            }
          }
      
        // 4. The parent process should wait for the child to complete unless its a background process
            /*
            printf("i am parent of %d\n", pid);
            int status; 
            struct rusage ru;
            pid_t child_pid = wait4(pid, &status, 0, &ru);
            if(child_pid < 0) {
              perror("wait4 failed");
            } else {
              printf("child exited with %d\n", status);
            }
            exit(0);
            */
    }
            return -1;
}
            

        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes

    // This should never be reached.
