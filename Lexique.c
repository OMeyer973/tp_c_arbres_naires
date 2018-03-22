/* HARABAGIU Léa, MEYER Olivier */

/*
https://elearning.u-pem.fr/pluginfile.php/182569/mod_resource/content/1/FilsGaucheFrereDroit.pdf
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLE 51


typedef struct noeud{
	unsigned char lettre;
	struct noeud *filsg, *frered;
} Noeud, *Arbre;

/*
arbre n-aire contenant les mots "de", "la", "le", "un", "une"
('#' représente '\0')

	|
	d--l-----u
	|  |     |
	e  a--e  n
	|  |  |  |
	#  #  #  #--e
		        |
		        #
*/

Arbre alloueArbre (char l) {
/*alloue un espace mémoire à un arbre de lettre l, filsg NULL, frered NULL
et et retourne son adresse*/
	Arbre tmp;
	tmp = (Noeud*)malloc(sizeof(Noeud));
	if (tmp != NULL) {
		tmp->lettre = l;
		tmp->filsg = NULL;
		tmp->frered = NULL;
	} else printf("malloc error\n");
	return tmp;
}

void videArbre (Arbre a) {
	//libère l'espace mémoire alloué à l'arbre a
	if (a != NULL) {
		videArbre(a->filsg);
		videArbre(a->frered);
	}
	free(a);
}

void afficheArbJoli(Arbre a, int niv) {
	/* affiche l'arbre a sous la forme d'une arborescence (fg en haut, fd en bas)
	Pour appeller le dessin de l'arbre a, taper afficheArbJoli(a,0); */
	int i = 0;
	for (i=0; i<niv; i++) {
		printf("|   ");
	}
	if (a == NULL)
		printf("-\n");
	else {
		(a->lettre == '\0') ? printf("\\0\n") : printf("%c\n", a->lettre);
		afficheArbJoli(a->filsg, 1 + niv);
		afficheArbJoli(a->frered, 1 + niv);
	}
}


void ajouteBranche(Arbre* a, char* mot) {
	//ajoute un mot à un arbre vide (écrase l'arbre si non vide)
	*a = alloueArbre(mot[0]); 

	if (*a != NULL) {
		if (mot[0] != '\0')
			ajouteBranche(&(*a)->filsg, &mot[1]); 	
	}
}

void ajouteMot(Arbre* a, char* mot) {
	//ajoute le mot dans l'arbre lexico

	//si l'arbre est vide, on ajoute la branche entière
	if (*a == NULL)
		ajouteBranche(a, mot);
	
	else {
		//si la première lettre à ajouter est plus grande que la lettre courrante de l'arbre, on l'ajoute à son frere droit
		if ((*a)->lettre < mot[0])
			ajouteMot(&((*a)->frered), mot);
		else {
			
			//si la première lettre à ajouter est égale à la lettre courante, on essaye d'ajouter la lettre suivante à son fils
			if ((*a)->lettre == mot[0]) {
				//(si la lettre à ajouter est  '\0' et égale à la lettre courante, on ne fait rien, le mot entier est déjà dans l'arbre)
				if(mot[0] != '\0') 
					ajouteMot(&((*a)->filsg), &mot[1]);
			
			//si la lettre à ajouter est inférieur à la lettre courante, elle prend sa place et la lettre courante devient son frere droit
			} else {
				Arbre tmp = NULL;
				ajouteBranche(&tmp, mot);
				tmp->frered = *a;
				*a = tmp;
			}
		}
	}
}


Arbre ConstruitArbre(char* filename) {
	// retourne l'arbre décrivant le lexique du fichier à filename
	printf("construction de l'arbre ... \n");
	FILE *in = NULL;
	in = fopen(filename, "r");

	Arbre a = NULL;
	if (in != NULL) {
		char motBuffer[MAXLE];

		while(fscanf(in,"%s", motBuffer) != EOF){		
			ajouteMot(&a, motBuffer);
		}
		fclose(in);
	} else { 
		printf("Impossible de charger le fichier \"%s\" \n", filename);
		return NULL;
	}
	printf("OK\n");
	return a;
}

//parcours préfixe : ngd
void litArbrePrefixe(Arbre* a, FILE* in) {
	/*créé l'arbre a à partir du fichier décrivant son parcours préfixe*/
	char c;
	if (fscanf(in, "%c", &c) != EOF) {
		if (c != '\n') {
			if (c ==' ') {
				*a = alloueArbre('\0');
			} else {
				*a = alloueArbre(c);
				litArbrePrefixe(&(*a)->filsg, in);
			}
			litArbrePrefixe(&(*a)->frered, in);
		}
	}
}

