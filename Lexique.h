/* HARABAGIU Léa, MEYER Olivier */

//FONCTIONS GENERALES :
Arbre alloueArbre (char l);
//alloue un espace mémoire à un arbre de lettre l, filsg NULL, frered NULL et et retourne son adresse
void videArbre (Arbre a);
	//libère l'espace mémoire alloué à l'arbre a
void ajouteBranche(Arbre* a, char* mot);
	//ajoute un mot à un arbre vide (écrase l'arbre si non vide)
void ajouteMot(Arbre* a, char* mot);
	//ajoute le mot dans l'arbre lexico
Arbre ConstruitArbre(char* filename);
	// retourne l'arbre décrivant le lexique du fichier à filename
void litArbrePrefixe(Arbre* a, FILE* in);
	//créé l'arbre a à partir du fichier décrivant son parcours préfixe
Arbre chargeDico(char* filename);
	//retourne l'arbre créé à partir de son parcour préfixe décrit dans le fichier [filename].DICO
void afficheArbreLexique(Arbre a);
	//affiche dnas la console les mots de l'arbre lexico a en ordre alphabétique

//FONCTION POUR LA QUESTION 1 :
void afficheLexique(char* filename);
	//affiche dans la console les mots du fichier ayant pour chemin [filename] en ordre alphabétique
void ecritLexique(Arbre a, FILE* out);
	//ecrit dans le fichier out tous les mots de l'arbre lexico a

//FONCTION POUR LA QUESTION 2 :
void sauvegardeLexique(char* filename);
	//ecrit dans le fichier [filename].L les mots du fichier ayant pour chemin [filename] en ordre alphabétique 
int recherche(char* mot, Arbre a);
	//retourne 1 si le mot existe dans l'arbre lexico, 0 sinon

//FONCTION POUR LA QUESTION 3 :
int present(char* mot, char* filename);
	//affiche present si le mot [mot] est présent dans le document [filename]
void ecritArbrePrefixe(Arbre a, FILE* out);
	//ecrit le parcours préfixe de l'arbre a dans le fichier out

//FONCTION POUR LA QUESTION 4 :
void sauvegardeDico(char* filename);
	//ecrit dans le fichier [filename].DICO le parcours préfixe de l'arbre décrivant le lexique du fichier [filename] 

//FONCTION QUESTION 5 : rendu en dot
void ecritFgfrdDotRec(Arbre a, FILE* out);
	//ecrit le code dot représentant les noeuds de l'arbre a sous forme fils gauche frere droit dans le fichier out

void ecritFgfrdDot(char* filename);
	//ecrit le code dot représentant l'arbre a sous forme fils gauche frere droit dans le fichier de nom filename

//FONCTION POUR LA QUESTION 6 : représentation de l'arbre lexico à n fils au format dot
void ecritArbreLexDotRec(Arbre a, FILE* out, int idPere);
	//ecrit les noeuds représentant l'arbre a lexico à n fils dans le fichier dot out
void ecritArbreLexDot(char* filename);
	//ecrit le code dot représentant l'arbre a lexico à n fils dans le fichier filename

//FONCTIONS DE MENU
void commande(char *option, char* mot, char* filename);
	// gère les lignes de commandes avec options 
void afficheMenu();
	// affiche les options du menu 
int recupererAction();
	//scan et renvoie un entier entre 0 et 6 avec une sécurité en cas d'input non voulu
void executerAction(int action, char* filename);
	// execute action sur fichier 