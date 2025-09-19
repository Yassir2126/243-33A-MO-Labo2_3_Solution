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
#include <defines_matrice.h>
#include <Arduino.h>

#define TAILLE_MAX_SEQUENCE 100
#define LONGUEUR_INITIALE 4
#define NOMBRE_BOUTONS 4

enum Etats {DEBUT, JEU, GAGNE, PERDU};
Etats etatDuJeu = DEBUT;

struct SequenceJeu {
  int sequenceOrdi[TAILLE_MAX_SEQUENCE];
  int sequenceJoueur[TAILLE_MAX_SEQUENCE];
  int longueur = LONGUEUR_INITIALE;
};

SequenceJeu jeu;

// Fonction pour afficher le titre du jeu sur la matrice.
void afficheTitre() {
  matrix.setCursor(15, 0);
  matrix.setTextColor(matrix.Color333(0, 7, 0));
  matrix.print("Simon");
  matrix.setCursor(0, 10);
  matrix.setTextColor(matrix.Color333(7, 0, 0));
  matrix.print("Appuie sur");
  matrix.setCursor(30, 20);
  matrix.setTextColor(matrix.Color333(0, 0, 7));
  matrix.print("A");
}

// Fonction pour afficher l'état des boutons-poussoirs sur la matrice.
// Paramètre: bouton - le bouton qui doit être affiché (HAUT, BAS, GAUCHE, DROITE)
void afficheBoutons(int bouton, bool isFull, bool skipRefresh = false)
{
  static unsigned long ancienRafraichissement = 0;

  if((millis() - ancienRafraichissement) >= TEMPS_RAFRAICHISSEMENT_BOUTON || skipRefresh) {
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

// Fonction pour lire l'état des boutons-poussoirs et afficher les changements sur la matrice.
void lireBoutonsDebug() {
  static bool old_btn_haut = 0; //Variable pour l'ancienne valeur du bouton haut
  static bool old_btn_bas = 0;//Variable pour l'ancienne valeur du bouton bas
  static bool old_btn_gauche = 0;//Variable pour l'ancienne valeur du bouton gauche
  static bool old_btn_droite = 0;//Variable pour l'ancienne valeur du bouton droit
    
  if(!isBitSet(PINC, BTN_HAUT) && old_btn_haut == 1) {
    afficheBoutons(HAUT, true);
    old_btn_haut = 0;
  } else if(isBitSet(PINC, BTN_HAUT) && old_btn_haut == 0) {
    afficheBoutons(HAUT, false);
    old_btn_haut = 1;
  }
  if(!isBitSet(PINC, BTN_BAS) && old_btn_bas == 1) {
    afficheBoutons(BAS, true);
    old_btn_bas = 0;
  } else if(isBitSet(PINC, BTN_BAS) && old_btn_bas == 0) {
    afficheBoutons(BAS, false);
    old_btn_bas = 1;
  }
  if(!isBitSet(PINC, BTN_GAUCHE) && old_btn_gauche == 1) {
    afficheBoutons(GAUCHE, true);
    old_btn_gauche = 0;
  } else if(isBitSet(PINC, BTN_GAUCHE) && old_btn_gauche == 0) {
    afficheBoutons(GAUCHE, false);
    old_btn_gauche = 1;
  }
  if(!isBitSet(PINC, BTN_DROITE) && old_btn_droite == 1) {
    afficheBoutons(DROITE, true);
    old_btn_droite = 0;
  } else if(isBitSet(PINC, BTN_DROITE) && old_btn_droite == 0) {
    afficheBoutons(DROITE, false);
    old_btn_droite = 1;
  }  
}


// Fonction pour gérer l'état DEBUT du jeu.
void etatDebut() {
  // Initialisation du jeu
  afficheTitre();
  if(isBitSet(PINC, BTN_A) == 0) {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    afficheBoutons(HAUT, false, true);
    afficheBoutons(BAS, false, true);
    afficheBoutons(GAUCHE, false, true);
    afficheBoutons(DROITE, false, true);
    etatDuJeu = JEU; 
    Serial.println("Changement d'état: JEU");
  }
}

// Fonction pour réinitialiser les séquences de l'ordinateur et du joueur.
void videSequence() {
  for(int i = 0; i < TAILLE_MAX_SEQUENCE; i++) {
    jeu.sequenceOrdi[i] = -1;
    jeu.sequenceJoueur[i] = -1;
  }
}

// Fonction pour remplir la séquence de l'ordinateur avec des valeurs aléatoires.
void remplirSequenceOrdi() {
  for(int i = 0; i < jeu.longueur; i++) {
    jeu.sequenceOrdi[i] = random(0, NOMBRE_BOUTONS); // Génère un nombre aléatoire entre 0 et 3
  }
}

// Fonction pour afficher la séquence de l'ordinateur sur la matrice.
void afficheSequenceOrdi() {
  for(int i = 0; i < jeu.longueur; i++) {
    afficheBoutons(jeu.sequenceOrdi[i], true, true);
    delay(500);
    afficheBoutons(jeu.sequenceOrdi[i], false, true);
    delay(250);
  }
} 

int lireBouton() {
  if(!isBitSet(PINC, BTN_HAUT)) return HAUT;
  if(!isBitSet(PINC, BTN_BAS)) return BAS;
  if(!isBitSet(PINC, BTN_GAUCHE)) return GAUCHE;
  if(!isBitSet(PINC, BTN_DROITE)) return DROITE;
  return -1;
}

void lireSequenceJoueur() {
  int index = 0;
  while(index < jeu.longueur) {
    int bouton = lireBouton();
    if(bouton != -1 && jeu.sequenceJoueur[index] == -1) {
      jeu.sequenceJoueur[index] = bouton;
      //Affichage du bouton appuyé sur le moniteur série
      Serial.println(bouton == HAUT ? "Haut" : bouton == BAS ? "Bas" : bouton == GAUCHE ? "Gauche" : "Droite");
      //Affichage du bouton appuyé sur la matrice
      afficheBoutons(bouton, true, true);
      delay(250);
      afficheBoutons(bouton, false, true);
      index++; // Passe au bouton suivant dans la séquence du joueur
    }
  }
}

// Fonction pour vérifier si la séquence du joueur correspond à celle de l'ordinateur.
bool VerificationSequences() {
  for(int i = 0; i < jeu.longueur; i++) {
    if(jeu.sequenceOrdi[i] != jeu.sequenceJoueur[i]) {
      return false;
    }
  }
  return true;
}

// Fonction pour gérer l'état JEU du jeu.
void etatJeu() {
  videSequence();
  remplirSequenceOrdi();
  afficheSequenceOrdi();
  lireSequenceJoueur();
  if(VerificationSequences()) {
    Serial.println("Correct");
    etatDuJeu = GAGNE;
    Serial.println("Changement d'état: GAGNE");
  } else {
    Serial.println("Incorrect");
    etatDuJeu = PERDU;
    Serial.println("Changement d'état: PERDU");
  }
}

// Fonction pour gérer l'état GAGNE du jeu.
void etatGagne() {
  jeu.longueur++;
  if(jeu.longueur > TAILLE_MAX_SEQUENCE) {
    jeu.longueur = TAILLE_MAX_SEQUENCE;
  }
  matrix.fillScreen(matrix.Color333(0, 0, 7)); // Vert pour gagné
  matrix.setCursor(10, 10);
  matrix.setTextColor(matrix.Color333(0, 0, 0));
  matrix.print("GAGNE!");
  delay(2000); // Affiche le message pendant 2 secondes
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  etatDuJeu = JEU;
  Serial.println("Changement d'état: JEU");
  afficheBoutons(HAUT, false, true);
  afficheBoutons(BAS, false, true);
  afficheBoutons(GAUCHE, false, true);
  afficheBoutons(DROITE, false, true);
}

void etatPerdu() {
  jeu.longueur = LONGUEUR_INITIALE;
  matrix.fillScreen(matrix.Color333(7, 0, 0)); // Rouge pour perdu
  matrix.setCursor(10, 10);
  matrix.setTextColor(matrix.Color333(0, 0, 0));
  matrix.print("PERDU!");
  delay(2000); // Affiche le message pendant 2 secondes
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  etatDuJeu = DEBUT;
  Serial.println("Changement d'état: DEBUT");
}

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setTextColor(matrix.Color333(7, 7, 7));
  matrix.setTextSize(1);
  randomSeed(random(0, 100000));
  DDRG = setBitM(DDRG, B00000111); // Configure les broches de la DEL1 RVB comme sorties
  DDRL = setBitM(DDRL, B11111100); // Configure les broches de la DEL2 et DEL3 RVB comme sorties
  DDRC = clearBitM(DDRC, B11111111); // Configure les broches des boutons-poussoirs comme entrées
  PORTC = setBitM(PORTC, B11111111); // Active les résistances de pullups des broches des boutons-poussoirs
  etatDuJeu = DEBUT;
  Serial.println("Changement d'état: DEBUT");
}

void loop() {

  switch(etatDuJeu) {
    case DEBUT:
      // Logique de l'état DEBUT
      etatDebut();
      break;

    case JEU:
      // Logique du jeu
      etatJeu();
      break;

    case GAGNE:
      // Logique de l'état GAGNE
      etatGagne();
      break;

    case PERDU:
      // Logique de l'état PERDU
      etatPerdu();
      break;
  }
}
