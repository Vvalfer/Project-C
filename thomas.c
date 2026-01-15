#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BASE 2 // La base du nombre (2 pour binaire )

// Creation d'une structure pour la representation d'un nombre binaire de grande taille
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

// Vérifie qu'une chaîne ne contient que des '0' et des '1'
int ValidBigBinaryChar(const char *str) {
    if (str == NULL || strlen(str) == 0) return 0; // Chaîne vide
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != '0' && str[i] != '1') {
            return 0; // Caractère autre que 1 ou 0 trouvé
        }
    }
    return 1;
}

// creation d'un nombre binaire de grande taille a partir d'une chaine
// ajout d'une condition pour igogner les caracteres non binaires
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

// diviser par deux un binaire
// principe : on supprime le bit de poids faible (tout à droite), exemple 110 -> 11
void divisePar2(BigBinary *nb) {
    if (nb->Taille == 0 || nb->Signe == 0) {
        return;
    }
    nb->Taille--;

    // si taille devient 0
    if (nb->Taille == 0) {
        nb->Signe = 0;
        free(nb->Tdigits);
        nb->Tdigits = NULL;
        return;
    }

    // ajustement dynamique de la mémoire
    int *tmp = realloc(nb->Tdigits, nb->Taille * sizeof(int));
    if (tmp != NULL) {
        nb->Tdigits = tmp;
    }
}


// PHASE 1 : Comparaison, Soustraction, Addition


