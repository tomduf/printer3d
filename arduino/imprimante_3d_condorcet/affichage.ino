// affichage du texte
void affichageTexte(String text1, String text2, String text3){
  tft.fillScreen(BLUE);
  // Affichage ligne 1
  tft.setTextColor(WHITE);
  tft.setCursor(5,10);
  tft.print(text1);
  // Affichage ligne 2
  tft.setTextColor(RED);
  tft.setCursor(5,20);
  tft.print(text2);
  // Affichage ligne 3
  tft.setTextColor(BLACK);
  tft.setCursor(5,30);
  tft.print(text3);
}
