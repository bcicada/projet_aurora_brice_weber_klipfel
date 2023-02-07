#ifndef COMMAND_H
#define COMMAND_H

void process_options(int argc, char *argv[], int *list, int *extract, int *create, int *directory, int *compress, int *verbose, int *help);
void help_option();

#endif 
