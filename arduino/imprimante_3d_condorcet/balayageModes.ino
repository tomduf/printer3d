void balayageModes(){
  // Appuis sur les boutons. Par sécurité, la pression doit etre maintenue
  etatBas=digitalRead(boutonBas);
  if (etatBas==LOW){   // si bouton bas appuyé
     mode = modeBas;   // on descend
     appuiBas = true;  // et on se place en mode bouton appuyé
  }
  else if (appuiBas){ // si on n'appuie plus
    mode = modeStop;          // alors on stoppe
    appuiBas = false;  // et on remet le mode à 0
  }
    
  etatStop=digitalRead(boutonStop); // bouton prioritaire sur tout
  if (etatStop==LOW){
     mode = modeStop;
  }

  etatHaut=digitalRead(boutonHaut);
  if (etatHaut==LOW){
     mode = modeHaut;
     appuiHaut = true;
  }
  else if (appuiHaut){
    mode = modeStop;
    appuiHaut = false;
  }
  
  // Balaye le mode (modifié par les boutons ou par le Raspberry
  switch (mode){
  case modeStop :                // stop
    if (pas != 0){
      vitesse = 100;
      texteInfo = "Stop";
      changerTexte = true;
      pas = 0;
    }
    break;
  case modeHaut :                // up
    if (pas != 1){
      vitesse = 100;
      texteInfo = "Haut";
      changerTexte = true;
      pas = 1;
    }
    break;
  case modeBas :                 // down
    if (pas != -1){
      vitesse = 100;
      texteInfo = "Bas";
      changerTexte = true;
      pas = -1;
    }
    break;
  case modePrint :                // impression
    if (pas != 0){
      vitesse = 100;
      texteInfo = "Print";
      changerTexte = true;
      pas = 0;
    }
    break;
  case modePlateau :             // remontée du plateau
    if (pas != -1){
      vitesse = 100;
      texteInfo = "Remontée";
      changerTexte = true;
      pas = 1;
    }
    break;
  case modeCycle :               // cycle moteur (bas -> haut - épaisseur couche)
    if (pas != -1){
      vitesse = 50;
      texteInfo = "Cycle";
      changerTexte = true;
      pas = -1;
    }
    break;
  }
  if (changerTexte)
    affichageTexte(texteInfo, "", numCouche, "");
    changerTexte = false; 

    if (mode == modeHaut || mode == modeBas){
       myStepper.step(pas);
       envoiMode();
    }
    else if (mode == modeCycle)    // cycle moteur (bas -> haut - épaisseur couche)
      cycle();
      
    else if (mode == modePlateau)  // cycle de remontée du plateau
      remonteePlateau();
}
