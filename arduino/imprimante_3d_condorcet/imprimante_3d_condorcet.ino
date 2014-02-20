/* 
 Programme utilisé en combinaison avec le Raspberry Pi
 */

#include <Wire.h>
#include <Stepper.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>

// Liaison i2c sur le port 5
#define SLAVE_ADDRESS 0x05

// initialisation des broches pour l'écran (avec SPI)
#define sclk                13
#define mosi                11
#define cs                  10
#define rst                 9
#define dc                  8

// boutons
#define boutonBas           7
#define boutonStop          1
#define boutonHaut          0

// Modes
#define modeStop            0
#define modeHaut            1
#define modeBas             2
#define modePlateau         7
#define modeCycle           9

// Messages
#define messageStop         0
#define messagefinRemontee  6
#define messagefinCycle     8

// Pour Arduino Uno/Duemilanove, etc
// connecter la carte SD avec MOSI sur la broche 11,
// MISO sur la broche 12
// SCK sur la broche 13 (standard)
// ensuite la broche 4 va vers CS (or whatever you have set up)
#define SD_CS 4    // Set the chip select line to whatever you use (4 doesnt conflict with the library)

// Définition des couleurs
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#define BUFFPIXEL       20

// pour afficher les images de la carte SD
Adafruit_SSD1331 tft = Adafruit_SSD1331(cs, dc, rst);  
// Déclaration des variables
int vitesse =           100;
int mode =              0;
int pas =               0;
int etatBas, etatStop, etatHaut;

String texteInfo; 
boolean changerTexte = false;
boolean appuiHaut, appuiBas, appuiStop;
// nombre de pas du moteur 42BYG304
const float stepsPerRevolution = 200;
// Caractéristiques mécaniques
const float pasVis = 1; // mm
const float epaisseurCouche = 0.1; // mm

int pasParCouche = (int)(epaisseurCouche * stepsPerRevolution / pasVis);

// Déclaration des ports pour le moteur
// Initialisation de la bibliothèque (dans le tuto initial : broches 8 à 11, ici 2 à 6) :

Stepper myStepper(stepsPerRevolution, 2,3,5,6);            

// fichier image
File bmpFile;

// informations à extraire du fichier image
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);        // initialisation de l'i2c comme esclave
  Wire.onReceive(receptionDonnees); // Déclencheur à la réception de données
  Wire.onRequest(envoiMode);        // Déclencheur lors d'une requete du RPi
  myStepper.setSpeed(vitesse);      // Initialisation de la vitesse
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  // intialisation des boutons
  pinMode(boutonBas,INPUT);
  pinMode(boutonStop,INPUT);
  pinMode(boutonHaut,INPUT);
  // intialisation de l'écran OLED
  tft.begin();
  tft.fillScreen(BLUE);
  delay(500);
  Serial.print("Initialisation de la carte SD...");
  if (!SD.begin(SD_CS)) {
    Serial.println("echec!");
    return;
  }
  Serial.println("SD OK!");
}

void loop() {
  balayageModes();
}

