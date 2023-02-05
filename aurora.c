#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "config/tar_header/tar_header.c"


int main(int argc, char *argv[]) {
    int opt;

    // Options disponibles.
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
        case 'f':
            break;
        default:
            fprintf(stderr, "Utilisation : %s [-f <archive.tar>]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Vérification de l'existence de l'option -f.
    if (optind >= argc) {
        fprintf(stderr, "Erreur : Option -f <archive.tar> manquante\n");
        exit(EXIT_FAILURE);
    }

    // Ouverture du fichier tar.
    FILE *fp;
    errno = 0;
    fp = fopen(argv[optind], "r");
    if (fp == NULL) {
        perror("Erreur : Impossible d'ouvrir ce fichier");
        exit(EXIT_FAILURE);
    }
    tar_header header;

    // Boucle pour lire les en-têtes d'archives.
    while (fread(&header, BLOCK_SIZE, 1, fp) == 1) {
        if (header.name[0] == '\0') {
            break;
        }
        print_header(&header);
        int size = strtol(header.size, NULL, 8);
        int blocks = (size / BLOCK_SIZE) + (size % BLOCK_SIZE != 0);
        fseek(fp, blocks * BLOCK_SIZE, SEEK_CUR);
    }

    // Fermeture du fichier.
    if (fclose(fp) != 0) {
        perror("Erreur : Impossible de fermer ce fichier");
        exit(1);
    }

    return 0;
}
