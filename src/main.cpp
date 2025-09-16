/**************************************************************/
// Date de création du programme: 10 septembre 2025
// Date de la dernière modification: 16 septembre 2025
// Nom du programmeur principal: Guillaume Beaulieu
//
// But du programme: solution de base pour le laboratoire 2-3
//
// Pour obtenir cet exemple de code, voir le dépôt GitHub suivant :
// https://github.com/gbeaulieuMontmo/243-33A-MO-Labo2_1_Solution.git
/**************************************************************/
#include <defines.h>


// Fonction pour afficher l'état des boutons-poussoirs sur la matrice.
// Paramètre: bouton - le bouton qui doit être affiché (HAUT, BAS, GAUCHE, DROITE)
void affiche_boutons(int bouton, bool isFull, bool skipRefresh = false)
{
  static unsigned long ancienRafraichissement = 0;

  if((millis() - ancienRafraichissement) >= TEMPS_RAFRAICHISSEMENT || skipRefresh) {
    ancienRafraichissement = millis();

    switch(bouton) {
      case HAUT:
        if(isFull) {
          matrix.fillCircle(27, 11, 2, matrix.Color333(7, 7, 7));
        } else {
          matrix.fillCircle(27, 11, 2, matrix.Color333(0, 0, 0));
          matrix.drawCircle(27, 11, 2, matrix.Color333(7, 7, 7));
        }
        break;
      case BAS:
        if(isFull) {
          matrix.fillCircle(27, 21, 2, matrix.Color333(7, 7, 7));
        } else {
          matrix.fillCircle(27, 21, 2, matrix.Color333(0, 0, 0));
          matrix.drawCircle(27, 21, 2, matrix.Color333(7, 7, 7));
        }
        break;
      case GAUCHE:
        if(isFull) {
          matrix.fillCircle(22, 16, 2, matrix.Color333(7, 7, 7));
        } else {
          matrix.fillCircle(22, 16, 2, matrix.Color333(0, 0, 0));
          matrix.drawCircle(22, 16, 2, matrix.Color333(7, 7, 7));
        }
        break;
      case DROITE:
        if(isFull) {
          matrix.fillCircle(32, 16, 2, matrix.Color333(7, 7, 7));
        } else {
          matrix.fillCircle(32, 16, 2, matrix.Color333(0, 0, 0));
          matrix.drawCircle(32, 16, 2, matrix.Color333(7, 7, 7));
        }
        break;
    }
  }
}

void lire_boutons() {
  static bool old_btn_haut = 0; //Variable pour l'ancienne valeur du bouton haut
  static bool old_btn_bas = 0;//Variable pour l'ancienne valeur du bouton bas
  static bool old_btn_gauche = 0;//Variable pour l'ancienne valeur du bouton gauche
  static bool old_btn_droite = 0;//Variable pour l'ancienne valeur du bouton droit
    
  if(!isBitSet(PINC, BTN_HAUT) && old_btn_haut == 1) {
    affiche_boutons(HAUT, true);
    old_btn_haut = 0;
  } else if(isBitSet(PINC, BTN_HAUT) && old_btn_haut == 0) {
    affiche_boutons(HAUT, false);
    old_btn_haut = 1;
  }
  if(!isBitSet(PINC, BTN_BAS) && old_btn_bas == 1) {
    affiche_boutons(BAS, true);
    old_btn_bas = 0;
  } else if(isBitSet(PINC, BTN_BAS) && old_btn_bas == 0) {
    affiche_boutons(BAS, false);
    old_btn_bas = 1;
  }
  if(!isBitSet(PINC, BTN_GAUCHE) && old_btn_gauche == 1) {
    affiche_boutons(GAUCHE, true);
    old_btn_gauche = 0;
  } else if(isBitSet(PINC, BTN_GAUCHE) && old_btn_gauche == 0) {
    affiche_boutons(GAUCHE, false);
    old_btn_gauche = 1;
  }
  if(!isBitSet(PINC, BTN_DROITE) && old_btn_droite == 1) {
    affiche_boutons(DROITE, true);
    old_btn_droite = 0;
  } else if(isBitSet(PINC, BTN_DROITE) && old_btn_droite == 0) {
    affiche_boutons(DROITE, false);
    old_btn_droite = 1;
  }  
}

void setup() {
  Serial.begin(9600);
  matrix.begin();
  randomSeed(analogRead(0));
  DDRG = setBitM(DDRG, B00000111); // Configure les broches de la DEL1 RVB comme sorties
  DDRL = setBitM(DDRL, B11111100); // Configure les broches de la DEL2 et DEL3 RVB comme sorties
  DDRC = clearBitM(DDRC, B11111111); // Configure les broches des boutons-poussoirs comme entrées
  PORTC = setBitM(PORTC, B11111111); // Active les résistances de pullups des broches des boutons-poussoirs
  affiche_boutons(HAUT, false, true);
  affiche_boutons(BAS, false, true);
  affiche_boutons(GAUCHE, false, true);
  affiche_boutons(DROITE, false, true);
}

void loop() {

  lire_boutons();
}
