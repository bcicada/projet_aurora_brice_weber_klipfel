#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <tar.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>
#include "command.h"
#include "../tar/tar_header.h"

void process_options(int argc, char *argv[], int *list, int *extract, int *create, int *directory, int *compress, int *verbose, int *help) {
    int opt;

    // Options disponibles.
    while ((opt = getopt(argc, argv, "lecdvh")) != -1) {
        switch (opt) {
        case 'l':
            *list = 1;
            break;
        case 'e':
            *extract = 1;
            break;
        case 'c':
            *create = 1;
            break;
        case 'd':
            *directory = 1;
            break;
        case 'v':
            *verbose = 1;
            break;
        case 'h':
            *help = 1;
            break;
        default:
            fprintf(stderr, "Utilisation : %s [-l | --list] [-e | --extract] [-c | --create] [-d | --directory] [-v | --verbose] [-h | --help]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}

void help_option() {
    printf("-l, --list ARCHIVE_FILE\n");
    printf("-e, --extract ARCHIVE_FILE\n");
    printf("-c, --create ARCHIVE_FILE\n");
    printf("-d, --directory DIRECTORY_TO_PROCESS\n");
    printf("-z, --compress\n\n");

    printf("-v, --verbose : enable *verbose* mode\n");
    printf("-h, --help : display this help\n");
}

FILE *open_tar_file(const char *filename) {
    FILE *fp;

    /* Ouvre le fichier tar en lecture binaire */
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        /* Traite l'erreur en utilisant errno */
        switch (errno) {
        case ENOENT:
            printf("Le fichier %s n'existe pas.\n", filename);
            break;
        case EACCES:
            printf("Vous n'avez pas les droits d'accès au fichier %s.\n", filename);
            break;
        default:
            printf("Erreur inconnue lors de l'ouverture du fichier %s.\n", filename);
            break;
        }
        return NULL;
    }

    /* Le fichier a été ouvert avec succès */
    return fp;
}

void print_tar_entries(FILE *file) {
    int num_read; 
    tar_header t_header;

    // Boucle sur toutes les entrées dans le fichier tar.
    while ((num_read = fread(&t_header, sizeof(tar_header), 1, file)) > 0) {
        // Afficher l'entrée courante.
        print_header(&t_header);

        // Sauter la taille du fichier pour aller à la prochaine entrée.
        int size = strtol(t_header.size, NULL, 8);
        fseek(file, (size + 511) & ~511, SEEK_CUR);
    }
}

void extract_tar(FILE *file, const char *path, int *verbose) {
    tar_header t_header;
    int size;
    char file_path[512];
    
    while (fread(&t_header, sizeof(tar_header), 1, file) == 1) {
        // Calculer la taille du fichier en octets
        sscanf(t_header.size, "%o", &size);

        // Créer le chemin complet du fichier ou dossier
        sprintf(file_path, "%s/%s", path, t_header.name);

        // Si le typeflag est d (directory), créer le dossier
        if (t_header.typeflag[0] == '5') {
            mkdir(file_path, 0755);
            if (verbose) {
                printf("Dossier extrait : %s\n", file_path);
            }
        } else {
            // Sinon, extraire le fichier
            FILE *out = fopen(file_path, "wb");

            if (out == NULL) {
                perror("fopen");
                return;
            }

            for (int i = 0; i < size; i++) {
                fputc(fgetc(file), out);
            }

            fclose(out);

            if (verbose) {
                printf("Fichier extrait : %s\n", file_path);
            }
        }
        
        // Aligner la position du fichier sur un multiple de 512 octets
        int rest = size % 512;
        if (rest > 0) {
            fseek(file, 512 - rest, SEEK_CUR);
        }
    }
}