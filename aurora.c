#include "bin/tar/tar_header.c"
#include "bin/command/command.c"

int main(int argc, char *argv[]) {
    // Paramètres disponibles.
    int list = 0;
    int extract = 0;
    int create = 0;
    int directory = 0;
    int compress = 0;
    int verbose = 0;
    int help = 0;

    // Fichier utilisé.
    FILE *file;

    // Traitement des options de commande.
    process_options(argc, argv, &list, &extract, &create, &directory, &compress, &verbose, &help);

    // Si une demande d'aide a été faite.
    if (help) {
        help_option();
        return 0;
    }

    // Récupération et ouverture du fichier tar.
    file = open_tar_file(argv[2]);

    // Si une demande de listing a été faite.
    if (list) {
        print_tar_entries(file);
    }

    // Si une demande d'extraction a été faite.
    if (extract)
    {

    }

    return 0;
}
