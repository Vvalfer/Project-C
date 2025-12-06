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
    // nombre vide ou 0
    if (nb->Taille == 0 || nb->Signe == 0) {
        return;
    }

    // 1. Réduction de la taille logique
    // En binaire on supprime le bit de poids faible (tout à droite) divise par 2.
    // Ex: 110 (6) -> 11 (3)
    nb->Taille--;

    // 2. Gestion du cas où le nombre devient 0 (si on avait Taille = 1)
    if (nb->Taille == 0) {
        nb->Signe = 0;
        free(nb->Tdigits);
        nb->Tdigits = NULL;
        return;
    }

    // 3. Ajustement dynamique de la mémoire
    int *tmp = realloc(nb->Tdigits, nb->Taille * sizeof(int));
    if (tmp != NULL) {
        nb->Tdigits = tmp;
    }
}

// ---------------------------------------------------
//* Phase 1 : Comparaison, Soustraction, Addition *//
// ---------------------------------------------------

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

// L update : j'ai ajusté ta fonction d'addition pour qu'elle suive la même logique que la soustraction,
// en utilisant des boucles similaires, et en renvoyant le résultat via un pointeur proprement pour utiliser le resultat ailleurs 
// (on en a besoin pour la phase 2)
// Addition Thomas
int addition(BigBinary *nb1, BigBinary *nb2, BigBinary *res) {
    // 1. Détermination de la taille nécessaire (max + 1 pour la retenue)
    int maxTaille = (nb1->Taille > nb2->Taille) ? nb1->Taille : nb2->Taille;
    int n = maxTaille + 1;

    // Allocation du résultat
    *res = initBigBinary(n, +1);
    if (!res->Tdigits) return -1;

    int ia = nb1->Taille - 1;
    int ib = nb2->Taille - 1;
    int retenue = 0;

    // 2. Addition chiffre par chiffre à partir de la droite
    for (int i = n - 1; i >= 0; --i) {
        int av = (ia >= 0) ? nb1->Tdigits[ia] : 0;
        int bv = (ib >= 0) ? nb2->Tdigits[ib] : 0;

        int somme = av + bv + retenue;

        res->Tdigits[i] = somme % 2;
        retenue = somme / 2;

        // Décrémentation des indices
        if (ia >= 0) --ia;
        if (ib >= 0) --ib;
    }

    // 3. Nettoyage des zéros à gauche
    int first = 0;
    while (first < res->Taille - 1 && res->Tdigits[first] == 0) {
        ++first;
    }
    if (first > 0) {
        memmove(res->Tdigits, res->Tdigits + first, (res->Taille - first) * sizeof(int));
        res->Taille -= first;
        res->Tdigits = realloc(res->Tdigits, res->Taille * sizeof(int));
    }
    return 0;
}

// ------------------------------------
//* Phase 2 PGCD, Multiplication Egyptienne, Exponentielle *//
// ------------------------------------

// Copie profonde d'un BigBinary (allocation de nouvelle mémoire)
BigBinary copieBigBinary(BigBinary *src) {
    BigBinary dest = initBigBinary(src->Taille, src->Signe);
    // On copie le contenu du tableau d'entiers
    if (src->Tdigits != NULL && dest.Tdigits != NULL) {
        memcpy(dest.Tdigits, src->Tdigits, src->Taille * sizeof(int));
    }
    return dest;
}

// Vérifie si un nombre est pair
int estPair(BigBinary *nb) {
    if (nb->Taille == 0) return 1; // 0 est pair
    return (nb->Tdigits[nb->Taille - 1] == 0);
}

// Multiplie par 2 : Ajoute un 0 à la fin
// Ex: 11 (3) -> 110 (6)
void multipliePar2(BigBinary *nb) {
    if (nb->Taille == 0 || nb->Signe == 0) return;
    nb->Taille++;
    // On agrandit le tableau pour accueillir le nouveau bit
    int *tmp = realloc(nb->Tdigits, (nb->Taille + 1) * sizeof(int));
    if (tmp != NULL) {
        nb->Tdigits = tmp;
        nb->Tdigits[nb->Taille] = 0; // Le nouveau bit de poids faible est 0
    }
}

