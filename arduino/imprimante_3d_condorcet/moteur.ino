void cycle(){
      affichageTexte("Cycle","","","Down");
      for (int i = 0; i < pasCycleDown ; i++){
        myStepper.step(pas);        
      }

      delay(500);
      affichageTexte("Cycle","","","Up");
      pas = 1;
      for (int i = 0; i < pasCycleUp ; i++){ 
        myStepper.step(pas);        
      }
      affichageTexte("Cycle","","","Fin");

      delay(500);
      mode = 0;
      Wire.write(messagefinCycle); // Envoi de la fin du cycle au serveur
}

void remonteePlateau(){
    affichageTexte("Remontée","","","Début");
    for (int i = 0; i < 5000 ; i++){
      myStepper.step(pas);        
    }
    delay(500);
    affichageTexte("Remontée","","","Fin");
    Wire.write(messagefinRemontee); // Envoi de la fin de remontée du plateau au serveur
}

