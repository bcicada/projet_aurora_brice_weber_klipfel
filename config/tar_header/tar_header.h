#ifndef TAR_HEADER_H
#define TAR_HEADER_H

#define BLOCK_SIZE 512

/* Structure qui représente l'en-tête d'un fichier tar */

typedef struct tar_header {
  char name[100];       /* Nom du fichier */
  char mode[8];         /* Permissions du fichier */
  char uid[8];          /* Identifiant de l'utilisateur propriétaire du fichier */
  char gid[8];          /* Identifiant du groupe propriétaire du fichier */
  char size[12];        /* Taille du fichier en octets */
  char mtime[12];       /* Date de dernière modification */
  char chksum[8];       /* Somme de contrôle */
  char typeflag[1];     /* Type de l'entrée */
  char linkname[100];   /* Nom du fichier lié (si c'est un lien symbolique) */
  char magic[6];        /* En-tête de magie */
  char version[2];      /* Version de l'en-tête */
  char uname[32];       /* Nom de l'utilisateur propriétaire */
  char gname[32];       /* Nom du groupe propriétaire */
  char devmajor[8];     /* Major number du périphérique (si typeflag est 0 ou 5) */
  char devminor[8];     /* Minor number du périphérique (si typeflag est 0 ou 5) */
  char prefix[155];     /* Préfixe pour les noms de fichier tronqués */
  char padding[12];     /* Remplissage pour aligner sur bloc de 512 octets */
} tar_header;

void print_header(tar_header *header);

#endif
