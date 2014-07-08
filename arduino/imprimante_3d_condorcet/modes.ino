void balayageModes(){  
  // Balaye le mode (modifié par les boutons ou par le Raspberry)
  switch (mode){
    case modePrincipal :
      changerTexte : true;
    case modeStop :                // stop
      if (pas != 0){
        pas = 0;
        if (texteInfo != "Stop"){
          texteInfo = "Stop";
          changerTexte = true;
        }
      }
      break;
    case modeHaut :                // up
      if (pas != 1){
        vitesse = 100;
        pas = 1;
        if (texteInfo != "Haut"){
          texteInfo = "Haut";
          changerTexte = true;
        }
      }
      break;
    case modeBas :                 // down
      if (pas != -1){
        vitesse = 100;
        pas = -1;
        if (texteInfo != "Bas"){
          texteInfo = "Bas";
          changerTexte = true;
        }
      }
      break;
    case modePrint :                // impression
      vitesse = 100;
      if (texteInfo != "Print"){
        texteInfo = "Print";
        changerTexte = true;
      }
      pas = 0;
      if (etatDiapo == 3 )
        etatDiapo = 0;
      break;
    case modeEtatDiapo :             // affichage
      vitesse = 100;
      texteInfo = "Print";
      changerTexte = true;
      pas = 0;
      etatDiapo++;
      break;
    case modePlateau :             // remontée du plateau
      if (pas != -1){
        vitesse = 100;
        if (texteInfo != "Montee"){
          texteInfo = "Montee";
          changerTexte = true;
        }
        pas = 1;
        etatDiapo = 0;
      }
      break;
    case modeCycle :               // cycle moteur (bas -> haut - épaisseur couche)
      if (pas != -1){
        vitesse = 50;
        if (texteInfo != "Cycle"){
          texteInfo = "Cycle";
          changerTexte = true;
        }
        pas = -1;
        etatDiapo=0;
      }
      break;
    case modeConfig :
      // Modification de la config
      if (modifConfig) {
        // nombre de pas de descente par cycle (en fonction de la profondeur de plongée)
        pasCycleDown = (int)(profPlongee * stepsPerRevolution / pasVis);
        // si une révolution fait 200 pas, et que la vis a un pas de 1mm, alors 20 pas font 1/10ème mm
        pasParCouche = (int)(0.01 * epaisseurCouche * stepsPerRevolution / pasVis);
        // nombre de pas pour la remontée d'un cycle
        pasCycleUp = pasCycleDown - pasParCouche;
        // on remonte de la valeur initiale plus 1 cm = 10mm
        pasRemontee = nbCouches * pasParCouche + (int)(20 * stepsPerRevolution / pasVis);
        modifConfig = false;
      }
      changerTexte = true;
      break;
  }
  if (changerTexte){
    affichage(mode, ecran1);
    changerTexte = false;
  }
  
  // Rotation d'un pas
  if (mode == modeHaut || mode == modeBas){
    myStepper.step(pas);
    nbPasMoteur -= pas;
    envoiMode();
  }
  else if (mode == modeCycle)    // cycle moteur (bas -> haut - épaisseur couche)
    cycle();
  else if (mode == modePlateau)  // cycle de remontée du plateau
    remonteePlateau();
  else if (mode == modePriseRef) // prise de référence appelée en début
    priseReference();
  else if (mode == modePrincipal) // prise de référence appelée en début
    mode = modeStop;
  else if (mode == modeConfig) // prise de référence appelée en début
    mode = modeStop;
}

