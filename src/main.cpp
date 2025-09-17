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

enum Etats {DEBUT, JEU, GAGNE, PERDU};
Etats etatDuJeu = DEBUT;

struct SequenceJeu {
  int sequenceOrdi[100]; // Séquence générée par l'ordinateur
  int sequenceJoueur[100]; // Séquence entrée par le joueur
  int longueur = 4;      // Longueur actuelle de la séquence
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
  matrix.setCursor(0, 20);
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
  for(int i = 0; i < 100; i++) {
    jeu.sequenceOrdi[i] = -1;
    jeu.sequenceJoueur[i] = -1;
  }
}

// Fonction pour remplir la séquence de l'ordinateur avec des valeurs aléatoires.
void remplirSequenceOrdi() {
  for(int i = 0; i < jeu.longueur; i++) {
    jeu.sequenceOrdi[i] = random(0, 4); // Génère un nombre aléatoire entre 0 et 3
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

// Fonction pour lire la séquence entrée par le joueur.
void lireSequenceJoueur() {
  int index = 0;
  while(index < jeu.longueur) {

    if(!isBitSet(PINC, BTN_HAUT) && jeu.sequenceJoueur[index] == -1) {
      jeu.sequenceJoueur[index] = HAUT;
      Serial.println("Haut");
      afficheBoutons(HAUT, true, true);
      delay(250);
      afficheBoutons(HAUT, false, true);
      index++;
    }
    if(!isBitSet(PINC, BTN_BAS) && jeu.sequenceJoueur[index] == -1) {
      jeu.sequenceJoueur[index] = BAS;
      Serial.println("Bas");
      afficheBoutons(BAS, true, true);
      delay(250);
      afficheBoutons(BAS, false, true);
      index++;
    }
    if(!isBitSet(PINC, BTN_GAUCHE) && jeu.sequenceJoueur[index] == -1) {
      jeu.sequenceJoueur[index] = GAUCHE;
      Serial.println("Gauche");
      afficheBoutons(GAUCHE, true, true);
      delay(250);
      afficheBoutons(GAUCHE, false, true);
      index++;
    }
    if(!isBitSet(PINC, BTN_DROITE) && jeu.sequenceJoueur[index] == -1) {
      jeu.sequenceJoueur[index] = DROITE;
      Serial.println("Droite");
      afficheBoutons(DROITE, true, true);
      delay(250);
      afficheBoutons(DROITE, false, true);
      index++;
    }
  }
}

bool VerificationSequences() {
  for(int i = 0; i < jeu.longueur; i++) {
    if(jeu.sequenceOrdi[i] != jeu.sequenceJoueur[i]) {
      return false;
    }
  }
  return true;
}

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

void etatGagne() {
  jeu.longueur++;
  if(jeu.longueur > 200) {
    jeu.longueur = 200; // Limite la longueur maximale à 100
  }
  matrix.fillScreen(matrix.Color333(0, 0, 7)); // Vert pour gagné
  matrix.setCursor(10, 10);
  matrix.setTextColor(matrix.Color333(7, 7, 7));
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
  jeu.longueur = 4; // Réinitialise la longueur de la séquence
  matrix.fillScreen(matrix.Color333(7, 0, 0)); // Rouge pour perdu
  matrix.setCursor(10, 10);
  matrix.setTextColor(matrix.Color333(7, 7, 7));
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
