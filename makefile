# Cible par défaut
all:
	gcc -o aurora aurora.c

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f aurora *.o