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


/**
 * @brief Traite les options en ligne de commande.
 * @param argc Le nombre d'arguments en ligne de commande.
 * @param argv Les arguments en ligne de commande.
 * @param list Pointeur vers une variable booléenne pour indiquer si l'option "-l" ou "--list" a été fournie.
 * @param extract Pointeur vers une variable booléenne pour indiquer si l'option "-e" ou "--extract" a été fournie.
 * @param create Pointeur vers une variable booléenne pour indiquer si l'option "-c" ou "--create" a été fournie.
 * @param directory Pointeur vers une variable booléenne pour indiquer si l'option "-d" ou "--directory" a été fournie.
 * @param compress Pointeur vers une variable booléenne pour indiquer si l'option "-v" ou "--compress" a été fournie.
 * @param verbose Pointeur vers une variable booléenne pour indiquer si l'option "-v" ou "--verbose" a été fournie.
 * @param help Pointeur vers une variable booléenne pour indiquer si l'option "-h" ou "--help" a été fournie.
 *
 * Cette fonction utilise getopt() pour traiter les options en ligne de commande. 
 * Les options disponibles sont "-l" ou "--list", "-e" ou "--extract", "-c" ou "--create", "-d" ou "--directory", "-v" ou "--verbose", et "-h" ou "--help". 
 * Les variables booléennes correspondantes seront mises à jour en conséquence.
 * Si une option non valide est trouvée, un message d'erreur sera affiché et le programme quittera avec un code d'erreur.
*/
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

/**
 * @brief Affiche les options d'aide.
 *
 * Les descriptions détaillées pour chaque option sont affichées à l'aide de printf().
 */
void help_option() {
    printf("-l, --list ARCHIVE_FILE\n");
    printf("-e, --extract ARCHIVE_FILE\n");
    printf("-c, --create ARCHIVE_FILE\n");
    printf("-d, --directory DIRECTORY_TO_PROCESS\n");
    printf("-z, --compress\n\n");

    printf("-v, --verbose : enable *verbose* mode\n");
    printf("-h, --help : display this help\n");
}

/**
 * @brief Ouvre un fichier tar en mode binaire en lecture.
 *
 * Cette fonction ouvre un fichier tar en mode binaire en lecture, en utilisant `fopen()`. 
 * Si l'ouverture du fichier échoue, la fonction traite l'erreur en utilisant `errno` et affiche un message d'erreur approprié.
 *
 * @param filename Le nom du fichier tar à ouvrir.
 * @return Le pointeur sur le fichier ouvert en cas de succès, `NULL` en cas d'échec.
 */
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

/**
 * @brief Affiche les entrées du fichier tar.
 *
 * Cette fonction lit et affiche les entrées du fichier tar passé en paramètre. 
 * Pour chaque entrée, elle appelle la fonction `print_header` pour afficher les informations sur l'entrée.
 *
 * @param file Pointeur sur le fichier tar ouvert.
 */
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

/**
 * @brief Extrait les entrées d'un fichier tar
 *
 * La fonction `extract_tar` lit les entrées d'un fichier tar et les extrait 
 * dans un dossier donné. Si le paramètre `verbose` est non nul, les noms des 
 * dossiers et fichiers extraits seront affichés.
 *
 * @param file Pointeur sur le fichier tar
 * @param path Chemin vers le dossier où les entrées seront extraites
 * @param verbose Pointeur sur un entier, 0 pour désactiver l'affichage, non nul pour l'activer
 */
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