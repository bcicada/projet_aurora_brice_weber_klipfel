#include <stdio.h>
#include "tar_header.h"

void print_header(tar_header *header) {
    printf("Nom             : %s\n", header->name);
    printf("Taille          : %s\n", header->size);
    printf("Typeflag        : %s\n", header->typeflag);
    printf("Lien symbolique : %s\n\n", header->linkname);
}