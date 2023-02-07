#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "tar_header.h"

/**
 * @brief Imprime les informations de l'entrée tar.
 * 
 * @param header Pointeur vers la structure `tar_header` qui contient les informations à imprimer.
 * 
 * Cette fonction imprime le nom, la taille, la date de modification et le type de l'entrée tar
 * pointée par `header`. Si le nom est vide (null-terminated string), la fonction ne fait rien.
 */
void print_header(tar_header *header) {
    if (header->name[0] != '\0') {
        printf("Nom : %s\n", header->name);
        printf("Taille : %s\n", header->size);
        printf("Date de modification : %s\n", header->mtime);
        printf("Type : %s\n\n", header->typeflag);
    }
}