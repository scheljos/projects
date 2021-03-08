#include "commando.h"
#include <unistd.h>

// cmd.c: functions related the cmd_t struct abstracting a
// command. Most functions maninpulate cmd_t structs.

cmd_t *cmd_new(char *argv[]) {
    cmd_t *cmd = (cmd_t *) malloc(sizeof(cmd_t)); // Initializes new cmd_t object
    strcpy(cmd->name, argv[0]); // Initializes name
    for (int i = 0; argv[i] != NULL; i++) { // Initializes argv array
      cmd->argv[i] = strdup(argv[i]);
      if (argv[i+1] == NULL) {
        cmd->argv[i+1] = NULL; // NULL terminates the cmd->argv array
      }
    }
    cmd->pid = -1; // default pid
    cmd->out_pipe[0] = -1; // default out_pipe in read fd
    cmd->out_pipe[1] = -1; // default out_pipe in write fd
    cmd->finished = 0; // Initialized to 0 because cmd just started
    cmd->status = -1; // default status -1, changes via cmd_update_state
    snprintf(cmd->str_status, 5, "INIT"); // string value for status
    cmd->output = NULL; // No output yet; cmd just started
    cmd->output_size = -1; // 0 bytes in output to begin with (because NULL)
    return cmd; // returns new cmd
}

void cmd_free(cmd_t *cmd) {
    for (int i = 0; cmd->argv[i] != NULL; i++) {
      free(cmd->argv[i]); // deallocates all elements of cmd->argv
    }
    if (cmd->output != NULL) {
        free(cmd->output); // deallocates the output buffer if not null
    }
    free(cmd); // deallocates the cmd object itself
}

void cmd_start(cmd_t *cmd) {
  pipe(cmd->out_pipe); // makes the pipe with cmd->out_pipe
  snprintf(cmd->str_status, 4, "RUN"); // updates str_status to run
  pid_t child_pid = fork(); // fork into child and parent
  if (child_pid == 0) { // if (child)
    close(cmd->out_pipe[0]); // close read end of pipe
    dup2(cmd->out_pipe[1], STDOUT_FILENO); // change output of child process to be write end pipe
    execvp(cmd->name, cmd->argv); // execute the command
  } else if (child_pid > 0) { // else if (parent)
    cmd->pid = child_pid; // save child process ID in cmd object
    close(cmd->out_pipe[1]); // close write end of pipe
  } else {
    printf("Error: could not create child process");
    exit(1);
    // if child_pid < 0, there's been some sort of error. Abort!
  }

}

void cmd_update_state(cmd_t *cmd, int block) {
  if (cmd->finished == 0) { // if the command has not finished, proceed
    int status;
    pid_t pid = waitpid(cmd->pid, &status, block); // wait for child, with given block code
    if (pid == -1) {
      exit(1); // waitpid() returns -1, means error
    } else if (pid != 0) { // state change occurred
      if (WIFEXITED(status)) { // check to see if child process exited
        cmd->status = WEXITSTATUS(status); // save status after exit
        snprintf(cmd->str_status, 8, "EXIT(%d)", cmd->status); // Set str_status
        cmd->finished = 1; // Process finished, so update cmd's finished field
        cmd_fetch_output(cmd); // Read contents of pipe into output buffer
        printf("@!!! %s[#%d]: %s\n", cmd->name, (int) cmd->pid, cmd->str_status); // Message for user
      }
    }
  }
}

char *read_all(int fd, int *nread) { // Some code in this function copied from hw03-code's append-all.c!
  int cur_pos = 0;
  int max_size = 1024;
  char *buffer = (char*)malloc(max_size*sizeof(char));
  memset(buffer, '\0', max_size*sizeof(char)); // initialize new memory

  while(1) { // infinite loop
    if(cur_pos >= max_size) {
      max_size = max_size * 2;
      buffer = realloc(buffer, max_size); // resize buffer if more space needed
      memset(buffer+cur_pos, '\0', max_size / 2); // initialize new memory
      if(buffer == NULL) { // exit if buffer became NULL after realloc()
        printf("realloc failed; exit(1)\n");
        exit(1);
      }
    }
    int max_read = max_size - cur_pos; // Maximum we can read is the size of buffer minus cur_pos
    *nread = read(fd, buffer+cur_pos, max_read); // read from arg fd into buffer
    if(*nread == 0) { // end of input
      *nread = cur_pos; // *nread is how many bytes were read
      if (max_read == 0) {
        buffer = realloc(buffer, max_size + 1); // realloc if no space in buf for terminator
      }
      *(buffer + cur_pos + 1) = '\0'; // add null termintor
      break; // break infinite loop
    }
    else if(*nread == -1) {
      perror("read failed");
      exit(1);
    }

    cur_pos += *nread; // update cur_pos with the amount of bytes read in this iteration
  }
  return buffer; // return output buffer
}

void cmd_fetch_output(cmd_t *cmd) {
  if (cmd->finished == 0) { // Output not ready if not finished
    printf("%s[#%d] not finished yet\n", cmd->name, (int) cmd->pid);
  } else {
    int *nread;
    int x = 0;
    nread = &x; // make nread for read_all(), initialize to 0
    // again, nread is how many bytes read into the buffer
    cmd->output = read_all(cmd->out_pipe[0], nread); // create cmd->output via read_all()
    // use read end of out_pipe
    cmd->output_size = *nread; // *nread updated in read_all(), so now set it to output_size
    close(cmd->out_pipe[0]); // close read end pipe
  }
}

void cmd_print_output(cmd_t *cmd) {
  if (cmd->output == NULL) { // No output available yet
    printf("%s[#%d] : output not ready\n", cmd->name, (int) cmd->pid);
  } else {
    int nwrite = write(STDOUT_FILENO, cmd->output, cmd->output_size); // write the output to the terminal
    if (nwrite == -1) {
      printf("write failed");
      exit(1);
    }
  }
}