// Algorithme d'Euclide pour le PGCD
BigBinary pgcd(BigBinary *a, BigBinary *b) {
    // Copie pour ne pas modifier les originaux
    BigBinary u = copieBigBinary(a);
    BigBinary v = copieBigBinary(b);
    int shift = 0;

    // Gestion des cas u ou v est déjà nul
    if (u.Taille == 0 || u.Signe == 0 || (u.Taille == 1 && u.Tdigits[0] == 0)) {
        libereBigBinary(&u);
        return v;
    }
    if (v.Taille == 0 || v.Signe == 0 || (v.Taille == 1 && v.Tdigits[0] == 0)) {
        libereBigBinary(&v);
        return u;
    }

    // 1: Diviser par 2 tant que u et v sont pairs
    while (estPair(&u) && estPair(&v)) {
        divisePar2(&u);
        divisePar2(&v);
        shift++;
    }

    // 2: Boucle principale
    while (!(u.Taille == 0 || u.Signe == 0 || (u.Taille == 1 && u.Tdigits[0] == 0))) {
        while (estPair(&u)) {
            divisePar2(&u);
        }
        while (estPair(&v)) {
            divisePar2(&v);
        }

        if (inferieur(&u, &v)) {
            // Swap u et v pour avoir u >= v
            BigBinary temp = u;
            u = v;
            v = temp;
        }
        // u = u - v
        BigBinary diff = {0};
        soustraction(&u, &v, &diff);

        // Mise à jour de u
        libereBigBinary(&u);
        u = diff;
    }
    // 3: Rétablir le facteur 2^shift
    for (int i = 0; i < shift; i++) {
        multipliePar2(&v);
    }

    // Nettoyage final
    libereBigBinary(&u); // u est nul ici, mais on libère proprement la structure   
    return v;
}

// Fonction Modulo
BigBinary modulo(BigBinary *a, BigBinary *b) {
    if (b->Taille == 0 || b->Signe == 0) { // Modulo par 0 n'est pas défini
        BigBinary err = initBigBinary(0, 0);
        return err;
    }
    
    // 1. Copie de a pour le resultat initial
    BigBinary r = copieBigBinary(a);
    
    // Si A < B, le modulo est A. On retourne directement la copie.
    if (inferieur(&r, (BigBinary*)b)) {
        return r;
    }

    // 2. Boucle principale : tant que r >= b
    while (!inferieur(&r, (BigBinary*)b)) {
        
        BigBinary tempB = copieBigBinary(b);
        int shift = r.Taille - tempB.Taille;
        
        if (shift > 0) {
            for (int i = 0; i < shift; i++) {
                multipliePar2(&tempB);
            }
        }
        
        if (inferieur(&r, &tempB)) {
            divisePar2(&tempB);
        }
        
        BigBinary diff = {0};
        soustraction(&r, &tempB, &diff);
        
        libereBigBinary(&r);
        r = diff;
        
        libereBigBinary(&tempB);
    }   
    return r;
}

int convertirEnDecimal(BigBinary nb) { // a degager apres test
    int decimal = 0;
    for (int i = 0; i < nb.Taille; ++i) {
        decimal = decimal * BASE + nb.Tdigits[i];
    }
    return decimal;
}
// principe : Si A est pair, on divise A par 2 et on multiplie B par 2
// Si A est impair, on divise A par 2, on multiplie B par 2 et on ajoute B au total.
BigBinary Egyptienne(BigBinary *a, BigBinary *b){

    BigBinary total = initBigBinary(0, 0);
    BigBinary A = copieBigBinary(a);
    BigBinary B = copieBigBinary(b);

    while (!(A.Taille == 0 || A.Signe == 0 || (A.Taille == 1 && A.Tdigits[0] == 0))) { // tant que A est différent de 0
        if (!estPair(&A)) {
            BigBinary sum = {0};
            if (addition(&total, &B, &sum) != 0) {
                libereBigBinary(&total);
                libereBigBinary(&A);
                libereBigBinary(&B);
                return initBigBinary(0, 0);
            }
            libereBigBinary(&total);
            total = sum;
        }
        divisePar2(&A);
        multipliePar2(&B);
    }

    libereBigBinary(&A);
    libereBigBinary(&B);
    return total;
}

