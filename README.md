## Aurora 

**Version du logiciel** : 0.2.3

**Développeur** : [Brice WEBER KLIPFEL](https://github.com/bcicada) [Guillaume SCHMIDT](https://github.com/gs-cnam)

**Date de dernière mise à jour** : 2023-02-05

**Description** : Aurora est un générateur/extracteur d'archive .tar similaire à la commande éponyme tar.

**Fonctionnalités clefs** :
- ❌ Respect de la norme GNU POSIX 1003.1-1988 (ustar).
- ✅ FM01 : Le binaire est capable de lister les éléments d’une archive.
- ✅ FM02 : Le binaire est d’extraire l’intégralité d’une archive passée en paramètre.
- 🛠 FM03 : L’application est capable de générer une archive.

**Fonctionnalités clefs optionnelles** :
- ❌ FMO01 : La prise en charge de la compression d’une archive tar (via la libraire zlib).
- ❌ FMO02 : La prise en charge de la décompression d’une archive tar.gz (gzip).
- ❌ FMO03 : La réalisation d’une interface en mode console « tui » via la librairie ncurses.

**Contraintes techniques** :
- ✅ CT01 : La compilation du projet doit se faire via un Makefile.
- ✅ CT02 : La définition des structures doit se faire dans un fichier typedef.h.
- ✅ CT03 : La définition des méthodes protoype (.h) & implémentation (.c) doit se faire de manière séparée autant que faire se peut.
- ✅ CT04 : Le code produit doit être documenté.
- ✅ CT05 : La récupération des paramètres doit se faire via les fonctions usuelles getopt() / getoptlong().
- ✅ CT06 : La gestion des erreurs doit se faire via «les mécanismes proposés par errno».

**Contraintes techniques optionnelles** :
- ✅ CTO01 : La documentation du code générée via l’utilitaire doxygen.
- ❌ CTO02 : Le code est soumis à un contrôle de couverture via l’utilitaire gcov.
- ✅ CTO03 : Une page de manuel Linux est rédigée pour détailler l’exécution du binaire.
