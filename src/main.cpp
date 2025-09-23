/**************************************************************/
// Date de création du programme: 10 septembre 2025
// Date de la dernière modification: 19 septembre 2025
// Nom du programmeur principal: Guillaume Beaulieu
//
// But du programme: solution de base pour le laboratoire 2-3
// Le jeu Simon consiste à mémoriser une séquence de boutons aléatoires
// affichée par l'ordinateur et à la reproduire en appuyant sur les
// boutons-poussoirs dans le même ordre. La séquence commence avec
// une longueur de 4 et augmente de 1 à chaque tour réussi. Si le joueur
// se trompe, le jeu se termine et affiche "Perdu". S'il réussit
// le jeu affiche "Gagné" et une nouvelle séquence commence.
//
// Ici nous voyons qu'il y a beaucoup de code dans un seul fichier. Normalement,
// on diviserait le code en plusieurs fichiers pour une meilleure organisation.
// Nous verrons comment faire cela dans les prochains laboratoires.
// Notez que les fichiers d'en-tête (defines_matrice.h et bits_manip.h) ont
// été créés pour organiser les définitions et les fonctions. Nous verrons
// après l'examen 1 comment créer des fichiers source (.cpp) pour encore plus
// d'organisation.
//
// Ajout de fonctionnalités possibles:
// - Ajouter plus de boutons-poussoirs pour une séquence plus complexe.
// - Ajouter des niveaux de difficulté avec des séquences plus longues ou des
//   délais plus courts.
// - Ajouter un mode multijoueur où deux joueurs peuvent s'affronter.
// - Ajouter un tableau de scores pour suivre les meilleurs scores des joueurs.
// - Ajouter des animations sur la matrice pour rendre le jeu plus attrayant.
// - Ajouter des niveaux de difficulté progressifs où la vitesse d'affichage
//   de la séquence augmente à chaque tour réussi.
// - Ajouter un mode "mémoire" où le joueur doit mémoriser la séquence sans
//   l'afficher à l'écran.
// - Etc.
//
// Pour obtenir cet exemple de code, voir le dépôt GitHub suivant :
// https://github.com/gbeaulieuMontmo/243-33A-MO-Labo2_3_Solution.git
/**************************************************************/
#include <defines_matrice.h>
#include <Arduino.h>

#define TAILLE_MAX_SEQUENCE 100
#define LONGUEUR_INITIALE 4
#define NOMBRE_BOUTONS 5

enum Etats
{
  DEBUT,
  JEU,
  GAGNE,
  PERDU
};
Etats etatDuJeu = DEBUT;

struct SequenceJeu
{
  int sequenceOrdi[TAILLE_MAX_SEQUENCE];
  int sequenceJoueur[TAILLE_MAX_SEQUENCE];
  int longueur = LONGUEUR_INITIALE;
};

SequenceJeu jeu;

