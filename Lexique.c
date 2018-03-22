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


Arbre ConstruitArbre(FILE* in) {
	// retourne l'arbre décrivant le lexique du fichier in
	Arbre a = NULL;
	char motBuffer[MAXLE];

	while(fscanf(in,"%s", motBuffer) != EOF){		
		ajouteMot(&a, motBuffer);
	}
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
//TODO : charger l'arbre filename.DICO si il existe et
void afficheLexique(char* filename) {
	//affiche dans la console les mots du fichier ayant pour chemin [filename] en ordre alphabétique
	FILE *in = NULL;
	in = fopen(filename, "r");

	if (in != NULL) {
		Arbre a = ConstruitArbre(in);
		afficheArbreLexique(a);
		fclose(in);
	} else 
		printf("Impossible de charger le fichier %s \n", filename);
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
//TODO : charger l'arbre filename.DICO si il existe
void sauvegardeLexique(char* filename) {
	//ecrit dans le fichier [filename].L les mots du fichier ayant pour chemin [filename] en ordre alphabétique 
	char outfilename[MAXLE] = "";
	strcat(outfilename, filename);
	strcat(outfilename, ".L");
	
	FILE *in = NULL;
	FILE *out = NULL;
	in = fopen(filename, "r");

	if (in != NULL) {
		out = fopen(outfilename, "w");
		Arbre a = ConstruitArbre(in);
		ecritLexique(a, out);
		fclose(out);
		fclose(in);
	} else
		printf("Impossible de charger le fichier %s \n", filename);


}

int recherche(Arbre a, char* mot) {
	//retourne 1 si le mot existe dans l'arbre lexico, 0 sinon
	if  (a == NULL)
		return 0;

	//si la permière lettre du mot est la même que la racine, on continue le parcour de ce mot de l'arbre
	if (mot[0] == a->lettre) {
		//si la lettre courante est '\0' on a trouvé le mot
		if (mot[0] == '\0')
			return 1;
		return recherche(a->filsg, &mot[1]);
	}
	//sinon, on va tester le frere droit de la racine
	return recherche(a->frered, mot);
}

//FONCTION POUR LA QUESTION 3 :
//TODO : charger l'arbre filename.DICO si il existe
int present(char* mot, char* filename) {
	//affiche present si le mot [mot] est présent dans le document [filename]
	FILE *in = NULL;
	int out= -1;
	
	in = fopen(filename, "r");

		if (in != NULL) {
			Arbre a = ConstruitArbre(in);
			out = recherche(a, mot);
			out ? printf("present\n") : printf("absent\n");
			fclose(in);
		} else
			printf("Impossible de charger le fichier %s \n", filename);


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
	
	FILE *in = NULL;
	FILE *out = NULL;
	in = fopen(filename, "r");

	if (in != NULL) {
		out = fopen(outfilename, "w");
		Arbre a = ConstruitArbre(in);
		ecritArbrePrefixe(a, out);
		fclose(out);
		fclose(in);
	} else
		printf("Impossible de charger le fichier %s \n", filename);
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
		printf("Impossible de charger le fichier %s \n", dicoFilename);
		return (Arbre)-1; //erreur à récupérer dans les fonctions appelant chargeDico
	}
	
	return a;
}


//FONCTIONS DE MENU
/* gère les lignes de commandes spécifiques */
void commande(char *com, char* filename, char* mot){
	switch(com[1]){
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
			default:
				break;		
		}

}
/* affiche les options du menu */
void afficheMenu(){
	/* affiche le menu */
	printf("ARBRE LEXICOGRAPHIE - MENU : \n");
	printf("Sélectionnez l'action voulue s il vous plaît \n");
	printf("\t 1. Affichage des mots du lexique en ordre alphabétique.\n");
	printf("\t 2. Sauvegarde des mots en ordre alphabétique.\n");
	printf("\t 3. Recherche d un mot.\n");
	printf("\t 4. Sauvegarde de l'arbre\n");
}
/* récupère l'action voulue */
void recupererAction(int* action){
	/* récupère l'action vioulue par l'utilisateur */
	printf("Action n° : ");
	scanf("%d", action);
	printf("\n");
}
/* execute action sur fichier */
void executerAction(int action, char* filename){
	char* mot="";
	switch(action){
			case 1:
				printf("Affichage du lexique : \n");
				afficheLexique(filename);
				break;
			case 2:
				printf("Sauvegarde du lexique.\n");
				sauvegardeLexique(filename);
				break;
			case 3:
				printf("Indiquez le mot à rechercher : ");
				scanf("%s", mot);
				printf("\n");
				present(mot, filename);
				break;
			case 4:
				printf("Sauvegarde de l'arbre.\n");
				sauvegardeDico(filename);
				break;
			default:
				printf("ERROR : numéro hors liste.\n");
				break;
		}	
}

int main(int argc, char *argv[]) {
	
	/* on vérifie qu'il y ait une ligne de commande spécifique */
	if(argc<=3 && argc < 5&& argv[1]!=NULL && argv[2]!=NULL){
		if(argc==3)
			commande(argv[1], argv[2], NULL);
		else
			commande(argv[1], argv[2], argv[3]);
	}
	/* sinon on vérifie qu'il y ait bien un nom de fichier */
	else if(argc==2 && argv[1]!=NULL){
		char* filename=argv[1];
		/* affiche menu */
		afficheMenu();
		/*récupération action */
		int action;
		recupererAction(&action);
		/* exécution action */
		executerAction(action, filename);
	}
	/* sinon erreur */
	else{
		printf("ERROR : veuillez entrer une ligne de commande valide ou entrer un nom de fichier.\n");
	}

	return 0;
}