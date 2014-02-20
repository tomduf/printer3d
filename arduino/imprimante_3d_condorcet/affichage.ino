// affichage du texte avec une grille
void affichageTexte(String text1, String text2, int text3, String text4){
  // Fond bleu
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

  tft.setTextColor(BLUE);
  tft.setCursor(52,5);
  tft.print(text1);
  
  // Affichage ligne 2 (Hauteur)
  tft.setTextColor(BLACK);
  tft.setCursor(4,21);
  tft.print("Hauteur");

  tft.setTextColor(BLACK);
  tft.setCursor(52,21);
  tft.print(text2);
  // Affichage ligne 3 (Couche / nb couches)
  tft.setTextColor(BLACK);
  tft.setCursor(4,37);
  tft.print("Couche");

  tft.setTextColor(RED);
  tft.setCursor(52,37);
  tft.print(text3);
  
  tft.setTextColor(BLACK);
  tft.setCursor(75,37);
  tft.print("/");
  tft.setCursor(80,37);
  tft.print(nbCouches);
  
  // Affichage ligne 4 (Etat)
  tft.setTextColor(BLACK);
  tft.setCursor(4,53);
  tft.print("Etat");

  tft.setTextColor(GREEN);
  tft.setCursor(52,53);
  tft.print(text4);

  Serial.println(mode);

}


