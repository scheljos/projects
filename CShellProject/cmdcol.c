#include "commando.h"

// cmdcol.c: functions related to cmdcol_t collections of commands.

void cmdcol_add(cmdcol_t *col, cmd_t *cmd) {
    if (((col->size) + 1) == MAX_CMDS) {
        printf("Error: Exceeded maximum number of supported commands");
    } else {
        col->cmd[col->size++] = cmd; // Add cmd, increment size
    }
} // Add the given cmd to the col structure

void cmdcol_print(cmdcol_t *col) {
    printf("JOB  #PID      STAT   STR_STAT OUTB COMMAND\n"); // intro line
    for (int i = 0; i < col->size; i++) {
        char command[MAX_LINE] = "";
        int j = 0;
        while(col->cmd[i]->argv[j] != NULL) {
            strcat(command, col->cmd[i]->argv[j]);
            strcat(command, " ");
            j++;
        } // builds string of command args
        printf("%-5d#%-8d%5d%11s%5d %s\n",
        i, col->cmd[i]->pid, col->cmd[i]->status, col->cmd[i]->str_status, col->cmd[i]->output_size, command);
        // final formatted output according to writeup specifications
    }
}

void cmdcol_update_state(cmdcol_t *col, int block) {
    for (int i = 0; i < col->size; i++) {
        cmd_update_state(col->cmd[i], block);
    }
} // Update each cmd in col by calling cmd_update_state()

void cmdcol_freeall(cmdcol_t *col) {
    for (int i = 0; i < col->size; i++) {
        cmd_free(col->cmd[i]);
    }
} // Call cmd_free() on all of the constituent cmd_t's.
