#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "tar_header.h"

void print_header(tar_header *header) {
    if (header->name[0] != '\0') {
        printf("Nom : %s\n", header->name);
        printf("Taille : %s\n", header->size);
        printf("Date de modification : %s\n", header->mtime);
        printf("Type : %s\n\n", header->typeflag);
    }
}