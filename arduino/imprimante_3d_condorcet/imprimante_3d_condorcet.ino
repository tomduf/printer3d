/*
 Programme utilisé en combinaison avec le Raspberry Pi
 */

#include <Wire.h>              // Gestion de l'i2c
#include <Stepper.h>           // Gestion du moteur PAP
#include <SD.h>                // Gestion de la carte SD de l'écran
#include <SPI.h>               // Gestion du SPI
#include <Adafruit_GFX.h>      // Bibli Adafruit d'affichage sur l'écran Oled
#include <Adafruit_SSD1331.h>  // Gestion de l'écran Oled

// liaison i2c sur le port 5 avec l'Arduino
#define ARDUINO_ADDRESS 0x05

// déclaration des broches pour l'écran (avec SPI)
#define sclk                52 // 13
#define mosi                51 // 11
#define cs                  53 // 10
#define rst                 49 // 9
#define dc                  48 // 8

// déclaration des boutons de commande manuelle du moteur
#define boutonBas           24
#define boutonStop          1
#define boutonHaut          22

// déclaration des modes de fonctionnement de l'imprimante
#define modeStop            0
#define modeHaut            1
#define modeBas             2
#define modePrint           3
#define modeEtatDiapo       4
#define modePlateau         7
#define modeCycle           9
#define modeConfig          0x0A

// définition des valeurs de messages entre le RPi et l'Arduino
#define messageStop         0
#define messagefinRemontee  6
#define messagefinCycle     8

// pour Arduino Uno/Duemilanove, etc
// MOSI sur la broche 11 uno, 51 mega
// MISO sur la broche 12, 50 mega
// SCK sur la broche 13 (standard), 52 mega
// ensuite la broche 4 va vers CS (or whatever you have set up) sur mega 46
#define SD_CS 46    // Set the chip select line to whatever you use (4 doesnt conflict with the library)

// définition des couleurs
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define BUFFPIXEL       20

// déclaration de l'écran tft pour afficher les images de la carte SD
Adafruit_SSD1331 tft = Adafruit_SSD1331(cs, dc, rst);

// Déclaration des variables
int vitesse =           100;
int mode =              0;
int pas =               0;
int etatBas, etatStop, etatHaut;

String texteInfo;
boolean changerTexte(false), modifConfig(false);
boolean appuiHaut, appuiBas, appuiStop;

// nombre de pas du moteur 42BYG304
const float stepsPerRevolution(200);

// caractéristiques mécaniques
const int pasVis(1); // mm
float epaisseurCouche(10); // en 1/100 mm
float hauteur(0);

short int numCouche(1);
short int nbCouches(10);
short int nbExpCouche(3);
short int profPlongee(5);  // en mm
short int etatDiapo(0);
short int intervalleEntreExp(1); // en secondes
short int tempsArretAvantRemontee(100); // en millisecondes
short int tempsPauseAvantExpo(100);  // en millisecondes

// nombre de pas de descente par cycle (en fonction de la profondeur de plongée)
short int pasCycleDown = (int)(profPlongee * stepsPerRevolution / pasVis);
// si une révolution fait 200 pas, et que la vis a un pas de 1mm, alors 20 pas font 1/10ème mm
short int pasParCouche = (int)(0.01 * epaisseurCouche * stepsPerRevolution / pasVis);
// nombre de pas pour la remontée d'un cycle
short int pasCycleUp = pasCycleDown - pasParCouche;
// on remonte de la valeur initiale plus 1 cm = 10mm
short int pasRemontee = nbCouches * pasParCouche + (int)(20 * stepsPerRevolution / pasVis);


// Déclaration des ports pour le moteur
// Initialisation de la bibliothèque (dans le tuto initial : broches 8 à 11, sur le mega 30 à 36 par pas de 2, sur uno 2,3, 5, 6) :

Stepper myStepper(stepsPerRevolution, 30, 32, 34, 36);

// fichier image
File bmpFile;

// informations à extraire du fichier image
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;

void setup() {
  // initialisation de l'i2c comme esclave
  Wire.begin(ARDUINO_ADDRESS);
  // Déclencheur à la réception de données
  Wire.onReceive(receptionDonnees);
  // Déclencheur lors d'une requete du RPi
  Wire.onRequest(envoiMode);
  // Initialisation de la vitesse du moteur
  myStepper.setSpeed(vitesse);
  // intialisation des boutons
  pinMode(boutonBas, INPUT);
  pinMode(boutonStop, INPUT);
  pinMode(boutonHaut, INPUT);
  // Activation de la connexion avec la SD card
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  // intialisation de l'écran OLED
  tft.begin();
  tft.fillScreen(WHITE);

  // Affichage de la grille sans les valeurs
  //affichageGrille();
  if (SD.begin(SD_CS))
    bmpDraw("logo.bmp", 0, 0);
  delay(3000);
  affichageGrille();
}

void loop() {
  // Modification de la config
  if (modifConfig) {
    // nombre de pas de descente par cycle (en fonction de la profondeur de plongée)
    pasCycleDown = (int)(profPlongee * stepsPerRevolution / pasVis);
    // si une révolution fait 200 pas, et que la vis a un pas de 1mm, alors 20 pas font 1/10ème mm
    pasParCouche = (int)(0.01 * epaisseurCouche * stepsPerRevolution / pasVis);
    // nombre de pas pour la remontée d'un cycle
    pasCycleUp = pasCycleDown - pasParCouche;
    // on remonte de la valeur initiale plus 1 cm = 10mm
    pasRemontee = nbCouches * pasParCouche + (int)(20 * stepsPerRevolution / pasVis);
    affichageTexte("", "", epaisseurCouche, "");
    modifConfig = false;
  }
  // Appuis sur les boutons. Par sécurité, la pression doit etre maintenue
  etatStop = digitalRead(boutonStop); // bouton prioritaire sur tout
  if (etatStop == LOW) {
    mode = modeStop;
  }
  etatBas = digitalRead(boutonBas);
  if (etatBas == LOW) { // si bouton bas appuyé
    mode = modeBas;   // on se place en mode "descente"
    appuiBas = true;  // et en mode "bouton appuyé"
  }
  else if (appuiBas) { // si on n'appuie plus alros qu'on était en mode "bouton appuyé"
    mode = modeStop;   // alors on se place en mode "stop"
    appuiBas = false;  // et on remet le mode à "bouton relevé"
  }

  etatHaut = digitalRead(boutonHaut);
  if (etatHaut == LOW) {
    mode = modeHaut;
    appuiHaut = true;
  }
  else if (appuiHaut) {
    mode = modeStop;
    appuiHaut = false;
  }

  // Appel au balayage de tous les modes
  balayageModes();
}

