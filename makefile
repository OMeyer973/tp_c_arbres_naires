MAKEFLAGS += --silent

Lexique : Lexique.c Lexique.h
	gcc -c Lexique.c -Wall && echo " ------------------ \n fichier Lexique compilé. \n exécuter  avec Lexique 'fichier' pour exécuter sur le 'fichier' et afficher le menu. \n sinon les options disponibles sont -l, -s, -S, -r 'mot', -d, -D\n Exemple : Lexique -l mots\n ------------------ "