BigBinary exponentielleModulaire(BigBinary *base, BigBinary *e, BigBinary *mod) {
    // Si modulo invalide alors on retourne 0
    if (mod->Taille == 0 || mod->Signe == 0) return initBigBinary(0, 0);

    BigBinary result = initBigBinary(1, +1);
    result.Tdigits[0] = 1;

    // baseMod = base % mod
    BigBinary baseMod = modulo(base, mod);

    BigBinary e = copieBigBinary(e);

    // si l'exposant = 0 alors on retourne 1 % mod
    if (e.Taille == 0 || e.Signe == 0 || (e.Taille == 1 && e.Tdigits[0] == 0)) {
        libereBigBinary(&e);
        BigBinary r = modulo(&result, mod);
        libereBigBinary(&result);
        libereBigBinary(&baseMod);
        return r;
    }

    while (!(e.Taille == 0 || e.Signe == 0 || (e.Taille == 1 && e.Tdigits[0] == 0))) {
        if (!estPair(&e)) {
            BigBinary prod = Egyptienne(&result, &baseMod);
            BigBinary newRes = modulo(&prod, mod);
            libereBigBinary(&prod);
            libereBigBinary(&result);
            result = newRes;
        }
        divisePar2(&e);

        BigBinary sq = Egyptienne(&baseMod, &baseMod);
        BigBinary newBase = modulo(&sq, mod);
        libereBigBinary(&sq);
        libereBigBinary(&baseMod);
        baseMod = newBase;
    }

    libereBigBinary(&e);
    libereBigBinary(&baseMod);
    return result;
}

int main() {
    int bits1[] = {1, 0, 1, 1, 0};
    int bits2[] = {1, 1, 0};
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
    ret = addition(&nb, &nb2, &res);
    if (ret != 0) {
        printf("Addition impossible\n");
    } else {
        printf("Valeur apres addition : \n");
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

    // test du pgcd
    BigBinary rPGCD = pgcd(&nb, &nb2);
    printf("Valeur du PGCD : \n");
    afficheBigBinary(rPGCD);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(rPGCD));

    // test du modulo
    BigBinary rMod = modulo(&nb, &nb2);
    printf("Valeur du Modulo nb %% nb2 : \n");
    afficheBigBinary(rMod);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(rMod));

    // test de la division par 2
    divisePar2(&nb);
    printf("Valeur apres division par 2 de nb1 : \n");
    afficheBigBinary(nb);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(nb));

    // Test de la multiplication éyptienne
    printf("Valeur après multiplication éyptienne: \n");
    BigBinary test_egyptienne = Egyptienne(&nb, &nb2);
    afficheBigBinary(test_egyptienne);
    afficheBigBinary(nb);
    afficheBigBinary(nb2);

    // test de l'exponentielle modulaire
    BigBinary test_exponentielle = exponentielleModulaire(&nb, &nb2, &rMod);
    printf("résultat du l'expenentiel modulaire : \n");
    afficheBigBinary(test_exponentielle);
    afficheBigBinary(nb);
    afficheBigBinary(nb2);
    

    libereBigBinary(&nb);
    libereBigBinary(&nb2);
    libereBigBinary(&res);
    return 0;
}


// louis fais modulo et euclide = OK
// thomas fais l'egyptien et exponentiel == OK BOYYYY