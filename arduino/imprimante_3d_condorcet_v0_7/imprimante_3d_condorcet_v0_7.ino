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
#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8

// boutons
#define boutonBas 7
#define boutonStop 1
#define boutonHaut 0

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

#define BUFFPIXEL 20

// pour afficher les images de la carte SD
Adafruit_SSD1331 tft = Adafruit_SSD1331(cs, dc, rst);  
// Déclaration des variables
int vitesse = 100;
int mode = 0;
int pas = 0;
int etatBas, etatStop, etatHaut;

String texteInfo; 
boolean changerTexte = false;
boolean appuiHaut, appuiBas, appuiStop;
// nombre de pas du moteur 42BYG304
const int stepsPerRevolution = 200;

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
  Wire.onRequest(envoiMode);         // Déclencheur lors d'une requete du RPi
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
  // Appuis sur les boutons. Par sécurité, la pression doit etre maintenue
  etatBas=digitalRead(boutonBas);
  if (etatBas==LOW){   // si bouton bas appuyé
     mode = 2;         // on descend
     appuiBas = true;  // et on se place en mode bouton appuyé
  }
  else if (appuiBas){ // si on n'appuie plus
    mode = 0;          // alors on stoppe
    appuiBas = false;  // et on remet le mode à 0
  }
    
  etatStop=digitalRead(boutonStop); // bouton prioritaire sur tout
  if (etatStop==LOW){
     mode = 0;
  }

  etatHaut=digitalRead(boutonHaut);
  if (etatHaut==LOW){
     mode = 1;
     appuiHaut = true;
  }
  else if (appuiHaut){
    appuiHaut = false;
    mode = 0;
  }
  
  // Balaye le mode
  switch (mode){
  case 0 :                      // stop
    if (pas != 0){
      vitesse = 100;
      texteInfo = "Stop";
      changerTexte = true;
      pas = 0;
    }
    break;
  case 1 :                      // up
    if (pas != 1){
      vitesse = 100;
      texteInfo = "Haut";
      changerTexte = true;
      pas = 1;
    }
    break;
  case 2 :                      // down
    if (pas != -1){
      vitesse = 100;
      texteInfo = "Bas";
      changerTexte = true;
      pas = -1;
    }
    break;
  case 9 :                      // cycle moteur (bas -> haut - épaisseur couche)
    if (pas != -1){
      vitesse = 50;
      texteInfo = "Cycle moteur";
      changerTexte = true;
      pas = -1;
    }
    break;
  }
  if (changerTexte)
    affichageTexte(texteInfo);
    if (mode != 0 && mode != 9){
       myStepper.step(pas);
       envoiMode();
    }
    else if (mode == 9){  // cycle moteur (bas -> haut - épaisseur couche)
      for (int i = 0; i < 1000 ; i++){
        myStepper.step(pas);        
      }
      delay(500);
      pas = 1;
      for (int i = 0; i < 980 ; i++){ //  si une révolution fait 200 pas, et que la vis a un pas de 1mm, alors 20 pas font 1/10ème mm
        myStepper.step(pas);        
      }
      delay(500);
      mode = 0;
      Wire.write(8);
    }
}

// déclencheur à la réception de données du Raspberry par l'i2c
void receptionDonnees(int byteCount){
  while(Wire.available()) {
    mode = Wire.read();
  }
}

// callback pour le renvoi de données au raspberry
void envoiMode(){
  Wire.write(mode);
}

// affichage du texte
void affichageTexte(String text){
  if (changerTexte) {
    tft.fillScreen(BLUE);
    tft.setTextColor(WHITE);
    tft.setCursor(2,10);
    tft.print(text);
    changerTexte = false; 
  }
}

