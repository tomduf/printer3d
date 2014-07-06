void cycle(){
      affichageTexte("Cycle","",numCouche,"Down");
      pas = -1;
     for (int i = 0; i < pasCycleDown ; i++){
        myStepper.step(pas);        
        nbPasMoteur -= pas;
      }

      affichageTexte("Cycle","",numCouche,"Up");
      delay(tempsArretAvantRemontee);
      pas = 1;
      for (int i = 0; i < pasCycleUp ; i++){ 
        myStepper.step(pas);        
        nbPasMoteur -= pas;
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
      nbPasMoteur -= pas;
    }
    affichageTexte("Montee","",numCouche,"Fin");
    Wire.write(messagefinRemontee); // Envoi de la fin de remontée du plateau au serveur
    mode = modeStop;
}

void priseReference(){
  affichage(modePriseRef, ecran1);
  pas = 1;
  while (digitalRead(boutonStop) != LOW){
    myStepper.step(pas);
  }
  affichage(modePriseRef, ecran2);
  pas = -1;
  for (int i = 0; i < 500 ; i++){
    myStepper.step(pas);        
  }
  nbPasMoteur = 0;
  affichage(modePriseRef, ecran3);
  mode = modeStop;
  delay(5000);
  
}
