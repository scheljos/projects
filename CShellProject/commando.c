#include "commando.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0); // Turn off output buffering

  cmdcol_t *col = (cmdcol_t *) malloc(sizeof(cmdcol_t)); // Allocate memory for cmdcol_t struct
  col->size = 0; // Initialize struct size attribute to 0

  while(1) {
    char user_command[MAX_LINE]; // Buffer for user input

    printf("@> ");

    if (fgets(user_command, MAX_LINE, stdin) == NULL) {
      printf("\nEnd of input");
      break;
    } // Uses fgets() to fill buffer; if NULL, break loop

    if ((argc >= 2 && strcmp(argv[1], "--echo") == 0)
     || getenv("COMMANDO_ECHO") != NULL) {
      printf("%s", user_command);
    } // Check --echo or COMMAND_ECHO environment variable

    // Set up arguments for parse_into_tokens() call
    char *tokens[MAX_LINE];
    int *ntok;
    int num = 0;
    ntok = &num;

    parse_into_tokens(user_command, tokens, ntok); // Parse user_command into tokens

    if (*ntok == 0) {
      cmdcol_update_state(col, NOBLOCK);
      continue;
    } // If no arguments given, then just continue getting new input

    if (*ntok == 1 && strncmp(tokens[0], "help", 4) == 0) { // help command
      printf("COMMANDO COMMANDS\n");
      printf("help               : show this message\n");
      printf("exit               : exit the program\n");
      printf("list               : list all jobs that have been started giving information on each\n");
      printf("pause nanos secs   : pause for the given number of nanseconds and seconds\n");
      printf("output-for int     : print the output for given job number\n");
      printf("output-all         : print output for all jobs\n");
      printf("wait-for int       : wait until the given job number finishes\n");
      printf("wait-all           : wait for all jobs to finish\n");
      printf("command arg1 ...   : non-built-in is run as a job\n");

    } else if (*ntok == 1 && strncmp(tokens[0], "exit", 4) == 0) { // exit command
      break; // exits infinite while loop, where it deallocates memory

    } else if (*ntok == 1 && strncmp(tokens[0], "list", 4) == 0) { // list command
      cmdcol_print(col);

    } else if (*ntok == 3 && (strncmp(tokens[0], "pause", 5) == 0)) { // pause command
      pause_for(atoi(tokens[1]), atoi(tokens[2]));

    } else if (*ntok == 2 && (strncmp(tokens[0], "output-for", 10) == 0)) { // output-for int command
      if (atoi(tokens[1]) < col->size) { // if the desired job is valid
        printf("@<<< Output for %s[#%d] (%d bytes):\n", col->cmd[atoi(tokens[1])]->name, col->cmd[atoi(tokens[1])]->pid, col->cmd[atoi(tokens[1])]->output_size);
        printf("----------------------------------------\n");
        cmd_print_output(col->cmd[atoi(tokens[1])]);
        printf("----------------------------------------\n");
      } else {
        printf("Error: job number does not exist\n");
      }

    } else if (*ntok == 1 && (strncmp(tokens[0], "output-all", 10) == 0)) { // output-all command
      for (int i = 0; i < col->size; i++) {
        printf("@<<< Output for %s[#%d] (%d bytes):\n", col->cmd[i]->name, col->cmd[i]->pid, col->cmd[i]->output_size);
        printf("----------------------------------------\n");
        cmd_print_output(col->cmd[i]);
        printf("----------------------------------------\n");
      }

    } else if (*ntok == 2 && (strncmp(tokens[0], "wait-for", 8) == 0)) { // wait-for int command
      if (atoi(tokens[1]) < col->size) {
        int x = atoi(tokens[1]); // specifies which command in col to update
        cmd_update_state(col->cmd[x], DOBLOCK); // updates the state of one specific command
      } else {
        printf("Error: job number does not exist\n");
        continue;
      }

    } else if (*ntok == 1 && (strncmp(tokens[0], "wait-all", 8) == 0)) { // wait-all command
      for (int i = 0; i < col->size; i++) {
        cmd_update_state(col->cmd[i], DOBLOCK);
      } // updates the state of every command in col

    } else { // create new command, not one of the built-ins
      cmd_t* cmd = cmd_new(tokens);  // Create command
      cmdcol_add(col, cmd); // Add to column
      cmd_start(cmd); // Start command
    }

    // Update state of all child processes: cmdcol_update_state();
    cmdcol_update_state(col, NOBLOCK);

  } // main while loop

  cmdcol_freeall(col); // free the commands from col
  free(col); // free col struct itself
  return 0;

} // main
