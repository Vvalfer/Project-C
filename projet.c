#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

// retourne -1 si a < b, 0 si a == b, 1 si a > b
int compareBigBinary(const BigBinary *a, const BigBinary *b) {
    if (a->Taille < b->Taille) return -1;
    if (a->Taille > b->Taille) return 1;

    for (int i = 0; i < a->Taille; ++i) {
        if (a->Tdigits[i] < b->Tdigits[i]) return -1;
        if (a->Tdigits[i] > b->Tdigits[i]) return 1;
    }
    return 0;
}

// retourne 1 si a < b, 0 sinon
int inferieur(BigBinary *a, BigBinary *b) {
    return compareBigBinary(a, b) < 0;
}
// retourne 1 si a == b, 0 sinon
int egale(BigBinary *a, BigBinary *b) {
    return compareBigBinary(a, b) == 0;
}

int soustraction(BigBinary *a, BigBinary *b, BigBinary *res) {
    // si a <= b, retourne erreur
    if (compareBigBinary(a, b) <= 0) {
        if (res->Tdigits != NULL) {
            libereBigBinary(res);
        } else {
            res->Taille = 0;
            res->Signe = 0;
        }
        return -1;
    }

    int n = a->Taille; // car a > b implique Taille de a >= Taille de b
    *res = initBigBinary(n, +1);
    if (!res->Tdigits) return -1;

    int ia = a->Taille - 1;
    int ib = b->Taille - 1;
    int borrow = 0;

    // soustraction chiffre par chiffre a partir de la droite
    for (int i = n - 1; i >= 0; --i) {
        int av = (ia >= 0) ? a->Tdigits[ia] : 0;
        int bv = (ib >= 0) ? b->Tdigits[ib] : 0;

        int d = av - borrow - bv;
        if (d < 0) {
            d += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }

        res->Tdigits[i] = d;
        --ia;
        if (ib >= 0) --ib;
    }

    // enlever les zéros à gauche du résultat
    int first = 0;
    while (first < res->Taille - 1 && res->Tdigits[first] == 0) {
        ++first;
    }
    if (first > 0) {
        memmove(res->Tdigits, res->Tdigits + first, (res->Taille - first) * sizeof(int));
        res->Taille -= first;
    }
    res->Signe = +1;
    return 0;
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
    int bits1[] = {1, 1, 0, 0, 1};
    int bits2[] = {1, 1, 0, 0, 1};
    int taille = sizeof(bits1) / sizeof(bits1[0]);
    int taille2 = sizeof(bits2) / sizeof(bits2[0]);

    BigBinary nb = initBigBinary(taille, +1);
    BigBinary nb2 = initBigBinary(taille2, +1);
    BigBinary res = {0};
    for (int i = 0; i < taille; ++i) {
        nb.Tdigits[i] =  bits1[i];
    }
    for (int i = 0; i < taille2; ++i) {
        nb2.Tdigits[i] =  bits2[i];
    }

    // test unitaire des valeurs avec affichage décimal pour véfication
    // (à dégager pour uniquement utiliser createBigBinaryFromStr)
    printf("Valeur 1 initiale : \n");
    afficheBigBinary(nb);
    printf("-> Taille 1 : %d\n", nb.Taille);
    printf("Valeur 2 initiale : \n");
    printf("-> Taille 2 : %d\n", nb2.Taille);
    afficheBigBinary(nb2);
    printf("Valeur 1 en decimal : %d\n", convertirEnDecimal(nb));
    printf("Valeur 2 en decimal : %d\n", convertirEnDecimal(nb2));

    // test de la soustraction
    int ret = soustraction(&nb, &nb2, &res);
    if (ret != 0) {
        printf("Soustraction impossible\n");
    } else {
        printf("Valeur apres soustraction : \n");
        afficheBigBinary(res);
        printf("Valeur en decimal : %d\n", convertirEnDecimal(res));
    }

    // test de la comparaison
    if (inferieur(&nb, &nb2)) { // inférieur retourne 1
        printf("nb < nb2\n");
    } else if (egale(&nb, &nb2)) { // égal retourne 1
        printf("nb == nb2\n");
    } else { // inférieur et égal retournent 0
        printf("nb > nb2\n");
    }


    libereBigBinary(&nb);
    libereBigBinary(&nb2);
    libereBigBinary(&res);
    return 0;
}