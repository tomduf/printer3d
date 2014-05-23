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
#define sclk                13
#define mosi                11
#define cs                  10
#define rst                 9
#define dc                  8

// déclaration des boutons de commande manuelle du moteur
#define boutonBas           7
#define boutonStop          1
#define boutonHaut          0

// déclaration des modes de fonctionnement de l'imprimante
#define modeStop            0
#define modeHaut            1
#define modeBas             2
#define modePrint           3
#define modeEtatDiapo       4
#define modePlateau         7
#define modeCycle           9

// définition des valeurs de messages entre le RPi et l'Arduino
#define messageStop         0
#define messagefinRemontee  6
#define messagefinCycle     8

// pour Arduino Uno/Duemilanove, etc
// connecter la carte SD avec MOSI sur la broche 11,
// MISO sur la broche 12
// SCK sur la broche 13 (standard)
// ensuite la broche 4 va vers CS (or whatever you have set up)
#define SD_CS 4    // Set the chip select line to whatever you use (4 doesnt conflict with the library)

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
boolean changerTexte = false;
boolean modifConfig = false;
boolean appuiHaut, appuiBas, appuiStop;

// nombre de pas du moteur 42BYG304
const float stepsPerRevolution = 200;

// caractéristiques mécaniques
const float pasVis = 1; // mm
float epaisseurCouche = 10; // en 1/100 mm
float hauteur = 0;

int numCouche = 1;
int nbCouches = 10;
int nbExpCouche = 3;
int profPlongee = 5;  // en mm
int etatDiapo = 0;
int intervalleEntreExp = 1; // en secondes
int tempsArretAvantRemontee = 100; // en millisecondes
int tempsPauseAvantExpo = 100;  // en millisecondes

// nombre de pas de descente par cycle (en fonction de la profondeur de plongée)
int pasCycleDown = (int)(profPlongee * stepsPerRevolution / pasVis);
// si une révolution fait 200 pas, et que la vis a un pas de 1mm, alors 20 pas font 1/10ème mm
int pasParCouche = (int)(0.01 * epaisseurCouche * stepsPerRevolution / pasVis);
// nombre de pas pour la remontée d'un cycle
int pasCycleUp = pasCycleDown - pasParCouche;
// on remonte de la valeur initiale plus 1 cm = 10mm
int pasRemontee = nbCouches * pasParCouche + (int)(20 * stepsPerRevolution / pasVis); 


// Déclaration des ports pour le moteur
// Initialisation de la bibliothèque (dans le tuto initial : broches 8 à 11, ici 2 à 6) :

Stepper myStepper(stepsPerRevolution, 2,3,5,6);            

// fichier image
File bmpFile;

// informations à extraire du fichier image
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;

void setup() {
  // initialisation du port série
  Serial.begin(9600);
  // initialisation de l'i2c comme esclave
  Wire.begin(ARDUINO_ADDRESS);
  // Déclencheur à la réception de données
  Wire.onReceive(receptionDonnees);
  // Déclencheur lors d'une requete du RPi
  Wire.onRequest(envoiMode);
  // Initialisation de la vitesse du moteur
  myStepper.setSpeed(vitesse);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  // intialisation des boutons
  pinMode(boutonBas, INPUT);
  pinMode(boutonStop, INPUT);
  pinMode(boutonHaut, INPUT);
  // intialisation de l'écran OLED
  tft.begin();
  // Affichage de la grille sans les valeurs
  affichageGrille();
  /*
  Serial.print("Initialisation de la carte SD...");
   if (!SD.begin(SD_CS)) {
   Serial.println("echec!");
   return;
   }
   Serial.println("SD OK!");
   */
}

void loop() {
  // Modification de la config
  if (modifConfig){
    // nombre de pas de descente par cycle (en fonction de la profondeur de plongée)
    pasCycleDown = (int)(profPlongee * stepsPerRevolution / pasVis);
    // si une révolution fait 200 pas, et que la vis a un pas de 1mm, alors 20 pas font 1/10ème mm
    pasParCouche = (int)(0.01 * epaisseurCouche * stepsPerRevolution / pasVis);
    // nombre de pas pour la remontée d'un cycle
    pasCycleUp = pasCycleDown - pasParCouche;
    // on remonte de la valeur initiale plus 1 cm = 10mm
    pasRemontee = nbCouches * pasParCouche + (int)(20 * stepsPerRevolution / pasVis); 
    modifConfig = false;
  }
  // Appuis sur les boutons. Par sécurité, la pression doit etre maintenue
  etatStop=digitalRead(boutonStop); // bouton prioritaire sur tout
  if (etatStop==LOW){
    mode = modeStop;
  }
  etatBas=digitalRead(boutonBas);
  if (etatBas==LOW){   // si bouton bas appuyé
    mode = modeBas;   // on se place en mode "descente"
    appuiBas = true;  // et en mode "bouton appuyé"
  }
  else if (appuiBas){  // si on n'appuie plus alros qu'on était en mode "bouton appuyé"
    mode = modeStop;   // alors on se place en mode "stop"
    appuiBas = false;  // et on remet le mode à "bouton relevé"
  }

  etatHaut=digitalRead(boutonHaut);
  if (etatHaut==LOW){
    mode = modeHaut;
    appuiHaut = true;
  }
  else if (appuiHaut){
    mode = modeStop;
    appuiHaut = false;
  }

  // Appel au balayage de tous les modes
  balayageModes();
}





