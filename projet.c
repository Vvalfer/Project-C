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

int convertirEnDecimal(BigBinary nb) { // a degager apres test
    int decimal = 0;
    for (int i = 0; i < nb.Taille; ++i) {
        decimal = decimal * BASE + nb.Tdigits[i];
    }
    return decimal;
}

// louis fais modulo et euclide
// thomas fais l'egyptien et exponentiel

// Addition Thomas le bg
/* Ce que je pense --- Pour additionner : il faut que les tableaux soit de la même taille
 Etape 1 : mettre les poiteur à la même taille --> faire avec realloc
 Etape 2 : faire le calcul binaire de droite à gauche (faire attention aux retenues)
 */


int addition(BigBinary *nb1, BigBinary *nb2) {  // j'ai changer le type de void a int pour pouvoir renvoyer un code d'erreur si besoin
    int maxlen; // Initialisation de la longueur maximale
    int retenue = 0; // Initialisation de la retenue, pour utiliser plus tard dans le calcul

    // Etape 1 : Mettre les tableaux de la même taille
    if (nb1->Taille > nb2->Taille) {// Si la taille de nb1 est supérieur à la taille de nb2, alors on met nb2 à la même taille
        int ancienne = nb2->Taille; // Permet de garder la taille de nb2
        nb2->Tdigits = realloc(nb2->Tdigits, nb1->Taille * sizeof(int)); // on met nb2 à la même taille
        for (int i = nb1->Taille - 1; i >= 0; i--) { // on parcourt le tableau de droite à gauche pour ajouter des cases et décaller les anciennes
            if (i >= nb1->Taille - ancienne) // Si i est supérieur à la taille de nb1 - la taille de nb2
                nb2->Tdigits[i] = nb2->Tdigits[i - (nb1->Taille - ancienne)]; // alors on décale de position les bits
            else
                nb2->Tdigits[i] = 0; // On est dans les nouvelles cases ajoutées, donc on met 0
        }
        nb2->Taille = nb1->Taille;
    } else if (nb2->Taille > nb1->Taille) { // On fait la même chose si la taille de nb2 est supérieur à nb1.
        int ancienne = nb1->Taille;
        nb1->Tdigits = realloc(nb1->Tdigits, nb2->Taille * sizeof(int));
        for (int i = nb2->Taille - 1; i >= 0; i--) {
            if (i >= nb2->Taille - ancienne)
                nb1->Tdigits[i] = nb1->Tdigits[i - (nb2->Taille - ancienne)];
            else
                nb1->Tdigits[i] = 0;
        }
        nb1->Taille = nb2->Taille;
    }

    maxlen = nb1->Taille; // Maxlen est maintenant égal à la taille des deux nombres

    BigBinary somme = initBigBinary(maxlen + 1, +1); // On initialise le résultat

    // Étape 2 : On additionne les bits de droite à gauche
    for (int i = maxlen - 1; i >= 0; i--) {
        int s = nb1->Tdigits[i] + nb2->Tdigits[i] + retenue;
        somme.Tdigits[i + 1] = s % 2;  // On récupère le reste de la division euclidienne, soit 1, soit 0.
        retenue = s / 2; // Nouvelle retenue en fonction du résultat précédent.
    }
    somme.Tdigits[0] = retenue;

    // ici il faudrait renvoyer le resultat et faire affichage en dehors de la fonction 
    printf("\nRésultat de l'addition : "); // Affichage du résultat
    afficheBigBinary(somme);
    printf("Valeur en décimal : %d\n", convertirEnDecimal(somme)); // Affichage en décimal pour vérification
    libereBigBinary(&somme);
}

int main() {
    int bits1[] = {1, 1, 0, 0, 1, 0, 1};
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
    printf("Valeur 1 initiale : "); afficheBigBinary(nb);
    printf("-> Taille 1 : %d\n", nb.Taille);
    printf("Valeur 2 initiale : "); afficheBigBinary(nb2);
    printf("-> Taille 2 : %d\n", nb2.Taille);
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

    // test de l'addition
    addition(&nb, &nb2);


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