Arbre chargeDico(char* filename) {
	//retourne l'arbre créé à partir de son parcour préfixe décrit dans le fichier [filename].DICO
	printf("chargement du fichier .DICO ... \n");
	FILE *in = NULL;
	Arbre a = NULL;
	char dicoFilename[MAXLE] = "";
	strcat(dicoFilename, filename);
	strcat(dicoFilename, ".DICO"); 

	in = fopen(dicoFilename, "r");

	if (in != NULL) {
		litArbrePrefixe(&a, in);		
		fclose(in);
	} else {
		printf("Impossible de charger le fichier \"%s\" \n", dicoFilename);
		return NULL; //erreur à récupérer dans les fonctions appelant chargeDico
	}
	printf("OK\n");
	return a;
}

void afficheArbreLexique(Arbre a) {
	//affiche dnas la console les mots de l'arbre lexico a en ordre alphabétique

	static char buffer [MAXLE]; //chaine de caractère buffer qui contiendra tous les mots un à un
	static int lg = 0; //position du prochain caractère à écrire

	if (a != NULL) {
		//on écrit le caractère courant de l'arbre à la position courante du buffer
		buffer[lg] = a->lettre;
		lg++;
		//si on vient d'écrire le caractère '\0', on a fini d'écrire le mot, on peut l'afficher
		if (a->lettre == '\0') 
			printf("%s\n", buffer);
		else 
			//sinon on continue de parcourir le mot courant
			afficheArbreLexique(a->filsg);
		//on décrémente la position du prochain caractère à écrire pour retourner dans l'état de l'itération parente (nécéssaire car lg est static, sinon on peut la passer en paramètre de la fonction)
		lg--;

		//une fois qu'on a fini d'écrire un mot, on parcour sa branche frère
		if (a->frered != NULL)
			afficheArbreLexique(a->frered);
	}
}


//FONCTION POUR LA QUESTION 1 :
void afficheLexique(char* filename) {
	//affiche dans la console les mots du fichier ayant pour chemin [filename] en ordre alphabétique

	Arbre a = chargeDico(filename);
	if (a == NULL)
		a = ConstruitArbre(filename);

	if (a != NULL) {
		printf("Affichage du lexique ...\n");
		afficheArbreLexique(a);
	} else 
		printf("l'arbre lexico est nul. pas de résultat.\n");
	 videArbre (a);
}

void ecritLexique(Arbre a, FILE* out) {
	//ecrit dans le fichier out tous les mots de l'arbre lexico a

	static char buffer [MAXLE]; //chaine de caractère buffer qui contiendra tous les mots un à un
	static int lg = 0; //position du prochain caractère à écrire

	if (a != NULL) {
		//on écrit le caractère courant de l'arbre à la position courante du buffer
		buffer[lg] = a->lettre;
		lg++;
		//si on vient d'écrire le caractère '\0', on a fini d'écrire le mot, on peut l'afficher
		if (a->lettre == '\0') 
			fprintf(out, "%s\n", buffer);
		else 
			//sinon on continue de parcourir le mot courant
			ecritLexique(a->filsg, out);
		//on décrémente la position du prochain caractère à écrire pour retourner dans l'état de l'itération parente (nécéssaire car lg est static, sinon on peut la passer en paramètre de la fonction)
		lg--;

		//une fois qu'on a fini d'écrire un mot, on parcour sa branche frère
		if (a->frered != NULL)
			ecritLexique(a->frered, out);
	}
}

//FONCTION POUR LA QUESTION 2 :
void sauvegardeLexique(char* filename) {
	//ecrit dans le fichier [filename].L les mots du fichier ayant pour chemin [filename] en ordre alphabétique 
	
	Arbre a = chargeDico(filename);
	if (a == NULL)
		a = ConstruitArbre(filename);
	if (a != NULL) {
		printf("Sauvegarde du fichier %s.L ... \n",filename);
		char outfilename[MAXLE] = "";
		strcat(outfilename, filename);
		strcat(outfilename, ".L");
		FILE *out = NULL;
		out = fopen(outfilename, "w");
		
		ecritLexique(a, out);
		
		fclose(out);
		printf("OK\n");
	} else 
		printf("l'arbre lexico est nul. pas de résultat.\n");
	 videArbre (a);
}

int recherche(char* mot, Arbre a) {
	//retourne 1 si le mot existe dans l'arbre lexico, 0 sinon
	if  (a == NULL)
		return 0;

	//si la permière lettre du mot est la même que la racine, on continue le parcour de ce mot de l'arbre
	if (mot[0] == a->lettre) {
		//si la lettre courante est '\0' on a trouvé le mot
		if (mot[0] == '\0')
			return 1;
		return recherche(&mot[1], a->filsg);
	}
	//sinon, on va tester le frere droit de la racine
	return recherche(mot, a->frered);
}