// Fonction pour afficher le titre du jeu sur la matrice.
void afficheTitre()
{
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
//            isFull - permet d'afficher le bouton rempli ou pas (true = rempli)
//            skipRefresh - permet d'afficher le bouton même si le délai n'est pas terminé.
void afficheBoutons(int bouton, bool isFull, bool skipRefresh = false)
{
  static unsigned long ancienRafraichissement = 0;

  if ((millis() - ancienRafraichissement) >= TEMPS_RAFRAICHISSEMENT_BOUTON || skipRefresh)
  {
    ancienRafraichissement = millis();

    switch (bouton)
    {
    case HAUT:
      if (isFull)
      {
        matrix.fillCircle(27, 11, 2, matrix.Color333(7, 7, 7));
      }
      else
      {
        matrix.fillCircle(27, 11, 2, matrix.Color333(0, 0, 0));
        matrix.drawCircle(27, 11, 2, matrix.Color333(7, 7, 7));
      }
      break;
    case BAS:
      if (isFull)
      {
        matrix.fillCircle(27, 21, 2, matrix.Color333(7, 7, 7));
      }
      else
      {
        matrix.fillCircle(27, 21, 2, matrix.Color333(0, 0, 0));
        matrix.drawCircle(27, 21, 2, matrix.Color333(7, 7, 7));
      }
      break;
    case GAUCHE:
      if (isFull)
      {
        matrix.fillCircle(22, 16, 2, matrix.Color333(7, 7, 7));
      }
      else
      {
        matrix.fillCircle(22, 16, 2, matrix.Color333(0, 0, 0));
        matrix.drawCircle(22, 16, 2, matrix.Color333(7, 7, 7));
      }
      break;
    case DROITE:
      if (isFull)
      {
        matrix.fillCircle(32, 16, 2, matrix.Color333(7, 7, 7));
      }
      else
      {
        matrix.fillCircle(32, 16, 2, matrix.Color333(0, 0, 0));
        matrix.drawCircle(32, 16, 2, matrix.Color333(7, 7, 7));
      }
      break;
      case A:
      if (isFull)
      {
        matrix.fillCircle(40, 25, 2, matrix.Color333(7, 7, 7));
      }
      else
      {
        matrix.fillCircle(40, 25, 2, matrix.Color333(0, 0, 0));
        matrix.drawCircle(40, 25, 2, matrix.Color333(7, 7, 7));
      }
      break;
    }
  }
}

// Fonction pour lire l'état des boutons-poussoirs et afficher les changements sur la matrice.
void lireBoutonsDebug()
{
  static bool old_btn_haut = 0;   // Variable pour l'ancienne valeur du bouton haut
  static bool old_btn_bas = 0;    // Variable pour l'ancienne valeur du bouton bas
  static bool old_btn_gauche = 0; // Variable pour l'ancienne valeur du bouton gauche
  static bool old_btn_droite = 0;
  static bool old_btn_A = 0;  // Variable pour l'ancienne valeur du bouton droit

  if (!isBitSet(PINC, BTN_HAUT) && old_btn_haut == 1)
  {
    afficheBoutons(HAUT, true);
    old_btn_haut = 0;
  }
  else if (isBitSet(PINC, BTN_HAUT) && old_btn_haut == 0)
  {
    afficheBoutons(HAUT, false);
    old_btn_haut = 1;
  }
  if (!isBitSet(PINC, BTN_BAS) && old_btn_bas == 1)
  {
    afficheBoutons(BAS, true);
    old_btn_bas = 0;
  }
  else if (isBitSet(PINC, BTN_BAS) && old_btn_bas == 0)
  {
    afficheBoutons(BAS, false);
    old_btn_bas = 1;
  }
  if (!isBitSet(PINC, BTN_GAUCHE) && old_btn_gauche == 1)
  {
    afficheBoutons(GAUCHE, true);
    old_btn_gauche = 0;
  }
  else if (isBitSet(PINC, BTN_GAUCHE) && old_btn_gauche == 0)
  {
    afficheBoutons(GAUCHE, false);
    old_btn_gauche = 1;
  }
  if (!isBitSet(PINC, BTN_DROITE) && old_btn_droite == 1)
  {
    afficheBoutons(DROITE, true);
    old_btn_droite = 0;
  }
  else if (isBitSet(PINC, BTN_DROITE) && old_btn_droite == 0)
  {
    afficheBoutons(DROITE, false);
    old_btn_droite = 1;
  }
  if (!isBitSet(PINC, BTN_A) && old_btn_A == 1)
  {
    afficheBoutons(A, true);
    old_btn_A = 0;
  }
}

// Fonction pour réinitialiser les séquences de l'ordinateur et du joueur.
void videSequence()
{
  for (int i = 0; i < TAILLE_MAX_SEQUENCE; i++)
  {
    jeu.sequenceOrdi[i] = -1;
    jeu.sequenceJoueur[i] = -1;
  }
}

// Fonction pour remplir la séquence de l'ordinateur avec des valeurs aléatoires.
void remplirSequenceOrdi()
{
  for (int i = 0; i < jeu.longueur; i++)
  {
    jeu.sequenceOrdi[i] = random(0, NOMBRE_BOUTONS); // Génère un nombre aléatoire entre 0 et 4
  }
}

// Fonction pour afficher la séquence de l'ordinateur sur la matrice.
void afficheSequenceOrdi()
{
  for (int i = 0; i < jeu.longueur; i++)
  {
    afficheBoutons(jeu.sequenceOrdi[i], true, true);
    delay(500);
    afficheBoutons(jeu.sequenceOrdi[i], false, true);
    delay(250);
  }
}

// Fonction pour lire quel bouton a été appuyé par le joueur.
// Retourne HAUT, BAS, GAUCHE, DROITE ou -1 si aucun bouton n'est appuyé.
// Ici on inverse ! car les boutons sont actifs à l'état bas (0V).
int lireBouton()
{
  if (isBitSet(PINC, BTN_HAUT) == 0)
    return HAUT;
  if (!isBitSet(PINC, BTN_BAS))
    return BAS;
  if (!isBitSet(PINC, BTN_GAUCHE))
    return GAUCHE;
  if (!isBitSet(PINC, BTN_DROITE))
    return DROITE;
    if(!isBitSet(PINC, BTN_A))
    return A;
  return -1;
}

void lireSequenceJoueur()
{
  int index = 0;
  while (index < jeu.longueur)
  {
    int bouton = lireBouton();
    if (bouton != -1 && jeu.sequenceJoueur[index] == -1)
    {
      jeu.sequenceJoueur[index] = bouton;
      // Affichage du bouton appuyé sur le moniteur série
      Serial.println(bouton == HAUT ? "Haut" : bouton == BAS  ? "Bas"
                                           : bouton == GAUCHE ? "Gauche"
                                                              : "Droite");
      // Affichage du bouton appuyé sur la matrice
      afficheBoutons(bouton, true, true);
      delay(250);
      afficheBoutons(bouton, false, true);
      index++; // Passe au bouton suivant dans la séquence du joueur
    }
  }
}

// Fonction pour vérifier si la séquence du joueur correspond à celle de l'ordinateur.
bool VerificationSequences()
{
  for (int i = 0; i < jeu.longueur; i++)
  {
    if (jeu.sequenceOrdi[i] != jeu.sequenceJoueur[i])
    {
      return false;
    }
  }
  return true;
}

// Fonction pour gérer l'état DEBUT du jeu.
void etatDebut()
{
  // Initialisation du jeu
  afficheTitre();
  if (isBitSet(PINC, BTN_A) == 0)
  {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    afficheBoutons(HAUT, false, true);
    afficheBoutons(BAS, false, true);
    afficheBoutons(GAUCHE, false, true);
    afficheBoutons(DROITE, false, true);
    afficheBoutons(A, false, true);
    etatDuJeu = JEU;
    Serial.println("Changement d'état: JEU");

    // Génération d'une graine aléatoire basée sur le bruit électrique des broches analogiques
    // Ici on utilise A0 et A1 pour plus de variabilité et on ajoute millis() pour éviter d'avoir la même graine à chaque redémarrage rapide
    // On obtient ainsi une bien meilleure graine aléatoire.
    // On le fait pour qu'une nouvelle graine soit générée à chaque fois que le jeu commence.
    randomSeed(analogRead(A0) + analogRead(A1) + millis());
    delay(500); // Petite pause avant de commencer le jeu
  }
}

// Fonction pour gérer l'état JEU du jeu.
void etatJeu()
{
  videSequence();
  remplirSequenceOrdi();
  afficheSequenceOrdi();
  lireSequenceJoueur();
  if (VerificationSequences())
  {
    Serial.println("Correct");
    etatDuJeu = GAGNE;
    Serial.println("Changement d'état: GAGNE");
  }
  else
  {
    Serial.println("Incorrect");
    etatDuJeu = PERDU;
    Serial.println("Changement d'état: PERDU");
  }
}

// Fonction pour gérer l'état GAGNE du jeu.
void etatGagne()
{
  jeu.longueur++;
  if (jeu.longueur > TAILLE_MAX_SEQUENCE)
  {
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
  afficheBoutons(A, false, true);
  delay(500); // Petite pause avant de commencer le prochain tour
}

void etatPerdu()
{
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

void setup()
{
  Serial.begin(9600);
  matrix.begin();
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setTextColor(matrix.Color333(7, 7, 7));
  matrix.setTextSize(1);

  DDRG = setBitM(DDRG, B00000111);   // Configure les broches de la DEL1 RVB comme sorties
  DDRL = setBitM(DDRL, B11111100);   // Configure les broches de la DEL2 et DEL3 RVB comme sorties
  DDRC = clearBitM(DDRC, B11111110); // Configure les broches des boutons-poussoirs comme entrées
  PORTC = setBitM(PORTC, B11111110); // Active les résistances de pullups des broches des boutons-poussoirs
  etatDuJeu = DEBUT;
  Serial.println("Changement d'état: DEBUT");
}

void loop()
{

  switch (etatDuJeu)
  {
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
