# BigBinary & RSA en C

## Description

Ce projet implémente une bibliothèque de manipulation de nombres binaires de grande taille (BigBinary) en C, ainsi que des opérations mathématiques avancées et le chiffrement RSA.

Il permet de :
- Représenter des nombres binaires dynamiquement avec une structure personnalisée.
- Effectuer des opérations sur des BigBinary : addition, soustraction, comparaison, multiplication égyptienne, division par 2, etc.
- Calculer le PGCD, le modulo, et l'exponentiation modulaire.
- Chiffrer et déchiffrer des messages avec l’algorithme RSA.
- Tester les opérations sur des petits nombres binaires avec affichage en décimal pour vérification.

## Structure du projet

Le projet utilise une seule structure principale :

typedef struct {
    int *Tdigits; // tableau dynamique de chiffres binaires
    int Taille;   // nombre de chiffres
    int Signe;    // 1 = positif, -1 = négatif, 0 = nul
} BigBinary;


## Les principales fonctions incluent :
### Création et gestion

- BigBinary initBigBinary(int taille, int signe) : Initialise un nombre binaire vide.
- BigBinary createBigBinaryFromStr(const char *str) : Crée un BigBinary à partir d'une chaîne de '0' et '1'.
- void libereBigBinary(BigBinary *nb) : Libère la mémoire d’un BigBinary.
- void afficheBigBinary(BigBinary nb) : Affiche un BigBinary dans la console.

  ### Opérations sur BigBinary
- int addition(BigBinary *a, BigBinary *b, BigBinary *res) : Additionne deux BigBinary.
- int soustraction(BigBinary *a, BigBinary *b, BigBinary *res) : Soustrait b de a.
- int compareBigBinary(const BigBinary *a, const BigBinary *b) : Compare deux BigBinary.
- int inferieur(BigBinary *a, BigBinary *b) : Vérifie si a < b.
- int egale(BigBinary *a, BigBinary *b) : Vérifie si a == b.

### Opérations avancées
- BigBinary pgcd(BigBinary *a, BigBinary *b) : Calcule le PGCD (Euclide binaire).
- BigBinary modulo(BigBinary *a, BigBinary *b) : Calcule a % b.
- BigBinary Egyptienne(BigBinary *a, BigBinary *b) : Multiplication égyptienne.
- BigBinary exponentielleModulaire(BigBinary *base, BigBinary *e, BigBinary *mod) : Calcul (base^e) % mod.

### RSA
- BigBinary chiffrement_RSA(BigBinary *message, BigBinary *e, BigBinary *n) : Chiffre un message.
- BigBinary dechiffrement_RSA(BigBinary *chiffre, BigBinary *d, BigBinary *n) : Déchiffre un message.

## Utilisation

Compiler le code :

gcc -o bigbinary_rsa projet.c


## Lancer le programme :

*./bigbinary_rsa*

Entrer les nombres binaires et l’exposant pour tester les opérations.
Le programme affichera :
- Les résultats de l’addition, soustraction, PGCD, modulo, multiplication égyptienne.
- Les résultats de l’exponentielle modulaire.
- Les résultats du chiffrement et déchiffrement RSA.
- Des tests unitaires avec conversion en décimal pour vérification.

*Exemples d’entrée*
Entrez le premier nombre binaire: 1011
Entrez le deuxième nombre binaire: 110
Entrez un exposant pour les tests d'exponentielle modulaire: 101

**Notes importantes**

- Le message RSA doit être strictement inférieur à n.
- Les nombres binaires sont dynamiques : la mémoire est allouée et libérée automatiquement.
- Toutes les opérations utilisent des algorithmes "bit à bit" pour manipuler de grands nombres binaires.
- Conversion en décimal uniquement pour vérification, les calculs sont toujours en binaire.