//FONCTION POUR LA QUESTION 3 :
int present(char* mot, char* filename) {
	//affiche present si le mot [mot] est présent dans le document [filename]

	int out = -1;
	
	Arbre a = chargeDico(filename);
	if (a == NULL)
		a = ConstruitArbre(filename);
	
	if (a != NULL) {
		printf("recherche de \"%s\" dans le fichier \"%s\" ... \n", mot, filename);
		out = recherche(mot, a);
		out ? printf("present\n") : printf("absent\n");
	} else 
		printf("l'arbre lexico est nul. pas de résultat.\n");

	videArbre (a);
	return out;
}


//parcours préfixe : ngd
void ecritArbrePrefixe(Arbre a, FILE* out) {
	/*ecrit le parcours préfixe de l'arbre a dans le fichier out*/
	if (a != NULL) {
		if (a->lettre == '\0'){
			fprintf(out, " ");
		} else {
			fprintf(out, "%c", a->lettre);
			ecritArbrePrefixe(a->filsg, out);
		}
		ecritArbrePrefixe(a->frered, out);
	} else 
		fprintf(out, "\n");
}




//FONCTION POUR LA QUESTION 4 :
void sauvegardeDico(char* filename) {
	//ecrit dans le fichier [filename].DICO le parcours préfixe de l'arbre décrivant le lexique du fichier [filename] 
	char outfilename[MAXLE] = "";
	strcat(outfilename, filename);
	strcat(outfilename, ".DICO");
	
	Arbre a = ConstruitArbre(filename);
	
	if (a != NULL) {
		printf("Sauvegarde de l'arbre ... \n");
		FILE *out = NULL;
		out = fopen(outfilename, "w");
		ecritArbrePrefixe(a, out);
		fclose(out);

		printf("OK\n");
	}
	videArbre (a);
}


//FONCTION QUESTION 5 : rendu en dot
static int idNoeud;

void ecritFgfrdDotRec(Arbre a, FILE* out) {
	//ecrit le code dot représentant l'arbre a sous forme fils gauche frere droit dans le fichier out
	if (a != NULL) {

		fprintf(out, "n%d[label=\"",idNoeud);
		int currId = idNoeud;
		idNoeud++;

		if (a->lettre == '\0'){
			fprintf(out, "/0");
		} else {
			fprintf(out, "%c", a->lettre);
		}
		fprintf(out, "\"];\n\t");
		
		if (a->frered != NULL) {
			fprintf(out, "n%d -> n%d;\n\t", currId, idNoeud);
			fprintf(out, "{rank=same; n%d n%d}\n\t",currId, idNoeud);
			ecritFgfrdDotRec(a->frered, out);
		}
		if (a->filsg != NULL) {
			fprintf(out, "n%d -> n%d;\n\t", currId, idNoeud);
			ecritFgfrdDotRec(a->filsg, out);
		}
	}
}

void ecritFgfrdDot(char* filename) {
	//ecrit le code dot représentant l'arbre a sous forme fils gauche frere droit dans le fichier de nom filename

	Arbre a = chargeDico(filename);
	if (a == NULL)
		a = ConstruitArbre(filename);
	
	if (a != NULL) {
		printf("Sauvegarde du fichier %s_fgfrd.DOT ... \n",filename);
		char outfilename[MAXLE] = "";
		strcat(outfilename, filename);
		strcat(outfilename, "_fgfrd.DOT");

		FILE *out = NULL;
		out = fopen(outfilename, "w");
		
		fprintf(out, "digraph %s {\n\tnode [shape=square];\n\t",filename);
			ecritFgfrdDotRec(a, out);
		fprintf(out, "}");

		fclose(out);
		printf("OK\n");
	} else 
		printf("l'arbre lexico est nul. pas de résultat.\n");
	 videArbre (a);
}

//FONCTION POUR LA QUESTION 5 : représentation de l'arbre lexico à n fils au format dot
void ecritArbreLexDotRec(Arbre a, FILE* out, int idPere) {
	//ecrit le code dot représentant l'arbre a lexico à n fils dans le fichier out
	if (a != NULL) {

		fprintf(out, "n%d[label=\"",idNoeud);
		int currId = idNoeud;
		idNoeud++;

		if (a->lettre == '\0'){
			fprintf(out, "/0");
		} else {
			fprintf(out, "%c", a->lettre);
		}
		fprintf(out, "\"];\n\t");
		fprintf(out, "n%d -> n%d;\n\t", idPere, currId);
		
		if (a->frered != NULL) {
			fprintf(out, "{rank=same; n%d n%d}\n\t",currId, idNoeud);
			ecritArbreLexDotRec(a->frered, out, idPere);
		}
		if (a->filsg != NULL) {
			ecritArbreLexDotRec(a->filsg, out, currId);
		}
	}
}

