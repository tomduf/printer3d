// affichage du texte avec une grille
void affichageTexte(String text1, String text2, int text3, String text4){
  // Fond bleu
  tft.fillScreen(BLUE);
  // Grille avec des lignes rapides
  // Trois lignes horizontales
  tft.drawFastHLine(0, 16, 96, BLACK);
  tft.drawFastHLine(0, 32, 96, BLACK);
  tft.drawFastHLine(0, 48, 96, BLACK);
  // Une ligne verticale
  tft.drawFastVLine(48, 0, 64, BLACK);
  
  // Affichage ligne 1 (Mode)
  tft.setTextColor(WHITE);
  tft.setCursor(4,5);
  tft.print("Mode");

  tft.setTextColor(GREEN);
  tft.setCursor(52,5);
  tft.print(text1);
  
  // Affichage ligne 2 (Hauteur)
  tft.setTextColor(WHITE);
  tft.setCursor(4,21);
  tft.print("Hauteur");

  tft.setTextColor(RED);
  tft.setCursor(52,21);
  tft.print(text2);
  // Affichage ligne 3 (Couche)
  tft.setTextColor(WHITE);
  tft.setCursor(4,37);
  tft.print("Couche");

  tft.setTextColor(BLACK);
  tft.setCursor(52,37);
  tft.print(text3);
  
  // Affichage ligne 4 (Etat)
  tft.setTextColor(WHITE);
  tft.setCursor(4,53);
  tft.print("Etat");

  tft.setTextColor(RED);
  tft.setCursor(52,53);
  tft.print(text4);
}