// retourne -1 si a < b, 0 si a == b, 1 si a > b
int compareBigBinary(const BigBinary *a, const BigBinary *b) {
    int startA = 0; // Trouver le début réel de A (ignorer les 0 au début)
    while (startA < a->Taille - 1 && a->Tdigits[startA] == 0) {
        startA++;
    }

    // trouver le début réel de B
    int startB = 0;
    while (startB < b->Taille - 1 && b->Tdigits[startB] == 0) {
        startB++;
    }

    // calcul des tailles de A et B
    int realSizeA = a->Taille - startA;
    int realSizeB = b->Taille - startB;

    if (realSizeA < realSizeB) return -1;
    if (realSizeA > realSizeB) return 1;

    // si les tailles sont égales, comparer chiffre par chiffre
    for (int i = 0; i < realSizeA; ++i) {
        int valA = a->Tdigits[startA + i];
        int valB = b->Tdigits[startB + i];

        if (valA < valB) return -1;
        if (valA > valB) return 1;
    }

    return 0; // si égaux
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
    // si a < b, retourne erreur
    if (compareBigBinary(a, b) < 0) {
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

// Addition de deux BigBinary
int addition(BigBinary *nb1, BigBinary *nb2, BigBinary *res) {
    int maxTaille = (nb1->Taille > nb2->Taille) ? nb1->Taille : nb2->Taille;
    int n = maxTaille + 1;
    *res = initBigBinary(n, +1);
    if (!res->Tdigits) return -1;

    int ia = nb1->Taille - 1;
    int ib = nb2->Taille - 1;
    int retenue = 0;

    // on additionne chiffre par chiffre à partir de la droite (technique mathématique)
    for (int i = n - 1; i >= 0; --i) {
        int av = (ia >= 0) ? nb1->Tdigits[ia] : 0;
        int bv = (ib >= 0) ? nb2->Tdigits[ib] : 0;

        int somme = av + bv + retenue;

        res->Tdigits[i] = somme % 2;
        retenue = somme / 2;

        if (ia >= 0) --ia;
        if (ib >= 0) --ib;
    }

    // On retire les 0 à gauche
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

// PHASE 2 : PGCD, Multiplication Egyptienne, Exponentielle

// Copie d'un BigBinary (allocation de nouvelle mémoire)
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
void multipliePar2(BigBinary *nb) {
    if (nb->Taille == 0 || nb->Signe == 0) return;
    nb->Taille++;
    int *tmp = realloc(nb->Tdigits, nb->Taille * sizeof(int));
    if (tmp != NULL) {
        nb->Tdigits = tmp;
        nb->Tdigits[nb->Taille - 1] = 0; 
    }
}

// Algorithme d'Euclide pour le PGCD
// exemple pgcd(1101, 101) = 1 (13 et 5 sont premiers entre eux);
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

    // Diviser par 2 tant que u et v sont pairs
    while (estPair(&u) && estPair(&v)) {
        divisePar2(&u);
        divisePar2(&v);
        shift++;
    }

    while (!(u.Taille == 0 || u.Signe == 0 || (u.Taille == 1 && u.Tdigits[0] == 0))) {
        while (estPair(&u)) {
            divisePar2(&u);
        }
        while (estPair(&v)) {
            divisePar2(&v);
        }

        if (inferieur(&u, &v)) {
            // echanger u et v pour que u >= v
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
    // on remet le facteur de 2
    for (int i = 0; i < shift; i++) {
        multipliePar2(&v);
    }
    libereBigBinary(&u);
    return v;
}

// Fonction Modulo, exemple modulo(1101, 101) = 11 (13 % 5 = 3), on retient que le reste de a / b
BigBinary modulo(BigBinary *a, BigBinary *b) {
    if (b->Taille == 0 || b->Signe == 0) { // Modulo par 0 n'est pas défini
        BigBinary err = initBigBinary(0, 0);
        return err;
    }
    
    BigBinary r = copieBigBinary(a);
    
    // si A < B, le modulo est A.
    if (inferieur(&r, (BigBinary*)b)) {
        return r;
    }
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

// Conversion en décimal d'un binaire
int convertirEnDecimal(BigBinary nb) {
    int decimal = 0;
    for (int i = 0; i < nb.Taille; ++i) {
        decimal = decimal * BASE + nb.Tdigits[i];
    }
    return decimal;
}
// Multiplication égyptienne de deux BigBinary
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

// Exponentielle modulaire
// principe : on fait l'exponetielle de la base puis le modulo du resultat avec mod. exemple : (base^e) % mod
BigBinary exponentielleModulaire(BigBinary *base, BigBinary *e, BigBinary *mod) {
    // Si modulo invalide alors on retourne 0
    if (mod->Taille == 0 || mod->Signe == 0) return initBigBinary(0, 0);
    BigBinary result = initBigBinary(1, +1);
    result.Tdigits[0] = 1;
    BigBinary baseMod = modulo(base, mod);
    BigBinary eCopy = copieBigBinary(e);
    // si l'exposant = 0 alors on retourne 1 % mod
    if (eCopy.Taille == 0 || eCopy.Signe == 0 || (eCopy.Taille == 1 && eCopy.Tdigits[0] == 0)) {
        libereBigBinary(&eCopy);
        BigBinary r = modulo(&result, mod);
        libereBigBinary(&result);
        libereBigBinary(&baseMod);
        return r;
    }

    while (!(eCopy.Taille == 0 || eCopy.Signe == 0 || (eCopy.Taille == 1 && eCopy.Tdigits[0] == 0))) {
        if (!estPair(&eCopy)) {
            BigBinary prod = Egyptienne(&result, &baseMod);
            BigBinary newRes = modulo(&prod, mod);
            libereBigBinary(&prod);
            libereBigBinary(&result);
            result = newRes;
        }
        divisePar2(&eCopy);

        BigBinary sq = Egyptienne(&baseMod, &baseMod);
        BigBinary newBase = modulo(&sq, mod);
        libereBigBinary(&sq);
        libereBigBinary(&baseMod);
        baseMod = newBase;
    }

    libereBigBinary(&eCopy);
    libereBigBinary(&baseMod);
    return result;
}


// PHASE 3 : RSA, chiffrement et déchiffrement

// principe : le chiffrement RSA se fait en calculant le message^e mod n
// la clé publique est composée de (e, n) et la clé privée de (d, n)
// exemple : message = 1101, e = 11, n = 101 (5), le résultat est 11
BigBinary chiffrement_RSA(BigBinary *message, BigBinary *e, BigBinary *n) {
    if (!inferieur(message, n)) {
        printf("Erreur : le message doit être strictement inférieur à n\n");
        return initBigBinary(0, 0);
    }
    return exponentielleModulaire(message, e, n);
}

// Déchiffrement 
// Principe : le déchiffrement RSA se fait en calculant le chiffre^d mod n
// la clé privée est composée de (d, n)
// exemple : chiffre = 11, d = 101, n = 101, le résultat est 1101
BigBinary dechiffrement_RSA(BigBinary *chiffre, BigBinary *d, BigBinary *n) {
    if (chiffre->Taille == 0 || chiffre->Signe == 0) {
        printf("Erreur : chiffre invalide\n");
        return initBigBinary(0, 0);
    }
    return exponentielleModulaire(chiffre, d, n);
}

// Programme principal
int main() {
    // Variables pour les chaînes
    char str1[256];
    char str2[256];
    char str3[256];
    
    // Vérirrification d'une entrée correcte
    int saisieValide = 0;
    printf("--- Tests de la création et affichage de BigBinary ---\n\n");
    do {
        printf("Entrez le premier nombre binaire: ");
        scanf("%s", str1);

        printf("Entrez le deuxième nombre binaire: ");
        scanf("%s", str2);

        printf("Entrez un exposant pour les tests d'exponentielle modulaire: ");
        scanf("%s", str3);

        if (ValidBigBinaryChar(str1) && ValidBigBinaryChar(str2) && ValidBigBinaryChar(str3)) {
            saisieValide = 1;
        } else {
            printf("Erreur : Une ou plusieurs chaînes contiennent des caractères invalides.\n");
        }

    } while (saisieValide == 0);

    printf("\n");
    BigBinary userNum1 = createBigBinaryFromStr(str1);
    BigBinary userNum2 = createBigBinaryFromStr(str2);
    BigBinary userExp = createBigBinaryFromStr(str3);

    printf("Premier nombre binaire: \n");
    afficheBigBinary(userNum1);
    printf("Deuxième nombre binaire: \n");
    afficheBigBinary(userNum2);

    printf("\n--- Tests des opérations sur des BigBinary ---\n");


    // addition
    BigBinary sum = {0};
    if (addition(&userNum1, &userNum2, &sum) != 0) {
        printf("Addition impossible\n");
        printf("\n");
    } else {
        printf("Résultat de l'addition : \n");
        afficheBigBinary(sum);
        libereBigBinary(&sum);
        printf("\n");
    }

    // soustraction
    BigBinary diff = {0};
    if (soustraction(&userNum1, &userNum2, &diff) != 0) {
        printf("Soustraction impossible\n");
        printf("\n");
    } else {
        printf("Résultat de la soustraction : \n");
        afficheBigBinary(diff);
        libereBigBinary(&diff);
        printf("\n");
    }

    // pgcd
    BigBinary gcd = pgcd(&userNum1, &userNum2);
    printf("Résultat du PGCD : \n");
    afficheBigBinary(gcd);
    libereBigBinary(&gcd);
    printf("\n");

    // modulo
    BigBinary mod = modulo(&userNum1, &userNum2);
    printf("Résultat du Modulo : \n");
    afficheBigBinary(mod);
    libereBigBinary(&mod);
    printf("\n");

    // multiplication égyptienne
    BigBinary egyptienne = Egyptienne(&userNum1, &userNum2);
    printf("Résultat de la multiplication égyptienne : \n");
    afficheBigBinary(egyptienne);
    libereBigBinary(&egyptienne);
    printf("\n");

    // exponentielle modulaire
    BigBinary expMod = exponentielleModulaire(&userNum1, &userNum2, &userExp);
    printf("Résultat de l'exponentielle modulaire : \n");
    afficheBigBinary(expMod);
    libereBigBinary(&expMod);
    libereBigBinary(&userExp);
    printf("\n");

    // Chiffrement RSA

    // clé rsa 
    BigBinary p = createBigBinaryFromStr("11111");
    BigBinary q = createBigBinaryFromStr("111101");
    BigBinary n = Egyptienne(&p, &q);

    printf("Résultat du Chiffrement RSA : \n");
    BigBinary rsaC = chiffrement_RSA(&userNum1, &userExp, &userNum2);
    afficheBigBinary(rsaC);
    printf("\n");

    // Déchiffrement RSA
    printf("Résultat du Déchiffrement RSA : \n");
    BigBinary rsaM = dechiffrement_RSA(&rsaC, &userExp, &userNum2);
    afficheBigBinary(rsaM);
    printf("\n");

    libereBigBinary(&rsaC);
    libereBigBinary(&rsaM);
    libereBigBinary(&userNum1);
    libereBigBinary(&userNum2);

    // Test de resultat des opérations avec des chiffres binaires convertibles en décimal facilement
    int bits1[] = {1, 0, 0};
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

    printf("\n--- Tests des résultats des opérations sur des BigBinary (avec des petit entier convertibles en décimal facilement) ---\n");
    printf("\n");

    // test unitaire des valeurs avec affichage décimal pour véfication
    printf("Valeur 1 initiale : "); afficheBigBinary(nb);
    printf("-> Taille 1 : %d\n", nb.Taille);
    printf("\n");
    printf("Valeur 2 initiale : "); afficheBigBinary(nb2);
    printf("-> Taille 2 : %d\n", nb2.Taille);
    printf("\n");
    printf("Valeur 1 en decimal : %d\n", convertirEnDecimal(nb));
    printf("Valeur 2 en decimal : %d\n", convertirEnDecimal(nb2));
    printf("\n");

    // test de la soustraction
    int ret = soustraction(&nb, &nb2, &res);
    if (ret != 0) {
        printf("Soustraction nb - nb2 : Impossible (A < B)\n");
        printf("\n");
    } else {
        printf("Soustraction nb - nb2 : \n");
        afficheBigBinary(res);
        printf("Valeur en decimal : %d\n", convertirEnDecimal(res));
        printf("\n");
    }
    libereBigBinary(&res);

    // test de l'addition
    ret = addition(&nb, &nb2, &res);
    if (ret != 0) {
        printf("Addition impossible\n");
        printf("\n");
    } else {
        printf("Valeur apres addition : \n");
        afficheBigBinary(res);
        printf("Valeur en decimal : %d\n", convertirEnDecimal(res));
        printf("\n");
    }

    // test de la comparaison
    if (inferieur(&nb, &nb2)) { // inférieur retourne 1
        printf("nb < nb2\n");
        printf("\n");
    } else if (egale(&nb, &nb2)) { // égal retourne 1
        printf("nb == nb2\n");
        printf("\n");
    } else { // inférieur et égal retournent 0
        printf("nb > nb2\n");
        printf("\n");
    }

    // test du pgcd
    BigBinary rPGCD = pgcd(&nb, &nb2);
    printf("Valeur du PGCD : \n");
    afficheBigBinary(rPGCD);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(rPGCD));
    printf("\n");

    // test du modulo
    BigBinary rMod = modulo(&nb, &nb2);
    printf("Valeur du Modulo nb %% nb2 : \n");
    afficheBigBinary(rMod);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(rMod));
    printf("\n");

    // Test de la multiplication éyptienne
    printf("Valeur après multiplication éyptienne: \n");
    BigBinary test_egyptienne = Egyptienne(&nb, &nb2);
    afficheBigBinary(test_egyptienne);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(test_egyptienne));
    libereBigBinary(&test_egyptienne);
    printf("\n");

    // test de l'exponentielle modulaire
    BigBinary test_exponentielle = exponentielleModulaire(&nb, &nb2, &rMod);
    printf("résultat du l'expenentiel modulaire : \n");
    afficheBigBinary(test_exponentielle);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(test_exponentielle));
    libereBigBinary(&test_exponentielle);
    printf("\n");

    // RSA Chiffrement
    printf("Chiffrement RSA : \n");
    BigBinary resRSA = chiffrement_RSA(&nb, &nb, &nb2);
    afficheBigBinary(resRSA);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(resRSA));
    printf("\n");

    // RSA Déchiffrement    
    printf("Déchiffrement RSA : \n");
    BigBinary resRSADec = dechiffrement_RSA(&resRSA, &nb, &nb2);
    afficheBigBinary(resRSADec);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(resRSADec));
    printf("\n");

    libereBigBinary(&resRSA);
    libereBigBinary(&resRSADec);
    
    // test de la division par 2
    divisePar2(&nb);
    printf("Valeur apres division par 2 de nb1 : \n");
    afficheBigBinary(nb);
    printf("Valeur en decimal : %d\n", convertirEnDecimal(nb));
    printf("\n");

    libereBigBinary(&nb);
    libereBigBinary(&nb2);
    libereBigBinary(&res);
    return 0;
}
