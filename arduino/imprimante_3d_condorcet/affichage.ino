// affichage de la grille et des textes statiques
void affichageGrille(){
  // Fond blanc
  tft.fillScreen(WHITE);
  // Grille avec des lignes rapides
  // Trois lignes horizontales
  tft.drawFastHLine(0, 16, 96, BLACK);
  tft.drawFastHLine(0, 32, 96, BLACK);
  tft.drawFastHLine(0, 48, 96, BLACK);
  // Une ligne verticale
  tft.drawFastVLine(48, 0, 64, BLACK);
  
  // Affichage ligne 1 (Mode)
  tft.setTextColor(BLACK);
  tft.setCursor(4,5);
  tft.print("Mode");

  // Affichage ligne 2 (Hauteur)
  tft.setCursor(4,21);
  tft.print("Hauteur");

  // Affichage ligne 3 (Couche / nb couches)
  tft.setCursor(4,37);
  tft.print("Couche");
  tft.setCursor(72,37);
  tft.print("/");

  // Affichage ligne 4 (Etat)
  tft.setTextColor(BLACK);
  tft.setCursor(4,53);
  tft.print("Etat");
}

// affichage et rafraichissement des textes variables sur quatre lignes
void affichageTexte(String text1, String text2, int text3, String text4){
  // Affichage du mode
  tft.setTextColor(BLUE);
  tft.fillRect(52, 5, 30, 10, WHITE);
  tft.setCursor(52,5);
  tft.print(text1);
  
  // Affichage de la hauteur
  tft.setTextColor(BLACK);
  tft.fillRect(52, 21, 30, 10, WHITE);
  tft.setCursor(52,21);
  tft.print(text2);

  // Affichage de la couche active / nb couches
  tft.setTextColor(RED);
  tft.fillRect(52, 37, 25, 10, WHITE);
  tft.setCursor(52,37);
  tft.print(text3);
  
  /*tft.setTextColor(BLACK);
  tft.setCursor(78,37);
  tft.print(nbCouches);*/
  
  // Affichage de l'état
  tft.setTextColor(GREEN);
  if (etatDiapo == 0 )
    tft.fillRect(52, 53, 30, 10, WHITE);
  tft.setCursor(52,53);
  tft.print(text4);
  
  if (etatDiapo > 0){
    tft.fillRect(52, 53, 30, 10, WHITE);
    tft.fillCircle(58, 56, 5, GREEN);
  }
  if (etatDiapo > 1)
    tft.fillCircle(72, 56, 5, GREEN);
  if (etatDiapo > 2)
    tft.fillCircle(86, 56, 5, GREEN);
}

// surcharge avec un seul texte (efface tout)
void affichageTexte(String text){
  // Affichage du fond blanc
  tft.fillScreen(WHITE);
  tft.setTextColor(BLACK);
  tft.setCursor(5,20);
  tft.print(text);
}
// surcharge avec un seul texte (entier)
void affichageTexte(int text){
  // Affichage du fond blanc
  tft.fillScreen(WHITE);
  tft.setTextColor(BLACK);
  tft.setCursor(5,20);
  tft.print(text);
}

void affichage(int mode, int ecran){
  switch (mode){
    case modePriseRef :
      switch (ecran) {
        case ecran1 :
          tft.fillScreen(WHITE);
          tft.fillRect(0, 0, 96, 16, BLUE);
          tft.setTextColor(WHITE);
          tft.setCursor(4,5);
          tft.print("Prise de ref");       
          tft.setTextColor(RED);
          tft.setCursor(4,21);
          tft.print("Montee...");       
        break;
        case ecran2 :
          tft.fillRect(0, 21, 95, 10, WHITE);
          tft.setTextColor(GREEN);
          tft.setCursor(4,21);
          tft.print("Fin de course");
          delay(1000);     
          tft.setTextColor(RED);
          tft.setCursor(4,37);
          tft.print("Descente 750");       
        break;
        case ecran3 :
          tft.setTextColor(GREEN);
          tft.setCursor(4,21);
          tft.fillRect(0, 21, 95, 10, WHITE);
          tft.print("Operation OK");       
          tft.fillRect(0, 37, 95, 10, WHITE);
          tft.fillRect(0, 53, 52, 10, WHITE);
          tft.setTextColor(BLUE);
          tft.setCursor(4,53);
          tft.print("Nb pas ->");       
          tft.setTextColor(RED);
          tft.setCursor(64,53);
          tft.print(nbPasMoteur);       
        break;
      }
    break;
    case modePrincipal :
      switch (ecran) {
        case ecran1 :
          tft.fillScreen(WHITE);
          tft.fillRect(0, 0, 96, 16, BLUE);
          tft.setTextColor(WHITE);
          tft.setCursor(4,5);
          tft.print("Imprimante 3D");       
          tft.setTextColor(GREEN);
          tft.setCursor(4,21);
          tft.print("En attente");       
          tft.setCursor(4,37);
          tft.print("Raspberry Pi");       
          tft.setCursor(4,53);
          tft.setTextColor(BLUE);
          tft.print("Stop");       
          tft.setTextColor(RED);
          tft.setCursor(70,53);
          tft.print(nbPasMoteur);       
        break;
      }
    break;
    case modeConfig :
      switch (ecran) {
        case ecran1 :
          tft.fillScreen(WHITE);
          tft.fillRect(0, 0, 96, 16, BLUE);
          tft.setTextColor(WHITE);
          tft.setCursor(4,5);
          tft.print("Configuration");       
          tft.setTextColor(GREEN);
          tft.setCursor(4,21);
          tft.print("Epaisseur ");       
          tft.setCursor(60,21);
          tft.print(epaisseurCouche);       
        break;
      }
    break;
    case modeStop :
      switch (ecran) {
        case ecran1 :
          tft.fillRect(0, 53, 96, 10, WHITE);
          tft.setTextColor(BLUE);
          tft.setCursor(4,53);
          tft.print("Stop");       
          tft.setTextColor(RED);
          tft.setCursor(64,53);
          tft.print(nbPasMoteur);       
        break;
      }
    break;
    case modeHaut :
      switch (ecran) {
        case ecran1 :
          tft.fillRect(0, 53, 52, 10, WHITE);
          tft.setTextColor(BLUE);
          tft.setCursor(4,53);
          tft.print("Haut");       
        break;
      }
    break;
    case modeBas :
      switch (ecran) {
        case ecran1 :
          tft.fillRect(0, 53, 52, 10, WHITE);
          tft.setTextColor(BLUE);
          tft.setCursor(4,53);
          tft.print("Bas");       
        break;
      }
    break;
  }
}
