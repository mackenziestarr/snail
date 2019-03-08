// @todo finish the provides thinger
#include <stdlib.h> // provides EXIT_SUCCESS, EXIT_FAILURE, pid_t, size_t, ssize_t
#include <stdio.h>  // provides getline, stdout, stdin
#include <unistd.h> // provides fork, waitpid
#include <string.h>

#define SHELL_NAME "snail"
#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIMITER " "

void launch(char** args);

int main() {
  char*   line = NULL;
  size_t  size = 0;
  char*   token;
  char**  token_list;
  size_t  token_count;
  size_t  buffer_size = TOKEN_BUFFER_SIZE;
  ssize_t length;

  while (1) {
    // read line
    fputs("$ ", stdout);
    length = getline(&line, &size, stdin);
    line[length - 1] = '\0';
    token_list = malloc(sizeof(char*) * buffer_size);
    token_count = 0;
    while ((token = strtok_r(line, TOKEN_DELIMITER, &line))) {
      token_list[token_count++] = token;
      if (token_count >= TOKEN_BUFFER_SIZE) {
        buffer_size += TOKEN_BUFFER_SIZE;
        token_list = realloc(token_list, buffer_size);
        if (!token_list) {
          perror(SHELL_NAME);
          free(token_list);
          continue;
        }
      }
    }

    launch(token_list);
    // evaluate
    // - builtins cd, exit
    // - programs
    /* free(line); */
  }

  return EXIT_SUCCESS;
}

void launch(char** args) {
  pid_t pid, wait_pid;
  int status;
  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror(SHELL_NAME);
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror(SHELL_NAME);
  } else {
    do {
      wait_pid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
}
