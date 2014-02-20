void cycle(){
      for (int i = 0; i < 1000 ; i++){
        myStepper.step(pas);        
      }
      affichageTexte("Cycle moteur","Fin descente","");

      delay(500);
      pas = 1;
      for (int i = 0; i < 1000 - pasParCouche ; i++){ //  si une révolution fait 200 pas, et que la vis a un pas de 1mm, alors 20 pas font 1/10ème mm
        myStepper.step(pas);        
      }
      affichageTexte("Cycle moteur","Fin remontée","OK");

      delay(500);
      mode = 0;
      Wire.write(messagefinCycle); // Envoi de la fin du cycle au serveur
}

void remonteePlateau(){
    affichageTexte("Remontée du plateau","Début","");
    for (int i = 0; i < 5000 ; i++){
      myStepper.step(pas);        
    }
    delay(500);
    affichageTexte("Remontée du plateau","Fin","");
    mode = modeStop;
    Wire.write(messagefinRemontee); // Envoi de la fin de remontée du plateau au serveur
}

