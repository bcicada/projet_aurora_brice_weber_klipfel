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

void extract_tar(FILE *fp, tar_header *header) {
    int size = strtol(header->size, NULL, 8);
    int blocks = (size / BLOCK_SIZE) + (size % BLOCK_SIZE != 0);

    // Créer un fichier pour stocker les données extraites
    FILE *out_fp = fopen(header->name, "w");
    if (out_fp == NULL) {
        perror("Erreur : Impossible de créer le fichier de sortie\n");
        exit(1);
    }

    // Allouer de la mémoire pour les données
    char *buf = (char *)malloc(size);
    if (buf == NULL) {
        perror("Erreur : Impossible d'allouer de la mémoire\n");
        exit(1);
    }

    // Lire les données dans le fichier tar
    if (fread(buf, size, 1, fp) != 1) {
        perror("Erreur : Impossible de lire les données de l'archive\n");
        exit(1);
    }

    // Écrire les données dans le fichier de sortie
    if (fwrite(buf, size, 1, out_fp) != 1) {
        perror("Erreur : Impossible d'écrire les données dans le fichier de sortie\n");
        exit(1);
    }

    // Libérer la mémoire allouée
    free(buf);

    // Fermer le fichier de sortie
    if (fclose(out_fp) != 0) {
        perror("Erreur : Impossible de fermer le fichier de sortie\n");
        exit(1);
    }

    // Avancer le curseur pour passer au prochain en-tête de l'archive
    fseek(fp, blocks * BLOCK_SIZE, SEEK_CUR);
}