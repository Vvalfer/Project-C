#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// creation d'un nombre binaire de grande taille a partir d'une chaine
BigBinary createBigBinaryFromStr(const char *str) {
    BigBinary nb;
    int n = strlen(str);
    nb.Taille = 0;

    for (int i = 0; i < n; ++i) {
        if (str[i] == '0' || str[i] == '1') {
            nb.Taille++;
        }
    }

    nb.Tdigits = malloc(sizeof(int) * nb.Taille);
    nb.Signe = +1;
    int index = 0;
    int allZero = 1;

    for (int i = 0; i < n; ++i) {
        if (str[i] == '0' || str[i] == '1') {
            nb.Tdigits[index] = str[i] - '0';
            if (nb.Tdigits[index] == 1) allZero = 0;
            index++;
        }
    }

    if (allZero) nb.Signe = 0;
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

// Liberation de la memoire allouee dynamiquement
void libereBigBinary(BigBinary *nb){
    free(nb->Tdigits);
    nb->Tdigits = NULL;
    nb->Taille = 0;
    nb->Signe = 0;
}

void divisePar2(BigBinary *nb) {
    // fonction d'exemple a faire plus tard (pas mandatory)
}

void egale(BigBinary *a, BigBinary *b) {
    // thomas a faire
}

void inferieur(BigBinary *a, BigBinary *b) {
    // louis a faire
}

int soustraction(BigBinary *a, BigBinary *b, BigBinary *res) {
    if ((a -> Taille >= b -> Taille)) { // si memes tailles et memes signe
        int taille_max = a -> Taille;
        // travailler sur allocation dynamique
        for (int i = 0; i < taille_max; ++i) { // boucle de soustraction sur chaque element du tableau
            res -> Tdigits[i] = a -> Tdigits[i] - b -> Tdigits[i]; // soustraction element par element
            if (res -> Tdigits[i] < 0) { // si negatif, on emprunte
                res -> Tdigits[i] += BASE; // on ajoute la base (2)
                res -> Tdigits[i - 1] -= 1; // on soustrait 1 a l'element precedent
            }
            res -> Taille = taille_max;
        }
        return 0; // succes
    } else {
        return -1; // erreur, a < b
    }
}

// fonction d'addition (thomas)

int convertirEnDecimal(BigBinary nb) { // a degager apres test
    int decimal = 0;
    for (int i = 0; i < nb.Taille; ++i) {
        decimal = decimal * BASE + nb.Tdigits[i];
    }
    return decimal;
}

int main() {
    int bits83[] = {1, 0, 1, 1, 0, 1};
    int bit40[] = {1, 1, 1, 1};
    int taille = sizeof(bits83) / sizeof(bits83[0]);
    int taille2 = sizeof(bit40) / sizeof(bit40[0]);

    BigBinary nb = initBigBinary(taille, +1);
    BigBinary nb2 = initBigBinary(taille2, +1);
    BigBinary res;
    for (int i = 0; i < taille; ++i) {
        nb.Tdigits[i] =  bits83[i];
    }
    for (int i = 0; i < taille2; ++i) {
        nb2.Tdigits[i] =  bit40[i];
    }

    printf("Valeur 1 initiale : ");
    afficheBigBinary(nb);
    printf("taille : %d\n", nb.Taille);
    printf("Valeur 2 initiale : ");
    printf("taille2 : %d\n", nb2.Taille);
    afficheBigBinary(nb2);
    printf("Valeur en decimal1 : %d\n", convertirEnDecimal(nb));
    printf("Valeur en decimal2 : %d\n", convertirEnDecimal(nb2));

    soustraction(&nb, &nb2, &res);
    printf("Valeur apres soustraction : ");
    afficheBigBinary(res);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(res));


    libereBigBinary(&nb);
    libereBigBinary(&nb2);
    libereBigBinary(&res);
    return 0;
}