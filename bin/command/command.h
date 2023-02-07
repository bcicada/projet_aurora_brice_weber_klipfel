#ifndef COMMAND_H
#define COMMAND_H

void process_options(int argc, char *argv[], int *list, int *extract, int *create, int *directory, int *compress, int *verbose, int *help);
void help_option();
FILE *open_tar_file(const char *filename);
void print_tar_entries(FILE *file);
void extract_tar(FILE *file, const char *path, int *verbose);

#endif 
