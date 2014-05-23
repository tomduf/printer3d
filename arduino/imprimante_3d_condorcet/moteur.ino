void cycle(){
      affichageTexte("Cycle","",numCouche,"Down");
      pas = -1;
     for (int i = 0; i < pasCycleDown ; i++){
        myStepper.step(pas);        
      }

      affichageTexte("Cycle","",numCouche,"Up");
      delay(tempsArretAvantRemontee);
      pas = 1;
      for (int i = 0; i < pasCycleUp ; i++){ 
        myStepper.step(pas);        
      }
      affichageTexte("Cycle","",numCouche,"Fin");

      numCouche++;
      Wire.write(messagefinCycle); // Envoi de la fin du cycle au serveur
      mode = modeStop;
      delay(tempsPauseAvantExpo);
      
}

void remonteePlateau(){
    affichageTexte("Montee","",numCouche,"Debut");
    pas = 1;
    for (int i = 0; i < pasRemontee ; i++){
      myStepper.step(pas);        
    }
    affichageTexte("Montee","",numCouche,"Fin");
    Wire.write(messagefinRemontee); // Envoi de la fin de remontée du plateau au serveur
    mode = modeStop;
}

