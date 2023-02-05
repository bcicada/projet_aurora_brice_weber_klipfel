#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tar_header.h"

void print_header(tar_header *header) {
    printf("Nom             : %s\n", header->name);
    printf("Taille          : %s\n", header->size);
    printf("Typeflag        : %s\n", header->typeflag);
    printf("Lien symbolique : %s\n\n", header->linkname);
}

void extract_file(const char *filename, int mode, int size, FILE *fp) {
    // Enregistrement des permissions d'accès actuelles.
    mode_t old_umask = umask(0);

    // Création du fichier avec les permissions spécifiées.
    int out_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (out_fd == -1) {
        perror("Impossible de créer ce fichier\n");
        exit(EXIT_FAILURE);
    }

    // Ouverture du descripteur de fichier en mode lecture.
    FILE *out_fp = fdopen(out_fd, "w");
    if (!out_fp) {
        perror("Impossible d'ouvrir ce descripteur de fichier\n");
        exit(EXIT_FAILURE);
    }

    // Extraction du contenu du fichier tar.
    char buf[BLOCK_SIZE];
    int blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    for (int i = 0; i < blocks; i++) {
        if (fread(buf, BLOCK_SIZE, 1, fp) != 1) {
            perror("Impossible de lire ce fichier\n");
            exit(EXIT_FAILURE);
        }
        if (fwrite(buf, BLOCK_SIZE, 1, out_fp) != 1) {
            perror("Impossible d'écrire ce fichier\n");
            exit(EXIT_FAILURE);
        }
    }

    // Fermeture du descripteur de fichier.
    if (fclose(out_fp) == EOF) {
        perror("Impossible de fermer ce descripteur de fichier\n");
        exit(EXIT_FAILURE);
    }

    // Restauration des permissions d'accès originales.
    umask(old_umask);
}