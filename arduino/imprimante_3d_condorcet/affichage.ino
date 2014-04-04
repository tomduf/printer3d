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
  tft.setCursor(75,37);
  tft.print("/");

  // Affichage ligne 4 (Etat)
  tft.setTextColor(BLACK);
  tft.setCursor(4,53);
  tft.print("Etat");
}

// affichage et rafraichissement des textes variables
void affichageTexte(String text1, String text2, int text3, String text4){
  // Affichage du mode
  tft.setTextColor(BLUE);
  tft.setCursor(52,5);
  tft.print(text1);
  
  // Affichage de la hauteur
  tft.setTextColor(BLACK);
  tft.setCursor(52,21);
  tft.print(text2);

  // Affichage de la couche actibe / nb couches
  tft.setTextColor(RED);
  tft.setCursor(52,37);
  tft.print(text3);
  
  tft.setTextColor(BLACK);
  tft.setCursor(80,37);
  tft.print(nbCouches);
  
  // Affichage de l'état
  tft.setTextColor(GREEN);
  tft.setCursor(52,53);
  tft.print(text4);
  
  if (etatDiapo > 0)
    tft.fillCircle(58, 56, 5, GREEN);
  if (etatDiapo > 1)
    tft.fillCircle(72, 56, 5, GREEN);
  if (etatDiapo > 2)
    tft.fillCircle(86, 56, 5, GREEN);
}