void ecritArbreLexDot(char* filename) {
	//ecrit le code dot représentant l'arbre a lexico à n fils dans le fichier filename

	Arbre a = chargeDico(filename);
	if (a == NULL)
		a = ConstruitArbre(filename);
	
	if (a != NULL) {
		printf("Sauvegarde du fichier %s_lex.DOT ... \n", filename);
		char outfilename[MAXLE] = "";
		strcat(outfilename, filename);
		strcat(outfilename, "_lex.DOT");

		FILE *out = NULL;
		out = fopen(outfilename, "w");
		
		fprintf(out, "digraph %s {\n\t",filename);
			fprintf(out, "node [shape=square];\n\t");
			fprintf(out, "n%d[label=\"orig\"];\n\t",idNoeud);
			idNoeud++;
			ecritArbreLexDotRec(a, out, idNoeud-1);
		fprintf(out, "}");

		fclose(out);
		printf("OK\n");
	} else 
		printf("l'arbre lexico est nul. pas de résultat.\n");
	 videArbre (a);
}


//FONCTIONS DE MENU
void commande(char *option, char* mot, char* filename){
	/* gère les lignes de commandes avec options */
	switch(option[1]){
			case 'l':
				afficheLexique(filename);
				break;
			case 's':
				sauvegardeLexique(filename);
				break;
			case 'r':
				present(mot, filename);
				break;
			case 'S':
				sauvegardeDico(filename);
				break;
			case 'd':
				ecritFgfrdDot(filename);
				break;
			case 'D':
				ecritArbreLexDot(filename);
				break;
			default:
				break;		
		}

}
void afficheMenu(){
	/* affiche les options du menu */
	printf("ARBRE LEXICOGRAPHIE - MENU : \n");
	printf("---------------------------- \n");
	printf("Sélectionnez l'action voulue s'il vous plaît \n");
	printf("\t0. sortir du programme.\n");
	printf("\t1. Afficher les mots du lexique en ordre alphabétique.\n");
	printf("\t2. Sauvegarder les mots du lexique en ordre alphabétique.\n");
	printf("\t3. Rechercher un mot.\n");
	printf("\t4. Sauvegarder l'arbre\n");
	printf("\t5. Sauvegarder le fichier dot représantant l'arbre fils gauche frere droit.\n");
	printf("\t6. Sauvegarder le fichier dot représantant l'arbre lexicographique à n fils.\n");
}

int recupererAction() {
	/*scan et renvoie un entier entre 0 et 4
	 avec une sécurité en cas d'input non voulu*/
	char line[256];
	int tmp = -1;
	while (1) {
		printf("Action n° : ");
		tmp = -1;

		if (fgets(line, sizeof(line), stdin)) {
    		if (1 == sscanf(line, "%d", &tmp) &&
    			tmp >= 0 && tmp <= 6) {
        		/* input can be safely used */
        		return tmp;
     		}
		}
		printf("Erreur : veuillez entrer un entier entre 0 et 6.\n");
  	}
}

/* execute action sur fichier */
void executerAction(int action, char* filename){
	char mot[MAXLE];
	switch(action){
			case 0:
				//sortie du programme
				break;
			case 1:
				afficheLexique(filename);
				break;
			case 2:
				sauvegardeLexique(filename);
				break;
			case 3:
				printf("Indiquez le mot à rechercher : ");
				scanf("%s", mot);
				present(mot, filename);
				break;
			case 4:
				sauvegardeDico(filename);
				break;
			case 5:
				ecritFgfrdDot(filename);
				break;
			case 6:
				ecritArbreLexDot(filename);
				break;
			default:
				printf("ERREUR : action non reconnue.\n");
				break;
		}	
}

int main(int argc, char *argv[]) {
	
	/* si des options sont spécifiées */
	if(argc >= 3 && argc < 5 && argv[1]!=NULL && argv[2] != NULL){
		if(argc==3)
			commande(argv[1], NULL, argv[2]);
		else
			commande(argv[1], argv[2], argv[3]);
	}
	/* sinon on vérifie qu'il y ait bien un nom de fichier */
	else if(argc == 2 && argv[1] != NULL){
		char* filename = argv[1];
		/* affiche menu */
		afficheMenu();
		/*récupération action */
		int action = recupererAction();
		/* exécution action */
		executerAction(action, filename);
	}
	/* sinon erreur */
	else{
		printf("ERREUR : veuillez entrer un nom de fichier existant en argument ainsi que des options valides.\n");
	}

	return 0;
}