#ifndef DEFINES_MATRICE_H
#define DEFINES_MATRICE_H
#include <Arduino.h>
#include <bits_manip.h>
#include <MOMO_RGB_Matrix.h>

// Définition des broches de la DEL1 RVB
#define DEL1R PG2
#define DEL1V PG1
#define DEL1B PG0
// Broches Arduino pour la DEL1 RVB
#define DEL1RA 39
#define DEL1VA 40
#define DEL1BA 41

// Définition des broches de la DEL2 RVB
#define DEL2R PL7
#define DEL2V PL6
#define DEL2B PL5
// Broches Arduino pour la DEL2 RVB
#define DEL2RA 42
#define DEL2VA 43
#define DEL2BA 44

// Définition des broches de la DEL3 RVB
#define DEL3R PL4
#define DEL3V PL3
#define DEL3B PL2
// Broches Arduino pour la DEL3 RVB
#define DEL3RA 45
#define DEL3VA 46
#define DEL3BA 47

// Définition des broches des boutons-poussoirs
#define BTN_A PC6 
#define BTN_B PC4
#define BTN_C PC5  
#define BTN_HAUT PC2
#define BTN_BAS PC7
#define BTN_GAUCHE PC1
#define BTN_DROITE PC3 

#define TEMPS_RAFRAICHISSEMENT_BOUTON 50 // en ms
#define TAUX_RAFRAICHISSEMENT_ECRAN 100 // en ms

enum Boutons {HAUT, BAS, GAUCHE, DROITE, A};

MOMO_RGB_Matrix matrix;

#endif
