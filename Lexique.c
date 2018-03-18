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

void afficheArbreDico(Arbre a) {
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
			afficheArbreDico(a->filsg);
		//on décrémente la position du prochain caractère à écrire pour retourner dans l'état de l'itération parente (nécéssaire car lg est static, sinon on peut la passer en paramètre de la fonction)
		lg--;

		//une fois qu'on a fini d'écrire un mot, on parcour sa branche frère
		if (a->frered != NULL)
			afficheArbreDico(a->frered);
	}
}


//FONCTION POUR LA QUESTION 1 :
void afficheDico(char* filename) {
	//affiche dans la console les mots du fichier ayant pour chemin [filename] en ordre alphabétique
	
	FILE *in = NULL;
	in = fopen(filename, "r");

	Arbre a = ConstruitArbre(in);
	afficheArbreDico(a);

	fclose(in);
}


void ecritDico(Arbre a, FILE* out) {
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
			ecritDico(a->filsg, out);
		//on décrémente la position du prochain caractère à écrire pour retourner dans l'état de l'itération parente (nécéssaire car lg est static, sinon on peut la passer en paramètre de la fonction)
		lg--;

		//une fois qu'on a fini d'écrire un mot, on parcour sa branche frère
		if (a->frered != NULL)
			ecritDico(a->frered, out);
	}
}

//FONCTION POUR LA QUESTION 2 :
void sauvegardeDico(char* filename) {
	//ecrit dans le fichier [filename].L les mots du fichier ayant pour chemin [filename] en ordre alphabétique 
	char outfilename[MAXLE] = "";
	strcat(outfilename, filename);
	strcat(outfilename, ".L");
	
	FILE *in = NULL;
	FILE *out = NULL;
	in = fopen(filename, "r");
	out = fopen(outfilename, "w");

	Arbre a = ConstruitArbre(in);

	ecritDico(a, out);

	fclose(in);
	fclose(out);
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
int present(char* filename, char* mot) {
	//affiche present  
}

int main() {

	Arbre a = NULL;
	/*
	a = alloueArbre('d');
	a->filsg = alloueArbre('e');
	a->filsg->filsg = alloueArbre('\0');
	a->frered = alloueArbre('l');
	a->frered->filsg = alloueArbre('a');
	a->frered->filsg->filsg = alloueArbre('\0');
	a->frered->filsg->frered = alloueArbre('e');
	a->frered->filsg->frered->filsg = alloueArbre('\0');
	a->frered->frered = alloueArbre('u');
	a->frered->frered->filsg = alloueArbre('n');
	a->frered->frered->filsg->filsg = alloueArbre('\0');
	a->frered->frered->filsg->filsg->frered = alloueArbre('e');
	a->frered->frered->filsg->filsg->frered->filsg = alloueArbre('\0');
	*/	
	//(a == NULL) ? printf("a null\n") : printf("a non null\n");
	/*
	ajouteMot(&a,"de");
	ajouteMot(&a,"la");
	ajouteMot(&a,"le");
	ajouteMot(&a,"l\0");
	ajouteMot(&a,"l\0");
	ajouteMot(&a,"un");
	ajouteMot(&a,"une");
	*/

	/*
	FILE *in = NULL;
	FILE *out = NULL;
	in = fopen("mots", "r");
	out = fopen("dico", "w");
	*/
	//a = ConstruitArbre(in);	
	//afficheArbJoli(a,0);

	sauvegardeDico("mots");
	/*
	fclose(in);
	fclose(out);
	*/
	/*
	printf("dico : \n");
	afficheArbreDico(a);
	*/
	/*
	char* str = "d";
	recherche(a, str) ? 
		printf("\"%\" trouvé !\n",str) : 
		printf("\"%s\" pas trouvé :(\n", str);
	*/

	return 0;
}