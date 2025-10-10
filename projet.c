#include <stdio.h>
#define BASE 2

// Creation d'une structure pour la..
// ..Representation d'un nombre binaire de grande taille
typedef struct {
    int *Tdigits;
    // creation d'un pointeur de tableau (dynamique) d'entiers 
    int Taille; 
    // nombre de chiffres du tableau
    int Signe;
    // 1 = positif, -1 = negatif, 0 = nul
} BigBinary ;

// intialisation d'un entier de structure BigBinary (dynamique et vide)
BigBinary initBigBinary (int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;
    nb.Signe = signe;
    nb.Tdigits = malloc(sizeof(int) * taille);
    for (int i = 0; i < taille; ++i) {
        nb.Tdigits[i] = 0;
    }
    return nb;
}

// Affichage d'un nombre binaire de grande taille
void afficheBigBinary (BigBinary nb) {
    if (nb.Signe == -1) printf("-");
    if (nb.Signe == 0 || nb.Taille == 0) {
        printf("0\n");
        return;
    }
    for (int i = 0; i < nb.Taille; ++i) {
        printf("%d", nb.Tdigits[i]);
    }
    printf("\n");
}

// Liberation de la memoire allouee dynamiquement pour un nombre binaire de grande taille
void libereBigBinary(BigBinary *nb){
    free(nb->Tdigits);
    nb->Tdigits = NULL;
    nb->Taille = 0;
    nb->Signe = 0;
}

void divisePar2(BigBinary *nb) {
}

int main() {
    int bits83[] = {1, 0, 1, 0, 0, 1, 1};
    int taille = sizeof(bits83) / sizeof(bits83[0]);

    BigBinary nb = initBigBinary(taille, +1);
    for (int i = 0, i < taille; ++i) {
        nb.Tdigits[i] =  bits83[i];
    }

    printf("Valeur initiale : ");
    afficheBigBinary(nb);

    // faire addition puis soustraction et afficher resultat


    libereBigBinary(&nb);
    return 0;
}