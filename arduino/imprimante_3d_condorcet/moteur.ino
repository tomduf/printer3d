void cycle(){
      affichageTexte("Cycle","",numCouche,"Down");
      delay(2000);
     for (int i = 0; i < pasCycleDown ; i++){
        myStepper.step(pas);        
      }

      delay(500);
      affichageTexte("Cycle","",numCouche,"Up");
      pas = 1;
      for (int i = 0; i < pasCycleUp ; i++){ 
        myStepper.step(pas);        
      }
      affichageTexte("Cycle","",numCouche,"Fin");

      numCouche++;
      Wire.write(messagefinCycle); // Envoi de la fin du cycle au serveur
      mode = modeStop;
      delay(500);
      
}

void remonteePlateau(){
    affichageTexte("Remontée","",numCouche,"Début");
    delay(500);
    for (int i = 0; i < 5000 ; i++){
      myStepper.step(pas);        
    }
    delay(500);
    affichageTexte("Remontée","",numCouche,"Fin");
    Wire.write(messagefinRemontee); // Envoi de la fin de remontée du plateau au serveur
}

